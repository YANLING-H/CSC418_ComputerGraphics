#include "catmull_rom_interpolation.h"
#include <Eigen/Dense>

Eigen::Vector3d catmull_rom_interpolation(
  const std::vector<std::pair<double, Eigen::Vector3d> > & keyframes,
  double t)
{

  if (keyframes.size() == 0)
    return Eigen::Vector3d::Zero();

  if (keyframes.size() == 1)
    return keyframes[0].second;

  if (t <= keyframes.front().first)
    return keyframes.front().second;
  if (t >= keyframes.back().first)
    return keyframes.back().second;

  double T;
  Eigen::Vector3d theta0, theta1, m0, m1;

  for (int i=0; i<keyframes.size(); ++i) {
    auto f1 = keyframes[i];
    if (t > f1.first)
      continue;
    if (t == f1.first)
      return f1.second;
    auto f0 = keyframes[i-1];
    double t0 = f0.first, t1 = f1.first;
    T = (t - t0) / (t1 - t0);
    theta0 = f0.second;
    theta1 = f1.second;
    
    // From https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
    // Read _1 as -1
    auto p_1 = (i == 1) ? f0.second : keyframes[i-2].second;
    auto p0 = f0.second;
    auto p1 = f1.second;
    auto p2 = (i == keyframes.size()-1) ? f1.second : keyframes[i+1].second;

    double t_1 = (i == 1) ? f0.first : keyframes[i-2].first;
    double t2 = (i == keyframes.size()-1) ? f1.first : keyframes[i+1].first;

    m0 = (p1 - p_1) / (t1 - t_1);
    m1 = (p2 - p0) / (t2 - t0);
    break;
  }

  double T2 = T * T;
  double T3 = T2 * T;

  return (2*T3 - 3*T2 + 1) * theta0 + (T3 - 2*T2 + T) * m0
       + (-2*T3 + 3*T2) * theta1 + (T3 - T2) * m1;
}
