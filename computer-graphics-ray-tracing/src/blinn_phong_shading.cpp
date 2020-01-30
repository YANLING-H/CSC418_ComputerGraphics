#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>
#include <algorithm>

#define AMBIENT_INTENSITY 0.1
#define FUDGE_FACTOR 1e-6

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  Ray ray_to_light;
  ray_to_light.origin = ray.origin + t * ray.direction;
  double max_t;

  int new_hit_id;
  double new_t;
  Eigen::Vector3d new_n;

  Eigen::Vector3d rgb = Eigen::Vector3d(0,0,0);   // Color Vector

  // Add ambient Light:
  rgb += AMBIENT_INTENSITY * objects[hit_id]->material->ka;

  for (int i = 0; i < lights.size(); ++i) {
    lights[i]->direction(ray_to_light.origin, ray_to_light.direction, max_t);
    if (first_hit(ray_to_light, FUDGE_FACTOR, objects, new_hit_id, new_t, new_n) && new_t < max_t) {
      // Object is in shadow from this light
      continue;
    }
    // Object gets light from this light source

    // Lambertian Light:
    Eigen::Vector3d lambertian = (std::max(0.0, n.dot(ray_to_light.direction)) * objects[hit_id]->material->kd);

    // Specular Light:
    Eigen::Vector3d h = ((-1 * ray.direction) + ray_to_light.direction).normalized();
    Eigen::Vector3d specular = pow(std::max(0.0, n.dot(h)), objects[hit_id]->material->phong_exponent) * objects[hit_id]->material->ks;

    // Add up the color intensities to current intensity
    rgb += ((lambertian + specular).array() * lights[i]->I.array()).matrix();

  }

  // Clip rgb to max of 1.0
  rgb = rgb.cwiseMin(1.0);

  // Return resultant rgb
  return rgb;
}
