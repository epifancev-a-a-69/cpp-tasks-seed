#include <iostream>
#include <fstream>
#include <cstdlib>

#include <Eigen/Dense>
#include <lazycsv.hpp>

#include "util.h"
#include "Gauss_solve.h"

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_csv_file> [output_csv_file]\n";
        return 1;
    }

    // Загрузка расширенной матрицы [A|b]
    GaussMatrix augmented = load_csv_to_matrix(argv[1]);
    if (augmented.rows() == 0) {
        std::cerr << "Error: Failed to load matrix from " << argv[1] << '\n';
        return 1;
    }

    int n = augmented.rows();
    int m = augmented.cols();

    std::cout << "Loaded matrix " << n << "x" << m << " from " << argv[1] << "\n\n";

    if (m != n + 1) {
        std::cerr << "Error: Augmented matrix must have " << n + 1 
                  << " columns (found " << m << ")\n";
        return 1;
    }

    // Решение системы
    GaussVector solution = Gauss_solve(augmented);

    if (solution.size() == 0) {
        std::cerr << "Error: Singular matrix or solving failed\n";
        return 1;
    }

    // Вывод решения
    std::cout << "Solution vector x:\n";
    for (int i = 0; i < solution.size(); ++i) {
        std::cout << "x[" << i << "] = " << solution(i) << '\n';
    }

    // Сохранение в файл
    if (argc >= 3) {
        std::ofstream out(argv[2]);
        if (out.is_open()) {
            GaussMatrix result_matrix(solution.size(), 1);
            for (int i = 0; i < solution.size(); ++i) {
                result_matrix(i, 0) = solution(i);
            }
            print_matrix_as_csv(out, result_matrix);
            std::cout << "\nSolution saved to " << argv[2] << '\n';
        } else {
            std::cerr << "Warning: Could not open " << argv[2] << " for writing\n";
        }
    }

    return 0;
}