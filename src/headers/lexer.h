/*
 * @author extremevoice
 * @version 1.0
 * */

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <map>
#include <memory>

#pragma once

#ifndef PROC_LEXER_H
#define PROC_LEXER_H

namespace lexer
{
    enum class ETokenType
    {
        LParen,                // (
        RParen,                // )
        LBrace,                // {
        RBrace,                // }
        LBracket,              // [
        RBracket,              // ]
        Underscore,            // _
        Period,                // .
        Comma,                 // ,
        Colon,                 // :
        AtSign,                // @
        Ampersand,             // &
        Exclamation,           // !
        Question,              // ?
        Asterisk,              // *
        AssignImmutable,       // .=
        AssignMutable,         // :=
        Tilda,                 // ~
        Int,                   // 5
        IntType,               // int,
        Identifier,            // data
        Keyword,               // object
        Plus,                  // +
        Slash,                 // /
        Skip,
    };

    struct Token;

    /// @brief Custom lexer string iterator. Should be passed inside std::unique_ptr
    class Iterator
    {
        const char* Iter_;
        const char* EOF_;

    public:
        /// @param str string to iterate over
        explicit Iterator(std::string_view str)
                : Iter_(str.begin())
                , EOF_(str.end()) {};

        char Next();
        bool HasNext();
        char Peek();
        void Advance();
    };

    /// @brief Lexical analysis class to transform source code into tokens
    class Lexer
    {
    public:
        /// @param source source code string
        /// @throws std::bad_alloc
        explicit Lexer(std::string_view source)
            : Source_(source)
            , Iterator_(std::make_unique<Iterator>(source)) {};

        std::vector<Token> Tokenize();

    protected:
        std::string_view Source_;
        std::unique_ptr<Iterator> Iterator_;
        std::map<std::string, ETokenType> Keywords_ {
                {"int", ETokenType::IntType},
        };

        /// @brief Returns token according to provided char or group of chars
        /// @param c separate source code char
        /// @return new token
        /// @throws std::logical_error
        Token Match_(const char& c);

        /// @brief Returns 'resolve' token if next char is target, otherwise returns 'reject' one
        /// @param c separate source code char
        /// @param target next char to resolve
        /// @param resolve token type if c is target
        /// @param reject token type if c is not target
        /// @return new token
        Token MatchNext_(const char& c, const char& target, ETokenType resolve, ETokenType reject);

        Token ParseDigit_(const char& c);
        Token ParseIdentifier_(const char& c);
    };

    /// @brief Source code standalone token
    class Token
    {
    public:
        ETokenType Type;
        std::string Lexeme;

        Token(ETokenType tokenType, std::string lexeme)
            : Type(tokenType)
            , Lexeme(std::move(lexeme)) {};
    };

    std::ostream& operator << (std::ostream& ostream, const Token& token);
}

#endif //PROC_LEXER_H
