#include "linear_blend_skinning.h"
#include <Eigen/Geometry>

void linear_blend_skinning(
  const Eigen::MatrixXd & V,
  const Skeleton & skeleton,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T,
  const Eigen::MatrixXd & W,
  Eigen::MatrixXd & U)
{
  U.resize(V.rows(), V.cols());
  for (int v=0; v<V.rows(); ++v) {
    Eigen::Vector3d v_rest = V.row(v).transpose();
    Eigen::Vector3d vi = Eigen::Vector3d::Zero();
    for (int i=0; i<skeleton.size(); ++i) {
      if (skeleton[i].weight_index == -1)
        continue;
      vi += W(v, skeleton[i].weight_index) * (T[i].linear() * v_rest + T[i].translation());
    }
//    if (transform.matrix().isZero())
//      transform = Eigen::Affine3d::Identity();
//    U.row(v) << (transform * V.row(v).transpose().homogeneous()).transpose();
    U.row(v) << vi.transpose();
  }
}
