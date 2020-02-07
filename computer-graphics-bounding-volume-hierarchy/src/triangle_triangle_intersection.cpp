#include "triangle_triangle_intersection.h"
#include "ray_intersect_triangle.h"

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{

  /*
   * Algorithm adapted from the explanation available here:
   * https://stackoverflow.com/a/7126909
   * 
   * As explained in the post, this does not work for intersecting
   * coplanar triangles
   */

  double min_t = 0.0, max_t = 1.0, t;

  Ray e1(A0, A1 - A0), e2(A0, A2 - A0), e3(A1, A2 - A1);

  // Does Triangle A intersect Triange B
  if (ray_intersect_triangle(e1, B0, B1, B2, min_t, max_t, t))
    return true;
  if (ray_intersect_triangle(e2, B0, B1, B2, min_t, max_t, t))
    return true;
  if (ray_intersect_triangle(e3, B0, B1, B2, min_t, max_t, t))
    return true;

  e1.origin = B0;
  e1.direction = B1 - B0;
  e2.origin = B0;
  e2.direction = B2 - B0;
  e3.origin = B1;
  e3.direction = B2 - B1;
  
  // Does Triangle B intersect Triange A
  if (ray_intersect_triangle(e1, A0, A1, A2, min_t, max_t, t))
    return true;
  if (ray_intersect_triangle(e2, A0, A1, A2, min_t, max_t, t))
    return true;
  if (ray_intersect_triangle(e3, A0, A1, A2, min_t, max_t, t))
    return true;

  return false; 
}
