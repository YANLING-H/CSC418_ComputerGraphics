#include "desaturate.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void desaturate(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double factor,
  std::vector<unsigned char> & desaturated)
{
  desaturated.resize(rgb.size());
  double h=0, s=0, v=0, r=0, g=0, b=0;
  for (int i = 0; i < width * height * 3; i += 3) {
    r = rgb[i] / 255.0, g = rgb[i+1] / 255.0, b = rgb[i+2] / 255.0;   // Convert rgb values to [0, 1] range
    rgb_to_hsv(r, g, b, h, s, v);   // Get pixel in hsv
    s *= (1.0 - factor);            // Apply desaturation
    hsv_to_rgb(h, s, v, r, g, b);   // Reconvert new hsv pixel to rgb
    // Reconvert rgb values to [0, 255] range and add to output array
    desaturated[i] = (unsigned char) (r * 255);
    desaturated[i+1] = (unsigned char) (g * 255);
    desaturated[i+2] = (unsigned char) (b * 255);
  }
}
