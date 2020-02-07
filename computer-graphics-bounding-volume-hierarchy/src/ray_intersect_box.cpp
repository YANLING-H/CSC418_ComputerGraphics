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
  double interval_start = -std::numeric_limits<double>::infinity(), interval_end = std::numeric_limits<double>::infinity();
  for (int i=0; i<3; ++i) {
    if (ray.direction[i] == 0) {
      // Edge case if ray does not move along this axis.
      // If the origin is within the bounds of the box,
      // then it will hit. Otherwise, it will not.
      tmin = -std::numeric_limits<double>::infinity();
      tmax = std::numeric_limits<double>::infinity();
      if (ray.origin[i] < box.min_corner[i] || ray.origin[i] > box.max_corner[i])
        return false;
    } else if (ray.direction[i] > 0) {
      tmin = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
      tmax = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
    } else {
      tmin = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
      tmax = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
    }
    // Update interval start and end points based on tmin and tmax along this axis
    if (tmin > interval_start)
      interval_start = tmin;
    if (tmax < interval_end)
      interval_end = tmax;
  }

  // Return if interval is valid
  return interval_end > min_t && interval_start < max_t && interval_end > interval_start;
}
