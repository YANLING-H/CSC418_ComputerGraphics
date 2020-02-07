#include "point_box_squared_distance.h"

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
  double min_dist = 0.0;
  double d;
  for (int i=0; i<3; ++i) {
    if (query[i] >= box.min_corner[i] && query[i] <= box.max_corner[i])
      d = 0.0;  // Don't need to move along this axis as it is between the boundaries of the box
    else if (query[i] > box.max_corner[i])
      d = query[i] - box.max_corner[i]; // Max corner is the closer corner
    else
      d = box.min_corner[i] - query[i]; // Min corner is the closer corner
    min_dist += d * d;  // Add squared distance along this axis
  }
  return min_dist;
}
