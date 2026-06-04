#ifndef UTIL_H
#define UTIL_H

#include <Eigen/Dense>

typedef Eigen::VectorXd GaussVector;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
GaussMatrix;

GaussMatrix load_csv_to_matrix(const char *filename);
void print_matrix_as_csv(std::ostream &out, const GaussMatrix &matrix,
                         int prec = 6);

#endif // UTIL_H