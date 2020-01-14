#include "over.h"

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
  /*
   * Formulas obtained from
   * https://en.wikipedia.org/wiki/Alpha_compositing
   * 
   */
  C.resize(A.size());
  for (int i = 0; i < width * height * 4; i += 4) {
    double aa = A[i + 3] / 255.0, ab = B[i + 3] / 255.0;    // Alpha values for pixel in A and B respectively
    for (int j = 0; j < 3; ++j) {
      // Color value of pixel
      C[i + j] = (unsigned char) ((double) A[i + j] * aa + (double) B[i + j] * ab * (1.0 - aa));
    }
    // Alpha value of pixel
    C[i + 3] = (unsigned char) ((aa + ab * (1.0 - aa)) * 255);
  }
}
