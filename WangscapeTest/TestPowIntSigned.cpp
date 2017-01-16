#include <gtest/gtest.h>

#include <type_traits>

#include <utils.h>

template<typename I>
class TestPowIntSigned : public ::testing::Test
{
protected:
    const static I min = std::numeric_limits<I>::min();
    const static I minus_five = -5;
    const static I minus_three = -3;
    const static I minus_two = -2;
    const static I minus_one = -1;
    const static I zero = 0;
    const static I one = 1;
    const static I two = 2;
    const static I three = 3;
    const static I five = 5;
    const static I digits = std::numeric_limits<I>::digits;
    const static I max = std::numeric_limits<I>::max();
    TestPowIntSigned()
    {

    };
    ~TestPowIntSigned() {};
};

typedef ::testing::Types<
    char, short, int, long, long long>
    SignedIntTypes;
TYPED_TEST_CASE(TestPowIntSigned, SignedIntTypes);

TYPED_TEST(TestPowIntSigned, TestPowIntSignedX0)
{
    EXPECT_EQ(this->one, pow(this->minus_one, this->zero));
    EXPECT_EQ(this->one, pow(this->minus_two, this->zero));
    EXPECT_EQ(this->one, pow(this->minus_three, this->zero));
    EXPECT_EQ(this->one, pow(this->minus_five, this->zero));
    EXPECT_EQ(this->one, pow(this->min, this->zero));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSigned0X)
{
    EXPECT_ANY_THROW(pow(this->zero, this->minus_one));
    EXPECT_ANY_THROW(pow(this->zero, this->minus_two));
    EXPECT_ANY_THROW(pow(this->zero, this->minus_three));
    EXPECT_ANY_THROW(pow(this->zero, this->minus_five));
    EXPECT_ANY_THROW(pow(this->zero, this->min));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSignedX1)
{
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->one));
    EXPECT_EQ(this->minus_two, pow(this->minus_two, this->one));
    EXPECT_EQ(this->minus_three, pow(this->minus_three, this->one));
    EXPECT_EQ(this->minus_five, pow(this->minus_five, this->one));
    EXPECT_EQ(this->min, pow(this->min, this->one));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSigned1X)
{
    EXPECT_EQ(this->one, pow(this->one, this->minus_one));
    EXPECT_EQ(this->one, pow(this->one, this->minus_two));
    EXPECT_EQ(this->one, pow(this->one, this->minus_three));
    EXPECT_EQ(this->one, pow(this->one, this->minus_five));
    EXPECT_EQ(this->one, pow(this->one, this->min));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSignedMinus1X)
{
    EXPECT_EQ(this->one, pow(this->minus_one, this->min));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->minus_five));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->minus_three));
    EXPECT_EQ(this->one, pow(this->minus_one, this->minus_two));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->minus_one));
    EXPECT_EQ(this->one, pow(this->minus_one, this->zero));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->one));
    EXPECT_EQ(this->one, pow(this->minus_one, this->two));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->three));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->five));
    EXPECT_EQ(this->minus_one, pow(this->minus_one, this->max));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSignedExamples)
{
    EXPECT_EQ(4, pow(this->minus_two, this->two));
    EXPECT_EQ(9, pow(this->minus_three, this->two));
    EXPECT_EQ(25, pow(this->minus_five, this->two));

    EXPECT_EQ(-8, pow(this->minus_two, this->three));
    EXPECT_EQ(-27, pow(this->minus_three, this->three));
    EXPECT_EQ(-125, pow(this->minus_five, this->three));

    EXPECT_EQ(-32, pow(this->minus_two, this->five));
}

TYPED_TEST(TestPowIntSigned, TestPowIntSignedMin)
{
    EXPECT_EQ(this->min, pow(this->minus_two, this->digits));
}
