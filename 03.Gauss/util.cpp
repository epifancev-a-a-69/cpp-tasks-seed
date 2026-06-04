#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"

GaussMatrix load_csv_to_matrix(const char *filename)
{
    if (filename == nullptr)
    {
        return GaussMatrix(0, 0);
    }

    std::ifstream in(filename);
    if (!in)
    {
        return GaussMatrix(0, 0);
    }

    std::vector<std::vector<double>> data;
    std::string line;

    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(std::stod(cell));
        }

        data.push_back(std::move(row));
    }

    if (data.empty())
    {
        return GaussMatrix(0, 0);
    }

    GaussMatrix result(static_cast<int>(data.size()),
                       static_cast<int>(data[0].size()));

    for (size_t i = 0; i < data.size(); ++i)
    {
        for (size_t j = 0; j < data[i].size(); ++j)
        {
            result(i, j) = data[i][j];
        }
    }

    return result;
}

void print_matrix_as_csv(std::ostream &out, const GaussMatrix &matrix, int prec)
{
    if (matrix.size() == 0)
    {
        return;
    }

    out << std::fixed << std::setprecision(10);

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            out << matrix(i, j);
            if (j < matrix.cols() - 1)
            {
                out << ',';
            }
        }
        out << '\n';
    }
}