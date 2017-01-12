#include <gtest/gtest.h>

#include <noise/module/ReseedableOps.h>
#include <noise/RasterImage.h>
#include <random>

using noise::module::ReseedablePtr;
class TestTileGenerationPlan : public ::testing::Test {
protected:
    ReseedablePtr x;
    ReseedablePtr y;
    ReseedablePtr c0;
    ReseedablePtr c1;
    TestTileGenerationPlan() :
        x(noise::module::makeX()),
        y(noise::module::makeY()),
        c0(noise::module::makeConst(0)),
        c1(noise::module::makeConst(1))
    {
    };
    ~TestTileGenerationPlan() {};
};

TEST_F(TestTileGenerationPlan, TestTileGenerationPlan)
{
    sf::Image output;
    output.create(256, 256);

    noise::RasterBase::Bounds xy(0, 0, 1, 1);
    noise::RasterBase::Bounds xy_(0, -1, 1, 1);
    noise::RasterBase::Bounds x_y(-1, 0, 1, 1);
    noise::RasterBase::Bounds x_y_(-1, -1, 1, 1);

    noise::RasterImage nmixy(output, xy);
    noise::RasterImage nmixy_(output, xy_);
    noise::RasterImage nmix_y(output, x_y);
    noise::RasterImage nmix_y_(output, x_y_);

    auto write_map = [&](const ReseedablePtr module,
                        std::string filename,
                        noise::RasterImage& nmi)
    {
        nmi.build(module->getModule());
        output.saveToFile("test/"+filename+".png");
    };

    write_map(x, "x", nmixy);
    write_map(x, "x_", nmix_y);
    write_map(y, "y", nmixy);
    write_map(y, "y_", nmixy_);

    ReseedablePtr msb_max_xy = noise::module::makeLinearMovingScaleBias(c1, true, true, 0.7, 0.25);
    ReseedablePtr msb_max_xy_ = noise::module::makeLinearMovingScaleBias(c1, true, false, 0.7, 0.25);
    ReseedablePtr msb_max_x_y = noise::module::makeLinearMovingScaleBias(c1, false, true, 0.7, 0.25);
    ReseedablePtr msb_max_x_y_ = noise::module::makeLinearMovingScaleBias(c1, false, false, 0.7, 0.25);
    ReseedablePtr msb_min_xy = noise::module::makeLinearMovingScaleBias(-c1, true, true, 0.7, 0.25);
    ReseedablePtr msb_min_xy_ = noise::module::makeLinearMovingScaleBias(-c1, true, false, 0.7, 0.25);
    ReseedablePtr msb_min_x_y = noise::module::makeLinearMovingScaleBias(-c1, false, true, 0.7, 0.25);
    ReseedablePtr msb_min_x_y_ = noise::module::makeLinearMovingScaleBias(-c1, false, false, 0.7, 0.25);

    write_map(msb_max_xy, "msb_max_xy", nmixy);
    write_map(msb_max_x_y, "msb_max_x_y", nmixy);
    write_map(msb_max_xy_, "msb_max_xy_", nmixy);
    write_map(msb_max_x_y_, "msb_max_x_y_", nmixy);
    write_map(msb_min_xy, "msb_min_xy", nmixy);
    write_map(msb_min_x_y, "msb_min_x_y", nmixy);
    write_map(msb_min_xy_, "msb_min_xy_", nmixy);
    write_map(msb_min_x_y_, "msb_min_x_y_", nmixy);

    ReseedablePtr cc = noise::module::makeCornerCombiner(true, true);
    write_map(cc, "ccxy", nmixy);
    write_map(cc, "ccxy_", nmixy_);
    write_map(cc, "ccx_y", nmix_y);
    write_map(cc, "ccx_y_", nmix_y_);

    std::mt19937 rng;
    rng.seed((unsigned int)time(nullptr));
    
    // this corner's weight in the tile centre
    ReseedablePtr stochastic = scaleBias(noise::module::makePlaceholder(rng()), 0.5, 0.5);
    // this corner's weight along its adjacent horizontal border
    ReseedablePtr border_x = noise::module::makePlaceholder(rng());
    // this corner's weight along its adjacent vertical border
    ReseedablePtr border_y = noise::module::makePlaceholder(rng());

    write_map(stochastic, "stochastic", nmixy);
    write_map(border_x, "borderx_y", nmixy);
    write_map(border_x, "borderx_y_", nmixy_);
    write_map(border_y, "bordery_x", nmixy);
    write_map(border_y, "bordery_x_", nmix_y);

    ReseedablePtr border_xy = blend(cc, border_y, border_x);
    write_map(border_xy, "border_xy", nmixy);

    ReseedablePtr deterministic = noise::module::makeLinearMovingScaleBias(border_xy, true, true, 0.7, 0.25);
    write_map(deterministic, "deterministic", nmixy);

    ReseedablePtr ef = noise::module::makeEdgeFavouringMask(1.5, 1.);
    write_map(ef, "ef", nmixy);

    ReseedablePtr corner = blend(ef, stochastic, deterministic);
    write_map(corner, "corner", nmixy);
}
