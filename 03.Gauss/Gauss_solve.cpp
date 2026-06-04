#include "Gauss_solve.h"
#include <cmath>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    const int n = ab.rows();

    if (n == 0 || ab.cols() != n + 1)
    {
        return GaussVector(0);
    }

    GaussMatrix A = ab;

    for (int i = 0; i < n; ++i)
    {
        int pivot_row = i;

        for (int r = i + 1; r < n; ++r)
        {
            if (std::fabs(A(r, i)) > std::fabs(A(pivot_row, i)))
            {
                pivot_row = r;
            }
        }

        if (std::fabs(A(pivot_row, i)) < 1e-12)
        {
            return GaussVector(0);
        }

        if (pivot_row != i)
        {
            A.row(i).swap(A.row(pivot_row));
        }

        for (int r = i + 1; r < n; ++r)
        {
            double factor = A(r, i) / A(i, i);

            for (int c = i; c <= n; ++c)
            {
                A(r, c) -= factor * A(i, c);
            }
        }
    }

    GaussVector x(n);

    for (int i = n - 1; i >= 0; --i)
    {
        double sum = A(i, n);

        for (int j = i + 1; j < n; ++j)
        {
            sum -= A(i, j) * x(j);
        }

        x(i) = sum / A(i, i);
    }

    return x;
}