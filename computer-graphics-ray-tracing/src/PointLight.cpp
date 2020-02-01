#include "PointLight.h"

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
  d = this->p - q;  // From q to p
  max_t = d.norm(); // Magnitude of direction
  d = d.normalized(); // Normalize direction vector
}
