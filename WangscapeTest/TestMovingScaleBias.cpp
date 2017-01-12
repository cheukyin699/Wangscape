#include <gtest/gtest.h>

#include <noise/module/ReseedableOps.h>

class TestMovingScaleBias : public ::testing::Test {
protected:
    noise::module::ReseedablePtr x;
    noise::module::ReseedablePtr y;
    noise::module::ReseedablePtr z;
    noise::module::ReseedablePtr msb;
    noise::module::ReseedablePtr lmsb1;
    noise::module::ReseedablePtr lmsb2;

    TestMovingScaleBias() :
        x(noise::module::makeX()),
        y(noise::module::makeY()),
        z(noise::module::makeZ()),
        msb(noise::module::makeMovingScaleBias(x, (y * 2) + 1, (y * 3) + 2)),
        // these need updating for 2D lmsbs!
        lmsb1(noise::module::makeLinearMovingScaleBias(z, true, true)),
        lmsb2(noise::module::makeLinearMovingScaleBias(z, false, false, 0.5, 0.25))
    {

    };
    ~TestMovingScaleBias() {};
};

TEST_F(TestMovingScaleBias, TestMovingScaleBias)
{
    EXPECT_NEAR(1.5, msb->getModule().GetValue(0., 0., 3453.), 0.0001);
    EXPECT_NEAR(3., msb->getModule().GetValue(-1., 1., 3453.), 0.0001);
    EXPECT_NEAR(-1., msb->getModule().GetValue(345304895., -1., 3453.), 0.0001);
    EXPECT_NEAR(8., msb->getModule().GetValue(1., 2., 3453.), 0.0001);
}

TEST_F(TestMovingScaleBias, TestLinearMovingScaleBiasDefault)
{
    EXPECT_NEAR(1., lmsb1->getModule().GetValue(0., 0., 1.), 0.0001);
    EXPECT_NEAR(1., lmsb1->getModule().GetValue(0., 0., -1.), 0.0001);

    EXPECT_NEAR(1., lmsb1->getModule().GetValue(0.5, 0., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(-0.5, 0., -1.), 0.0001);
    EXPECT_NEAR(1., lmsb1->getModule().GetValue(0., 0.5, 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(0., -0.5, -1.), 0.0001);
    EXPECT_NEAR(1., lmsb1->getModule().GetValue(0.25, -0.25, 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(-0.25, 0.25, -1.), 0.0001);

    EXPECT_NEAR(0., lmsb1->getModule().GetValue(-0.5, -0.5, 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(0.5, 0.5, -1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(1., 0., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(-1., 0., -1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(1., -1., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb1->getModule().GetValue(-1., 1., -1.), 0.0001);
}

TEST_F(TestMovingScaleBias, TestLinearMovingScaleBiasShort)
{
    EXPECT_NEAR(1., lmsb2->getModule().GetValue(1., 1., 1.), 0.0001);
    EXPECT_NEAR(1., lmsb2->getModule().GetValue(1., 1., -1.), 0.0001);

    EXPECT_NEAR(1., lmsb2->getModule().GetValue(0.875, 1., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(1., 0.875, -1.), 0.0001);
    EXPECT_NEAR(1., lmsb2->getModule().GetValue(1., 0.75, 1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(0.75, 1., -1.), 0.0001);
    EXPECT_NEAR(1., lmsb2->getModule().GetValue(0.625, 1., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(1., 0.625, -1.), 0.0001);

    EXPECT_NEAR(0., lmsb2->getModule().GetValue(1., 0.5, 1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(0.5, 1., -1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(0., 1., 1.), 0.0001);
    EXPECT_NEAR(0., lmsb2->getModule().GetValue(1., 0., -1.), 0.0001);
}
