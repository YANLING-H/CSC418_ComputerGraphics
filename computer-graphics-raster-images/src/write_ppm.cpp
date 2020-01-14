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

  /*
   * Based on ppm specifications obtained from
   * https://en.wikipedia.org/wiki/Netpbm_format#File_format_description
   * 
   */

  if (!ppm_file.is_open()) {
    // Some error occurred
    std::cout << "Could not open file '" << filename << "'" << std::endl;
    return false;
  }

  ppm_file << "P3" << std::endl;  // Using P3 specifications
  ppm_file << width << " " << height << std::endl;  // Input width and height
  ppm_file << 255 << std::endl;   // Max channel value

  for (size_t i = 0; i < width * height * num_channels; ++i) {
    ppm_file << (unsigned int) data[i];

    // For single channel input, repeat channel value to imitate rgb channel
    // as ppm expects 3 channel input
    if (num_channels == 1)
      ppm_file << " " << (unsigned int) data[i] << " " << (unsigned int) data[i];

    if ((i+1) % (width * num_channels) == 0)
      ppm_file << std::endl;    // New line on new row
    else if ((i+1) % num_channels == 0)
      ppm_file << "  ";   // Whitespace between pixel values
    else
      ppm_file << " ";    // Whitespace between channel values
  }

  ppm_file.close(); // Close file
  return true;
}
