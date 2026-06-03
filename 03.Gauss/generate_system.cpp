#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <Eigen/Dense>

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <size> <seed> <output.csv>\n";
        return 1;
    }

    const int n = std::stoi(argv[1]);
    const unsigned int seed = std::stoul(argv[2]);
    const char* output = argv[3];

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    Eigen::MatrixXd A(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A(i, j) = dist(rng);
        }
        A(i, i) += 10.0;
    }

    Eigen::VectorXd x(n);
    for (int i = 0; i < n; ++i) {
        x(i) = dist(rng);
    }

    Eigen::VectorXd b = A * x;

    std::ofstream out(output);
    out << std::fixed << std::setprecision(10);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << A(i, j) << ",";
        }
        out << b(i) << "\n";
    }

    std::string sol_file = std::string(output) + ".solution";
    std::ofstream sol_out(sol_file);
    sol_out << std::fixed << std::setprecision(10);
    for (int i = 0; i < n; ++i) {
        sol_out << x(i) << "\n";
    }

    std::cerr << "Generated system " << n << "x" << n << " with seed " << seed << "\n";
    return 0;
}