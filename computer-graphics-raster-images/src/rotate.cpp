#include "rotate.h"

void rotate(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & rotated)
{
  rotated.resize(height*width*num_channels);
  for (int row = 0; row < height; ++row) {
    int rotated_col = row;  // Column index of rotated image is row index of input image
    for (int col = width-1; col >= 0; --col) {
      int rotated_row = width - col - 1;  // Row index of rotated image is inverted column index of input image
      for (int channel = 0; channel < num_channels; ++channel) {
        rotated[num_channels * (height * rotated_row + rotated_col) + channel] 
          = input[num_channels * (width * row + col) + channel];
      }
    }
  }
}
