#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  double denom = this->normal.dot(ray.direction);   // Denominator
  if (denom == 0)
    return false;   // Ray is parallel to plane. No hit possible.
  double numer = this->normal.dot(this->point) - this->normal.dot(ray.origin);    // Numerator
  t = numer / denom;
  if (t >= min_t) {
    n = this->normal.normalized();    // Normal is the same throughout plane
    return true;
  }
  return false;   // Plane is not min_t distance away
}

