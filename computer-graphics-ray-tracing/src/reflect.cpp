#include <Eigen/Core>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
  return in - (2.0 * n.dot(in) * n);  // Formula based on lecture slide
}
