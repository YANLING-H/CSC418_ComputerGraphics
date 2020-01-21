#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  double denom = this->normal.dot(ray.direction);
  if (denom == 0)
    return false;
  double numer = this->normal.dot(this->point) - this->normal.dot(ray.origin);
  t = numer / denom;
  if (t >= min_t) {
    n = this->normal;
    return true;
  }
  return false;
}

