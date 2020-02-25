#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include "per_face_normals.h"
#include <iostream>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  std::vector< std::vector<int> > VF;
  Eigen::MatrixXd FN;
  vertex_triangle_adjacency(F, V.rows(), VF);
  per_face_normals(V, F, FN);

  double eps = cos(corner_threshold * M_PI / 180.0);
  N.resize(F.rows()*3, 3);

  for (int i=0; i<F.rows(); ++i) {
    auto face_normal = FN.row(i);
    for (int j=0; j<F.cols(); ++j) {
      int vertex = F(i, j);
      auto adjacent_faces = VF[vertex];
      auto n = Eigen::RowVector3d(0, 0, 0);
      for (int f : adjacent_faces) {
        if (FN.row(f).dot(face_normal) > eps) {
          n += triangle_area_normal(V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)));
        }
      }
      N.row(3*i + j) << n.normalized();
    }
  }
}
