#include <gtest/gtest.h>
#include <random>
#include "Gauss_solve.h"

double compute_residual(const GaussMatrix& ab, const GaussVector& x)
{
    int n = ab.rows();
    double max_resid = 0.0;
    
    for (int i = 0; i < n; ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < n; ++j) {
            sum += ab(i, j) * x(j);
        }
        double resid = std::fabs(sum - ab(i, n));
        if (resid > max_resid) max_resid = resid;
    }
    return max_resid;
}

TEST(GaussSolve, Small2x2)
{
    GaussMatrix ab(2, 3);
    ab << 2.0, 1.0, 5.0,
           1.0, 1.0, 3.0;

    GaussVector sol = Gauss_solve(ab);
    
    ASSERT_EQ(sol.size(), 2);
    EXPECT_NEAR(sol(0), 2.0, 1e-9);
    EXPECT_NEAR(sol(1), 1.0, 1e-9);
    EXPECT_LT(compute_residual(ab, sol), 1e-9);
}

TEST(GaussSolve, Small3x3)
{
    GaussMatrix ab(3, 4);
    ab << 1.0, 2.0, 3.0, 14.0,
           2.0, 3.0, 4.0, 20.0,
           3.0, 4.0, 5.0, 26.0;

    GaussVector sol = Gauss_solve(ab);
    
    ASSERT_EQ(sol.size(), 3);
    EXPECT_NEAR(sol(0), 1.0, 1e-9);
    EXPECT_NEAR(sol(1), 2.0, 1e-9);
    EXPECT_NEAR(sol(2), 3.0, 1e-9);
    EXPECT_LT(compute_residual(ab, sol), 1e-9);
}

TEST(GaussSolve, SingularMatrix)
{
    GaussMatrix ab(2, 3);
    ab << 1.0, 2.0, 3.0,
           2.0, 4.0, 6.0;

    GaussVector sol = Gauss_solve(ab);
    EXPECT_EQ(sol.size(), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 