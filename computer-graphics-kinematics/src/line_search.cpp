#include "line_search.h"
#include <iostream>

double line_search(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const Eigen::VectorXd & z,
  const Eigen::VectorXd & dz,
  const double max_step)
{
  double sigma = max_step * 2;
  double E0 = f(z);
  double E1;

  do {
    sigma /= 2;
    Eigen::VectorXd new_z = z - sigma * dz;
    proj_z(new_z);
    E1 = f(new_z);
  } while (E1 > E0);

  return sigma;
}
