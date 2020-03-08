#include "end_effectors_objective_and_gradient.h"
#include "transformed_tips.h"
#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include <iostream>
#include <cmath>

void end_effectors_objective_and_gradient(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  const Eigen::VectorXd & xb0,
  std::function<double(const Eigen::VectorXd &)> & f,
  std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  std::function<void(Eigen::VectorXd &)> & proj_z)
{
  f = [&](const Eigen::VectorXd & A)->double
  {
    double msd = 0.0;

    Skeleton transformed_skeleton = copy_skeleton_at(skeleton, A);
    Eigen::VectorXd tips = transformed_tips(transformed_skeleton, b);

    for (int i=0; i<b.rows(); ++i)
      msd += (tips.segment(3*i, 3) - xb0.segment(3*i, 3)).squaredNorm();

    return msd;
  };

  grad_f = [&](const Eigen::VectorXd & A)->Eigen::VectorXd
  {
    Skeleton transformed_skeleton = copy_skeleton_at(skeleton, A);
    Eigen::VectorXd tips = transformed_tips(transformed_skeleton, b);

    Eigen::VectorXd dE_dx(3 * b.rows());
    for (int i=0; i<dE_dx.rows(); ++i)
      dE_dx[i] = 2 * (tips[i] - xb0[i]);

    Eigen::MatrixXd J;
    kinematics_jacobian(transformed_skeleton, b, J);

    return J.transpose() * dE_dx;
  };

  proj_z = [&](Eigen::VectorXd & A)
  {
    assert(skeleton.size()*3 == A.size());
    for (int i=0; i<skeleton.size(); ++i) {
      for (int j=0; j<3; ++j) {
        A[3*i + j] = std::max(skeleton[i].xzx_min[j], std::min(skeleton[i].xzx_max[j], A[3*i + j]));
      }
    }
  };
}
