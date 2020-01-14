#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
  bayer.resize(width*height);
  for (int row = 0; row < height; ++row) {
    int offset = row % 2 ? 0 : 1; // Add 1 for even rows
    for (int col = 0; col < width; ++col) {
      int index = width * row + col;  // Bayer index
      int channel = offset + (col % 2);   // The color channel for the bayer pixel
      // Simply copy the channel value from the rgb pixel to the bayer pixel
      bayer[index] = rgb[3 * index + channel];
    }
  }
}
