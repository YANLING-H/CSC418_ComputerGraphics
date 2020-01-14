#include "rgb_to_hsv.h"
#include <algorithm>
#include <cmath>

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{

  /* 
   * Conversion algorithm adapted from
   * https://en.wikipedia.org/wiki/HSL_and_HSV
   * 
   */

  double M = std::max({r, g, b}); // Max channel value
  double m = std::min({r, g, b}); // Min channel value
  double C = M - m;   // Chroma value

  // Get value for h
  if (C == 0)
    h = 0;  // undefined
  else if (M == r)
    h = std::fmod((g - b) / C, 6);
  else if (M == g)
    h = ((b - r) / C) + 2;
  else if (M == b)
    h = ((r - g) / C) + 4;

  h *= 60;

  // Fix h value to [0, 360) range
  if (h < 0) {
    h *= -1;
    h = std::fmod(h, 360);
    h = 360 - h;
  } else
    h = std::fmod(h, 360);

  // Get value for v
  v = M;

  // Get value for s
  if (v == 0)
    s = 0;
  else
    s = C / v;
}
