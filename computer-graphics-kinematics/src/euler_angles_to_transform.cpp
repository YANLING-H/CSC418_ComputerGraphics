#include "euler_angles_to_transform.h"

Eigen::Affine3d euler_angles_to_transform(
  const Eigen::Vector3d & xzx)
{

  Eigen::AngleAxis<double> Rx3, Rz, Rx1;
  double theta1 = xzx[0] * M_PI/180.0, 
         theta2 = xzx[1] * M_PI/180.0, 
         theta3 = xzx[2] * M_PI/180.0;

  Rx1 = Eigen::AngleAxis<double>(theta1, Eigen::Vector3d::UnitX());
  Rz  = Eigen::AngleAxis<double>(theta2, Eigen::Vector3d::UnitZ());
  Rx3 = Eigen::AngleAxis<double>(theta3, Eigen::Vector3d::UnitX());

  return Eigen::Affine3d(Rx3 * Rz * Rx1);
}
