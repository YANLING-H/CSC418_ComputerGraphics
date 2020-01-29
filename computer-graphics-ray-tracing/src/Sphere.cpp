#include "Sphere.h"
#include "Ray.h"
#include <math.h>

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  Eigen::Vector3d x = ray.origin - this->center;    // Vector from sphere center to ray origin

  // Coefficients of equation a*t^2 + b*t + c = 0
  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * ray.direction.dot(x);
  double c = x.dot(x) - this->radius * this->radius;
  
  double d = b * b - 4 * a * c;   // Discriminant
  if (d <= 0)
    return false;   // None or one solution to equation, so no hit on sphere.
  t = (-1 * b - sqrt(d)) / (2.0 * a);     // First hit on sphere
  if (t >= min_t) {
    // First hit is atleast min_t distance away
    n = (ray.origin + t * ray.direction - this->center).normalized();
    return true;
  }

  // First hit is not min_t distance away
  t = (-1 * b + sqrt(d)) / (2.0 * a);   // Second hit on sphere.
  if (t >= min_t) {
    // We are inside the sphere
    n = (ray.origin + t * ray.direction - this->center).normalized();
    return true;
  }
  return false; // Sphere is behind us so no hit.
}

