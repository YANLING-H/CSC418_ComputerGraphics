#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
  gray.resize(height*width);
  // Weights obtained from the assignment handout
  float red_weight = 0.2126, green_weight = 0.7152, blue_weight = 0.0722;

  for (int i = 0; i < width * height * 3; i += 3) {
    float r = rgb[i], g = rgb[i+1], b = rgb[i+2];
    gray[i/3] = (unsigned char) (r * red_weight + g * green_weight + b * blue_weight);  // Pixel is weighted average
  }
}


