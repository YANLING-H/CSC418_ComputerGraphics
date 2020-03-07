#include "euler_angles_to_transform.h"

Eigen::Affine3d euler_angles_to_transform(
  const Eigen::Vector3d & xzx)
{
  Eigen::Affine3d R, Rx3, Rz, Rx1;
  double theta1 = xzx[0], theta2 = xzx[1], theta3 = xzx[2];

  Rx1.matrix() << 
    1,0,0,0,
    0,cos(theta1),-sin(theta1),0,
    0,sin(theta1),cos(theta1),0,
    0,0,0,1;

  Rz.matrix() << 
    cos(theta2),-sin(theta2),0,0,
    sin(theta2),cos(theta2),0,0,
    0,0,1,0,
    0,0,0,1;

  Rx3.matrix() << 
    1,0,0,0,
    0,cos(theta3),-sin(theta3),0,
    0,sin(theta3),cos(theta3),0,
    0,0,0,1;

  R = Rx3 * Rz * Rx1;
  return R;
}
