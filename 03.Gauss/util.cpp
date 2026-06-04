#include <string>
#include <iomanip>
#include <vector>
#include <lazycsv.hpp>

#include "util.h"

GaussMatrix load_csv_to_matrix(const char *filename)
{
    if (filename == nullptr) {
        return GaussMatrix(0, 0);
    }

    std::vector<std::vector<double>> rcsv;
    {
        lazycsv::parser parser{filename};
        for (const auto& row : parser)
        {
            std::vector<double> r;
            r.reserve(row.size());
            for (const auto& cell : row)
            {
                r.push_back(std::stod(std::string(cell.raw())));
            }
            rcsv.push_back(std::move(r));
        }
    }

    if (rcsv.empty() || rcsv[0].empty()) {
        return GaussMatrix(0, 0);
    }

    GaussMatrix result(static_cast<int>(rcsv.size()), static_cast<int>(rcsv[0].size()));
    for (size_t i = 0; i < rcsv.size(); ++i)
    {
        for (size_t j = 0; j < rcsv[i].size(); ++j)
        {
            result(static_cast<int>(i), static_cast<int>(j)) = rcsv[i][j];
        }
    }

    return result;
}

void print_matrix_as_csv(std::ostream& out, const GaussMatrix& matrix, int prec)
{
    if (matrix.size() == 0) {
        return;
    }

    for (int j = 0; j < matrix.cols() - 1; ++j) {
        out << "A,";
    }
    out << "B\n";

    out << std::fixed << std::setprecision(prec);

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            out << matrix(i, j);
            if (j < matrix.cols() - 1) {
                out << ',';
            }
        }
        out << '\n';
    }
}