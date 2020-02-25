#include "per_vertex_normals.h"
#include "triangle_area_normal.h"
#include "vertex_triangle_adjacency.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  std::vector< std::vector<int> > VF;
  vertex_triangle_adjacency(F, V.rows(), VF);

  N.resize(V.rows(), 3);
  for (int i=0; i<VF.size(); ++i) {
    auto n = Eigen::RowVector3d(0, 0, 0);
    for (int j=0; j<VF[i].size(); ++j) {
      int f = VF[i][j];
      n += triangle_area_normal(V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)));
    }
    N.row(i) << n.normalized();
  }
}
