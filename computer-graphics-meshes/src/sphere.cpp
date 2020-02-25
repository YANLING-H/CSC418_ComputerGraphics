#include "sphere.h"
#include <iostream>
#include <math.h>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  // Resize Matrices
  int num_vertices = num_faces_u * (num_faces_v - 1) + 2;
  int num_faces = num_faces_u * num_faces_v;
  V.resize(num_vertices, 3);
  F.resize(num_faces, 4);
  UV.resize(num_vertices + num_faces_v - 1, 2);
  UF.resize(num_faces, 4);
  NV.resize(num_vertices, 3);
  NF.resize(num_faces, 4);

  double radius = 1.0;
  double u_step = 1.0 / num_faces_u;
  double v_step = 1.0 / num_faces_v;
  double lat_step = M_PI * v_step;
  double long_step = 2.0 * M_PI * u_step;

  int v_row = 0, uv_row = 0;
  V.row(v_row++) << 0, 0, -radius;
  UV.row(uv_row++) << 0.5, 0;

  for (int v = 1; v<num_faces_v; ++v) {
    double latitude = v * lat_step;
    
    // v coordinate
    double uv_v = v * v_step;
    for (int u = 0; u<num_faces_u; ++u) {
      double longitude = u * long_step;

      // Spherical to x, y, z
      double x = radius * sin(latitude) * cos(longitude);
      double y = radius * sin(latitude) * sin(longitude);
      double z = -radius * cos(latitude);   // Consider the angle 180-phi to sweep sphere from bottom to top

      // u coordinate
      double uv_u = u * u_step;

      // Add Vertices
      V.row(v_row++) << x, y, z;
      UV.row(uv_row++) << uv_u, uv_v;
    }
    UV.row(uv_row++) << 1.0, uv_v;
  }

  V.row(v_row) << 0, 0, radius;
  UV.row(uv_row) << 0.5, 1.0;

  // Add Faces
  int f_row = 0;
  for (int v = 0; v<num_faces_v; ++v) {
    for (int u = 0; u<num_faces_u; ++u) {
      int ref_point_prev = (v-1) * num_faces_u + u + 1;
      int ref_point = v * num_faces_u + u + 1;

      int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
      bool top = v == num_faces_v - 1, bottom = v == 0, 
            right = u == num_faces_u - 1;

      if (bottom) {
        v1 = 1;
        v4 = 1;
      } else {
        v1 = ref_point_prev + 1;
        v4 = ref_point_prev + 2;
        if (right)
          v4 -= u;
      }

      if (top) {
        v2 = v_row;
        v3 = v_row;
      } else {
        v2 = ref_point + 1;
        v3 = ref_point + 2;
        if (right)
          v3 -= u;
      }

      F.row(f_row) << v1, v2, v3, v4;

      ref_point_prev = (v-1) * (num_faces_u + 1) + u + 1;
      ref_point = v * (num_faces_u + 1) + u + 1;
      int uv1 = 0, uv2 = 0, uv3 = 0, uv4 = 0;
      if (bottom) {
        uv1 = 1;
        uv4 = 1;
      } else {
        uv1 = ref_point_prev + 1;
        uv4 = ref_point_prev + 2;
      }

      if (top) {
        uv2 = uv_row;
        uv3 = uv_row;
      } else {
        uv2 = ref_point + 1;
        uv3 = ref_point + 2;
      }

      UF.row(f_row++) << uv1, uv2, uv3, uv4;
    }
  }

  // Sphere is at origin, so vertex locations are their normals
  NV = V.normalized();
  NF = F;
  
}
