#include "cube.h"

void cube(
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  // Resize Matrices
  V.resize(8,3);
  F.resize(6,4);
  UV.resize(14,2);
  UF.resize(6,4);
  NV.resize(6,3);
  NF.resize(6,4);

  // Vertices
  // Using a unit cube with one corner at (0, 0, 0) and its opposing corner at (1, 1, 1)
  V <<  0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1;

  // Texture Coordinates
  // Interpret each "cell" in texture map as a 1x1 square
  UV << 0, 1,
        0, 2,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        2, 3,
        3, 1,
        3, 2,
        4, 1,
        4, 2;

  // Convert UV coordinates to unit square
  UV /= 4.0;  // 4 is the length along an axis

  // Normals
  NV <<  1,  0,  0,
        -1,  0,  0,
         0,  1,  0,
         0, -1,  0,
         0,  0,  1,
         0,  0, -1;

  // Faces
  // Bottom face:
  F.row(0)  <<  1,  2,  6,  5;
  UF.row(0) <<  5,  9,  8,  4;
  NF.row(0) <<  4,  4,  4,  4;

  // Left Face:
  F.row(1)  <<  1,  5,  8,  4;
  UF.row(1) <<  5,  4,  1,  2;
  NF.row(1) <<  2,  2,  2,  2;

  // Right Face:
  F.row(2)  <<  2,  3,  7,  6;
  UF.row(2) <<  9, 12, 11,  8;
  NF.row(2) <<  1,  1,  1,  1;

  // Front Face:
  F.row(3)  <<  5,  6,  7,  8;
  UF.row(3) <<  4,  8,  7,  3;
  NF.row(3) <<  5,  5,  5,  5;

  // Back Face:
  F.row(4)  <<  4,  3,  2,  1;
  UF.row(4) <<  6, 10,  9,  5;
  NF.row(4) <<  6,  6,  6,  6;

  // Top Face:
  F.row(5)  <<  8,  7,  3,  4;
  UF.row(5) << 13, 11, 12, 14;
  NF.row(5) <<  3,  3,  3,  3;
  
}
