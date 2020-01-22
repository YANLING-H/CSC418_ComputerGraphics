#include "Sphere.h"
#include "Ray.h"
#include <math.h>

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  Eigen::Vector3d x = ray.origin - this->center;
  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * ray.direction.dot(x);
  double c = x.dot(x) - this->radius * this->radius;
  double d = b * b - 4 * a * c;
  if (d <= 0)
    return false;
  t = (-1 * b - sqrt(d)) / (2.0 * a);
  if (t >= min_t) {
    n = (ray.origin + t * ray.direction - this->center).normalized();
    return true;
  }
  t = (-1 * b + sqrt(d)) / (2.0 * a);
  if (t >= min_t) {
    n = (ray.origin + t * ray.direction - this->center).normalized();
    return true;
  }
  return false;
}

