#include "per_face_normals.h"
#include "triangle_area_normal.h"
#include <Eigen/Geometry>
#include <iostream>

void per_face_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N.resize(F.rows(), 3);
  for (int i=0; i<F.rows(); ++i) {
    Eigen::RowVector3d t1 = V.row(F(i, 1)) - V.row(F(i, 0));
    Eigen::RowVector3d t2 = V.row(F(i, 2)) - V.row(F(i, 0));
    N.row(i) << t1.cross(t2).normalized();
  }
}
