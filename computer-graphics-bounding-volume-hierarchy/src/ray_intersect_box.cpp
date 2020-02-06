#include "ray_intersect_box.h"
#include <iostream>
#include <limits>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{
  double tmin, tmax;
  double max = std::numeric_limits<double>::infinity(), min = -std::numeric_limits<double>::infinity();
  for (int i=0; i<3; ++i) {
    if (ray.direction[i] == 0) {
      tmin = -std::numeric_limits<double>::infinity();
      tmax = std::numeric_limits<double>::infinity();
    } else if (ray.direction[i] > 0) {
      tmin = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
      tmax = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
    } else {
      tmin = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
      tmax = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
    }
    if (tmin > max)
      max = tmin;
    if (tmax < min)
      min = tmax;
  }

  return max > min_t && min < max_t && max > min;
}
