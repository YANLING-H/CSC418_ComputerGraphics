#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

#define OFFSET 1e-4
#define MAX_DEPTH 2

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  rgb = Eigen::Vector3d(0,0,0);
  bool hit;
  int hit_id;
  double t;
  Eigen::Vector3d n;
  if (first_hit(ray, min_t, objects, hit_id, t, n)) {
    rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    if (num_recursive_calls < MAX_DEPTH && !objects[hit_id]->material->km.isZero(OFFSET)) {
      // We have not yet reached max depth
      // Object has reflective properties, so we generate and cast reflected ray
      Ray reflected_ray;
      reflected_ray.origin = ray.origin + t * ray.direction;
      reflected_ray.direction = reflect(ray.direction, n);
      Eigen::Vector3d color;
      if (raycolor(reflected_ray, OFFSET, objects, lights, num_recursive_calls + 1, color))
        rgb += (objects[hit_id]->material->km.array() * color.array()).matrix();  // Multiply with objects reflective coefficients
    }
    return true;
  }
  return false;
}
