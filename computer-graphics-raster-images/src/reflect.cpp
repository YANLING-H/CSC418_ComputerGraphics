#include "reflect.h"

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
  reflected.resize(width*height*num_channels);
  for (size_t row = 0; row < height; ++row) {
    int left = 0, right = width - 1;
    while (left <= right) {
      for (size_t color = 0; color < num_channels; ++color) {
        reflected[num_channels * (width * row + left) + color] = input[num_channels * (width * row + right) + color];
        reflected[num_channels * (width * row + right) + color] = input[num_channels * (width * row + left) + color];
      }
      left++;
      right--;
    }
  }
}
