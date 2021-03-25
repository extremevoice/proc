#include <map>

#include "gtest/gtest.h"
#include "../src/headers/lexer.h"

class LexerTest : public lexer::Lexer
{
public:
    using lexer::Lexer::Lexer;
    using ET = lexer::ETokenType;

    lexer::Iterator* GetIterator() {
        return Iterator_.get();
    }

    lexer::Token Match(const char& c) {
        return Match_(c);
    }

    lexer::Token MatchNext(const char& c, const char& target, ET resolve, ET reject) {
        return MatchNext_(c, target, resolve, reject);
    }

    lexer::Token ParseDigit(const char& c) {
        return ParseDigit_(c);
    }

    lexer::Token ParseIdentifier(const char& c) {
        return ParseIdentifier_(c);
    }
};

class LexerTestParametrized: public testing::TestWithParam<std::pair<std::string_view, lexer::Token>>
{
public:
    LexerTest LT;

    LexerTestParametrized()
        : LT(LexerTest("")) {};

    void SetUp(const std::string_view source) {
        LT = LexerTest(source);
    }
};

TEST(LexerTest, LexerMatch) {
    auto lexer = LexerTest("@");
    auto expectedToken = lexer::Token(lexer::ETokenType::AtSign, "@");
    auto givenToken = lexer.Match('@');

    ASSERT_EQ(givenToken.Type, expectedToken.Type);
    ASSERT_EQ(givenToken.Lexeme, expectedToken.Lexeme);
}

TEST(LexerTest, LexerMatchNext) {
    using ET = lexer::ETokenType;
    auto lexer = LexerTest(".=");
    lexer.GetIterator()->Advance();

    auto expectedToken = lexer::Token(ET::AssignImmutable, ".=");
    auto givenToken = lexer.MatchNext('.', '=', ET::AssignImmutable, ET::Period);

    ASSERT_EQ(givenToken.Type, expectedToken.Type);
    ASSERT_EQ(givenToken.Lexeme, expectedToken.Lexeme);
}

TEST(LexerTest, LexerParseDigit) {
    auto lexer = LexerTest("5000");

    auto expectedToken = lexer::Token(lexer::ETokenType::Int, "5000");
    auto givenToken = lexer.ParseDigit(lexer.GetIterator()->Next());

    ASSERT_EQ(givenToken.Type, expectedToken.Type);
    ASSERT_EQ(givenToken.Lexeme, expectedToken.Lexeme);
}

TEST_P(LexerTestParametrized, LexerParseIdentifier) {
    auto [source, expectedToken] = GetParam();
    SetUp(source);

    auto givenToken = LT.ParseIdentifier(LT.GetIterator()->Next());

    ASSERT_EQ(givenToken.Type, expectedToken.Type);
    ASSERT_EQ(givenToken.Lexeme, expectedToken.Lexeme);
}

INSTANTIATE_TEST_CASE_P(
        LexerTests,
        LexerTestParametrized,
        testing::Values(
                std::make_pair("data", lexer::Token(lexer::ETokenType::Identifier, "data")),
                std::make_pair("int", lexer::Token(lexer::ETokenType::Keyword, "int"))
        ));
