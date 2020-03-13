#include "kinematics_jacobian.h"
#include "transformed_tips.h"
#include <iostream>

void kinematics_jacobian(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  Eigen::MatrixXd & J)
{
  J.resize(b.size()*3,skeleton.size()*3);

  Skeleton copy = skeleton;
  Eigen::VectorXd tips = transformed_tips(skeleton, b);
  double h = 1e-7;

  for (int i=0; i<b.rows(); ++i) {
    auto xi = tips.segment(3*i, 3);
    Eigen::VectorXi b2(1);
    b2 << b[i];
    for (int j=0; j<skeleton.size(); ++j) {
      for (int theta_i=0; theta_i<3; ++theta_i) {
        copy[j].xzx[theta_i] += h;
        auto x_plus_del = transformed_tips(copy, b2);
        copy[j].xzx[theta_i] = skeleton[j].xzx[theta_i];

        J.block(3*i, 3*j + theta_i, 3, 1) << (x_plus_del - xi) / h;
      }
    }
  }
}
