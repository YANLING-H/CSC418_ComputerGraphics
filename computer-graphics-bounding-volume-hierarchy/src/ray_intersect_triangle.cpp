#include "ray_intersect_triangle.h"
#include <Eigen/Dense>

bool ray_intersect_triangle(
  const Ray & ray,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  const double min_t,
  const double max_t,
  double & t)
{
  // Transpose vectors to transform problem to
  // the same one as solved for ray-triangle intersections
  // in previous labs
  Eigen::Vector3d c = A.transpose();       // Take first point as join point of triangle
  // The two arms of the triangle
  Eigen::Vector3d t1 = B.transpose() - c; 
  Eigen::Vector3d t2 = C.transpose() - c;

  Eigen::Matrix3d m;
  m << t1, t2, -1 * ray.direction;

  if (m.determinant() == 0)
    return false;   // The matrix m is not linearly dependant, so no hit possible

  Eigen::Vector3d values = m.inverse() * (ray.origin - c);    // Values of alpha, beta and t

  if (values[0] < 0 || values[1] < 0 || 
    values[0] + values[1] > 1 || values[2] < min_t || values[2] > max_t)
      return false;   // Values of alpha, beta and/or t do not meet constraints

  // Triangle hit is valid
  t = values[2];
  return true;
}

