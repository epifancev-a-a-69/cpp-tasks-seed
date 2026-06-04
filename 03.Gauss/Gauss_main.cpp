#include <Eigen/Dense>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Gauss_solve.h"
#include "util.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <input_csv_file> [output_csv_file]\n";
        return 1;
    }

    GaussMatrix augmented = load_csv_to_matrix(argv[1]);
    if (augmented.rows() == 0)
    {
        std::cerr << "Error: Failed to load matrix from " << argv[1] << '\n';
        return 1;
    }

    int n = augmented.rows();
    int m = augmented.cols();

    std::cout << "Loaded matrix " << n << "x" << m << " from " << argv[1]
              << "\n\n";

    if (m != n + 1)
    {
        std::cerr << "Error: Expected " << n + 1 << " columns, but found " << m
                  << "\n";
        return 1;
    }

    GaussVector solution = Gauss_solve(augmented);

    if (solution.size() == 0)
    {
        std::cerr << "Error: Singular matrix or solving failed\n";
        return 1;
    }

    std::cout << "Solution:\n";
    for (int i = 0; i < solution.size(); ++i)
    {
        std::cout << "x[" << i << "] = " << solution(i) << '\n';
    }

    if (argc >= 3)
    {
        std::ofstream out(argv[2]);
        if (out.is_open())
        {
            out << std::fixed << std::setprecision(10);
            for (int i = 0; i < solution.size(); ++i)
            {
                out << solution(i);
                if (i + 1 < solution.size())
                    out << '\n';
            }
            std::cout << "\nSolution saved to " << argv[2] << '\n';
        }
    }

    return 0;
}