#include "viewing_ray.h"

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ray.origin = camera.e;
  ray.direction[0] = (camera.width * (j + 0.5) / width) - (camera.width / 2.0);
  ray.direction[1] = (camera.height / 2.0) - (camera.height * (i + 0.5) / height);
  ray.direction[2] = -1 * camera.d;

  Eigen::Matrix3d cam_transform;
  cam_transform << camera.u, camera.v, camera.w;

  ray.direction = cam_transform * ray.direction;

}

