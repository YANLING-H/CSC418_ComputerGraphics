#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"

#include <limits>

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
  bool hit_found = false;
  hit_t = std::numeric_limits<double>::infinity();  // Initialize closest hit as infinity
  double t = 0.0; // Capture t value of hit
  for (int i=0; i < F.rows(); ++i) {
    if (ray_intersect_triangle(ray, V.row(F(i,0)), V.row(F(i,1)), V.row(F(i,2)), min_t, max_t, t) && t < hit_t) {
      // Found new closest hit
      hit_t = t;
      hit_f = i;
      hit_found = true;
    }
  }

  // Return whether any hit was found
  return hit_found;
}
