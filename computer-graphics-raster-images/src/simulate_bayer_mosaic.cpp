#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
  bayer.resize(width*height);
  for (int row = 0; row < height; ++row) {
    int offset = row % 2 ? 0 : 1;
    for (int col = 0; col < width; ++col) {
      int index = 3 * (width * row + col);
      int channel = offset + (col % 2);
      bayer[width * row + col] = rgb[index + channel];
    }
  }
}
