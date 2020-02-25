#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");

  // Filestream to write to
  std::ofstream obj(filename);
  if (!obj.is_open()) {
    // Some error occurred
    std::cout << "Could not open file '" << filename << "'" << std::endl;
    return false;
  }

  // Write all vertex positions
  for (int i=0; i<V.rows(); ++i) {
    obj << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << std::endl;
  }

  // Write all texture coordinates
  for (int i=0; i<UV.rows(); ++i) {
    obj << "vt " << UV(i, 0) << " " << UV(i, 1) << " " << UV(i, 2) << std::endl;
  }

  // Write all normals
  for (int i=0; i<NV.rows(); ++i) {
    obj << "vn " << NV(i, 0) << " " << NV(i, 1) << " " << NV(i, 2) << std::endl;
  }

  // Write all faces
  for (int i=0; i<F.rows(); ++i) {
    obj << "f ";
    for (int j=0; j<F.cols(); ++j) {
      obj << F(i, j) << "/" << UF(i, j) << "/" << NF(i, j);
      if (j < F.cols()-1)
        obj << " ";
    }
    obj << std::endl;
  }

  // Close file and return
  obj.close();
  return true;
}
