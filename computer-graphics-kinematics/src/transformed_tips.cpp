#include "transformed_tips.h"
#include "forward_kinematics.h"

Eigen::VectorXd transformed_tips(
  const Skeleton & skeleton, 
  const Eigen::VectorXi & b)
{
  Eigen::VectorXd a = Eigen::VectorXd::Zero(3*b.size());
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > T;
  T.resize(skeleton.size());

  forward_kinematics(skeleton, T);
  
  for (int i=0; i<b.size(); ++i) {
    auto d = T[b[i]] * Eigen::Vector4d(skeleton[i].length, 0.0, 0.0, 1.0);
    a[3*i] = d[0];
    a[3*i+1] = d[1];
    a[3*i+2] = d[2];
  }

  return a;
}
