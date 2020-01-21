#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  bool hit_found = false;
  double closest_hit = 0.0;
  double hit_t;
  Eigen::Vector3d hit_n;

  for (int i = 0; i < objects.size(); ++i) {
    bool hit = objects[i]->intersect(ray, min_t, hit_t, hit_n);
    if (hit) {
      if (!hit_found || hit_t < closest_hit) {
        hit_id = i;
        t = hit_t;
        n = hit_n;
        closest_hit = hit_t;
      }
      hit_found = true;
    }
  }

  return hit_found;
}

