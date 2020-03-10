#include "projected_gradient_descent.h"
#include "line_search.h"

void projected_gradient_descent(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const int max_iters,
  Eigen::VectorXd & z)
{
  double max_step = 10000.0;
  for (int iters=0; iters<max_iters; ++iters) {
    Eigen::VectorXd dz = grad_f(z);
    if (f(z) <= 1e-5 || dz.isZero())
      return;
    max_step = line_search(f, proj_z, z, dz, max_step);
    z = z - max_step * dz;
    proj_z(z);
  }
}
