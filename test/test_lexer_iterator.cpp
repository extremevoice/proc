#include <memory>

#include "gtest/gtest.h"
#include "../src/headers/lexer.h"

class IteratorTest : public testing::Test
{
public:
    std::unique_ptr<lexer::Iterator> Iterator;

    explicit IteratorTest()
        : Iterator(std::make_unique<lexer::Iterator>("@d17_")) {};
};

TEST_F(IteratorTest, IteratorHasNext) {
    ASSERT_TRUE(Iterator->HasNext());
}

TEST_F(IteratorTest, IteratorNext) {
    auto expected = '@';
    ASSERT_EQ(Iterator->Next(), expected);
}

TEST_F(IteratorTest, IteratorPeek) {
    Iterator->Next();
    auto expected = 'd';

    ASSERT_EQ(Iterator->Peek(), expected);
}
