#include "catmull_clark.h"
#include "vertex_triangle_adjacency.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <unordered_set>

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  if (num_iters <= 0)
    return;

  // Necessary data structures
  Eigen::MatrixXi E;    // All edges, identified by indexing vertices
  Eigen::MatrixXi EF;   // Face to edge index
  Eigen::MatrixXi FE;   // Edge to face index
  std::vector<std::vector<int> > VF;    // Vector to face adjacency list
  std::vector<std::unordered_set<int> > VV;    // Vector to vector adjacency list

  std::vector<Eigen::RowVector3d> face_points;
  std::vector<Eigen::RowVector3d> edge_points;

  // Initialize data structures
  E.resize(V.rows() + F.rows() - 2, 2);   // Euler's Formula
  EF.resize(F.rows(), 4);
  FE.resize(E.rows(), 2);

  vertex_triangle_adjacency(F, V.rows(), VF);
  VV.resize(V.rows());

  // Resize output vectors
  SV.resize(F.rows() + E.rows()/2 + V.rows(), 3);
  SF.resize(4 * F.rows(), 4);
  int sv_row = 0, sf_row = 0;

  int e_row = 0;
  for (int i=0; i<F.rows(); ++i) {
    int ef_col = 0;
    for (int j=0; j<F.cols(); ++j) {
      int v0 = j == 0 ? F(i, F.cols()-1) : F(i, j-1);
      int v1 = F(i, j);
      int v2 = F(i, (j+1) % F.cols());
      auto forward_edge = Eigen::RowVector2i(v1, v2);
      auto backward_edge = Eigen::RowVector2i(v2, v1);

      VV[v1].insert(v0);
      VV[v1].insert(v2);

      bool exists = false;
      for (int k=0; k<e_row; ++k) {
        if (E.row(k).isApprox(forward_edge) || E.row(k).isApprox(backward_edge)) {
          exists = true;
          FE(k, 1) = i;
          EF(i, ef_col++) = k;
          break;
        }
      }

      if (!exists) {
        E.row(e_row) << forward_edge;
        EF(i, ef_col++) << e_row;
        FE(e_row++, 0) = i;
      }
    }
  }

  // For each face, add a face point
  for (int i=0; i<F.rows(); ++i) {
    Eigen::RowVector3d avg = Eigen::RowVector3d(0.0, 0.0, 0.0);
    for (int j=0; j<F.cols(); ++j)
      avg += V.row(F(i, j));
    avg /= F.cols();
    face_points.push_back(avg);
    SV.row(sv_row++) << avg;
  }

  // For each edge, add an edge point
  for (int i=0; i<E.rows(); i += 2) {
    Eigen::RowVector3d avg = Eigen::RowVector3d(0.0, 0.0, 0.0);
    avg += V.row(E(i, 0)) + V.row(E(i, 1));
    avg += face_points[FE(i, 0)] + face_points[FE(i, 1)];
    avg /= 4.0;
    edge_points.push_back(avg);
    SV.row(sv_row++) << avg;
  }

  // For each original point P, take the average F 
  // of all n (recently created) face points for faces 
  // touching P, and take the average R of all n edge 
  // midpoints for (original) edges touching P, where 
  // each edge midpoint is the average of its two endpoint 
  // vertices (not to be confused with new "edge points" above). 
  // Move each original point to the barycenter
  for (int i=0; i<V.rows(); ++i) {
    auto adj_faces = VF[i];
    auto f = Eigen::RowVector3d(0.0, 0.0, 0.0);
    auto r = Eigen::RowVector3d(0.0, 0.0, 0.0);
    auto p = V.row(i);
    int n = adj_faces.size();
    for (int face:adj_faces)
      f += face_points[face];
    f /= n;

    for (int v:VV[i]) {
      auto midpoint = (V.row(v) + p) / 2.0;
      r += midpoint;
    }
    r /= VV[i].size();

    // Barycenter
    auto barycenter = (f + 2.0 * r + (n - 3.0) * p) / (double) n;
    SV.row(sv_row++) << barycenter;
  }

  // Define new faces as enclosed by edges
  for (int i=0; i<F.rows(); ++i) {
    int v1, v2, v3, v4;
    v1 = i;   // Face point for face
    auto edges = EF.row(i);       // Edge points for face
    for (int j=0; j<F.cols(); ++j) {
      v2 = F.rows() + j == 0 ? edges[edges.cols()-1] : edges[j-1];
      v3 = F.rows() + E.rows() + F(i, j);
      v4 = F.rows() + edges[j];
      SF.row(sf_row++) << v1, v2, v3, v4;
    }
  }

  // Recursive call
  catmull_clark(SV, F, num_iters - 1, SV, SF);
}
