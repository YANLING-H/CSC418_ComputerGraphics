#include "forward_kinematics.h"
#include "euler_angles_to_transform.h"
#include <functional> // std::function

Eigen::Affine3d bone_transformation(
  const Skeleton & skeleton,
  const int index,
  std::vector<bool> & T_assigned,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T);

void forward_kinematics(
  const Skeleton & skeleton,
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
  std::vector<bool> T_assigned = std::vector<bool>(T.size(), false);
  for (int i=0; i<T.size(); ++i) {
    T[i] = bone_transformation(skeleton, i, T_assigned, T);
  }
}

Eigen::Affine3d bone_transformation(
  const Skeleton & skeleton,
  const int index,
  std::vector<bool> & T_assigned,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
  if (index < 0 || index >= skeleton.size())
    return Eigen::Affine3d::Identity();
  if (T_assigned[index])
    return T[index];

  T_assigned[index] = true;
  const Bone &bone = skeleton[index];
  
  return bone_transformation(skeleton, bone.parent_index, T_assigned, T)
          * bone.rest_T * euler_angles_to_transform(bone.xzx) * bone.rest_T.inverse();
}