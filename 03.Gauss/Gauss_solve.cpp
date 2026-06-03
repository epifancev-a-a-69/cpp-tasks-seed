#include "Gauss_solve.h"
#include <cmath>

GaussVector Gauss_solve(GaussMatrix& ab)
{
    const int n = ab.rows();
    const int m = ab.cols();

    if (n == 0 || m != n + 1) {
        return GaussVector(0);
    }

    GaussVector x(n);

    // Прямой ход с выбором главного элемента
    for (int i = 0; i < n; ++i)
    {
        // Поиск главного элемента
        int max_row = i;
        double max_val = std::fabs(ab(i, i));
        for (int k = i + 1; k < n; ++k)
        {
            double val = std::fabs(ab(k, i));
            if (val > max_val) {
                max_val = val;
                max_row = k;
            }
        }

        if (max_val < 1e-12) {
            return GaussVector(0);
        }

        if (max_row != i) {
            ab.row(i).swap(ab.row(max_row));
        }

        // Нормализация строки
        double pivot = ab(i, i);
        for (int j = i; j <= n; ++j) {
            ab(i, j) /= pivot;
        }

        // Исключение в остальных строках
        for (int k = 0; k < n; ++k)
        {
            if (k != i && std::fabs(ab(k, i)) > 1e-15)
            {
                double factor = ab(k, i);
                for (int j = i; j <= n; ++j) {
                    ab(k, j) -= factor * ab(i, j);
                }
            }
        }
    }

    // Извлечение решения
    for (int i = 0; i < n; ++i) {
        x(i) = ab(i, n);
    }

    return x;
}