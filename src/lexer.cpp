/*
 * @author extremevoice
 * @version 1.0
 * */

#include <iostream>
#include <iterator>
#include <string_view>
#include <cctype>
#include <vector>
#include <map>

#include "headers/lexer.h"

namespace lexer
{
    // ========
    // Utils
    // ========

    std::string_view tokenTypeToString(ETokenType tokenType) {
        switch (tokenType) {
            case ETokenType::LParen: return "TT_LPAREN";
            case ETokenType::RParen: return "TT_RPAREN";
            case ETokenType::LBrace: return "TT_LBRACE";
            case ETokenType::RBrace: return "TT_RBRACE";
            case ETokenType::LBracket: return "TT_LBRACKET";
            case ETokenType::RBracket: return "TT_RBRACKET";
            case ETokenType::Underscore: return "TT_UNDERSCORE";
            case ETokenType::Period: return "TT_PERIOD";
            case ETokenType::Comma: return "TT_COMMA";
            case ETokenType::Colon: return "TT_COLON";
            case ETokenType::AtSign: return "TT_AT";
            case ETokenType::Ampersand: return "TT_AMPERSAND";
            case ETokenType::Exclamation: return "TT_EXCLAMATION";
            case ETokenType::Question: return "TT_QUESTION";
            case ETokenType::Asterisk: return "TT_ASTERISK";
            case ETokenType::AssignImmutable: return "TT_ASSIGN_IMMUTABLE";
            case ETokenType::AssignMutable: return "TT_ASSIGN_MUTABLE";
            case ETokenType::Tilda: return "TT_TILDA";
            case ETokenType::Int: return "TT_INT";
            case ETokenType::IntType: return "TT_INT_TYPE";
            case ETokenType::Identifier: return "TT_IDENTIFIER";
            case ETokenType::Keyword: return "TT_KEYWORD";
            case ETokenType::Plus: return "TT_PLUS";
            case ETokenType::Slash: return "TT_SLASH";
        }

        return std::string_view();
    }

    std::ostream& operator << (std::ostream& ostream, const Token& token) {
        return ostream << "Token(" << tokenTypeToString(token.Type) << ", " <<
            "'" << token.Lexeme << "'" << ")" << std::endl;
    }

    // ========
    // Iterator
    // ========

    char Iterator::Next() {
        return *(Iter_++);
    }

    bool Iterator::HasNext() {
        return Iter_ != EOF_;
    }

    char Iterator::Peek() {
        return !HasNext() ? '\0': *Iter_;
    }

    void Iterator::Advance() {
        ++Iter_;
    }

    // =========
    // Lexer
    // =========

    std::vector<Token> Lexer::Tokenize() {
        std::vector<Token> tokens {};

        while (Iterator_->HasNext()) {
            char c = Iterator_->Next();
            Token token = Match_(c);

            if (token.Type == ETokenType::Skip) {
                continue;
            }

            tokens.push_back(token);
        }

        return tokens;
    }

    Token Lexer::Match_(const char& c) {
        switch (c) {
            case '(': return Token(ETokenType::LParen, std::string(1, c));
            case ')': return Token(ETokenType::RParen, std::string(1, c));
            case '{': return Token(ETokenType::LBrace, std::string(1, c));
            case '}': return Token(ETokenType::RBrace, std::string(1, c));
            case '[': return Token(ETokenType::LBracket, std::string(1, c));
            case ']': return Token(ETokenType::RBracket, std::string(1, c));
            case '_': return Token(ETokenType::Underscore, std::string(1, c));
            case ',': return Token(ETokenType::Comma, std::string(1, c));
            case '@': return Token(ETokenType::AtSign, std::string(1, c));
            case '&': return Token(ETokenType::Ampersand, std::string(1, c));
            case '!': return Token(ETokenType::Exclamation, std::string(1, c));
            case '?': return Token(ETokenType::Question, std::string(1, c));
            case '*': return Token(ETokenType::Asterisk, std::string(1, c));
            case '~': return Token(ETokenType::Tilda, std::string(1, c));
            case '+': return Token(ETokenType::Plus, std::string(1, c));
            case '/': return Token(ETokenType::Slash, std::string(1, c));
            case '.': {
                return MatchNext_(c, '=', ETokenType::AssignImmutable, ETokenType::Period);
            }
            case ':': {
                return MatchNext_(c, '=', ETokenType::AssignMutable, ETokenType::Colon);
            }
            default: {
                if (std::isspace(c)) {
                    return Token(ETokenType::Skip, "");
                } else if (std::isdigit(c)) {
                    return ParseDigit_(c);
                } else if (std::isalpha(c)) {
                    return ParseIdentifier_(c);
                } else {
                    throw std::logic_error("Unexpected char");
                }
            }
        }
    } // Lexer::Match_

    Token Lexer::MatchNext_(const char& c, const char& target, ETokenType resolve, ETokenType reject) {
        if (auto next = Iterator_->Peek(); next == target) {
            Iterator_->Advance();
            return Token(resolve, std::string({c, next}));
        }

        return Token(reject, std::string(1, c));
    }

    Token Lexer::ParseDigit_(const char &c) {
        std::vector<char> chars {c};

        while (Iterator_->HasNext() && std::isdigit(Iterator_->Peek())) {
            chars.push_back(Iterator_->Next());
        }

        return Token(ETokenType::Int, std::string(chars.begin(), chars.end()));
    }

    Token Lexer::ParseIdentifier_(const char &c) {
        std::vector<char> chars {c};

        while (Iterator_->HasNext() && (std::isalpha(Iterator_->Peek()) || Iterator_->Peek() == '_')) {
            chars.push_back(Iterator_->Next());
        }

        auto str = std::string(chars.begin(), chars.end());
        auto tokenType = Keywords_.find(str) != Keywords_.end() ? ETokenType::Keyword : ETokenType::Identifier;

        return Token(tokenType, str);
    }
} // namespace lexer
