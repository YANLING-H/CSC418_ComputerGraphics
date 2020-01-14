#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert(
    (num_channels == 3 || num_channels ==1 ) &&
    ".ppm only supports RGB or grayscale images");
  std::ofstream ppm_file(filename);
  if (!ppm_file.is_open()) {
    std::cout << "Could not open file '" << filename << "'" << std::endl;
    return false;
  }
  ppm_file << "P3" << std::endl;
  ppm_file << width << " " << height << std::endl;
  ppm_file << 255 << std::endl;

  for (size_t i = 0; i < width * height * num_channels; ++i) {
    ppm_file << (unsigned int) data[i];

    if (num_channels == 1)
      ppm_file << " " << (unsigned int) data[i] << " " << (unsigned int) data[i];

    if ((i+1) % (width * num_channels) == 0)
      ppm_file << std::endl;
    else if ((i+1) % num_channels == 0)
      ppm_file << "  ";
    else
      ppm_file << " ";
  }

  ppm_file.close();
  return true;
}
