#include "viewing_ray.h"

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ray.origin = camera.e;    // Origin of camera is origin of ray

  ray.direction[0] = (camera.width * (j + 0.5) / width) - (camera.width / 2.0);   // Transform j -> u
  ray.direction[1] = (camera.height / 2.0) - (camera.height * (i + 0.5) / height);    // Transform i -> v
  ray.direction[2] = -1 * camera.d;

  Eigen::Matrix3d cam_transform;    // Camera transform matrix
  cam_transform << camera.u, camera.v, camera.w;

  ray.direction = cam_transform * ray.direction;  // Ray direction

}

