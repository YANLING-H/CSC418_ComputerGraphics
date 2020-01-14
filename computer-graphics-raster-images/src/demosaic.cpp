#include "demosaic.h"

void demosaic(
  const std::vector<unsigned char> & bayer,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
  rgb.resize(width*height*3);
  for (int row = 0; row < height; ++row) {
    int offset = row % 2 ? 0 : 1;
    for (int col = 0; col < width; ++col) {
      int index = width * row + col;      // Index in mosaic
      int channel = offset + (col % 2);   // Color channel of mosaic pixel

      // Get indexes of 8 neighbouring pixels
      int top_left = index - (width + 1),
          top = index - width,
          top_right = index - (width - 1),
          left = index - 1,
          right = index + 1,
          bot_left = index + (width - 1),
          bot = index + width,
          bot_right = index + (width + 1);
      
      int r, g, b;

      // Get value for r
      if (channel == 0) {
        // Red channel. Just take value.
        r = bayer[index];
      } else if (channel == 1) {
        // Green channel
        r = 0;
        int count = 0;
        if (row % 2 == 0) {
          // Red is vertically up and down
          if (row > 0) {
            r += bayer[top];
            count++;
          } 
          if (row < height - 1) {
            r += bayer[bot];
            count++;
          }
          
          if (count > 0)
            r /= count;
        } else {
          // Red is horizontally left and right
          if (col > 0) {
            r += bayer[left];
            count++;
          } 
          if (col < width - 1) {
            r += bayer[right];
            count++;
          }
          
          if (count > 0)
            r /= count;
        }
      } else {
        // Blue channel. Red is in four corners.
        r = 0;
        int count = 0;
        if (row > 0 && col > 0) {
          r += bayer[top_left];
          count++;
        }
        if (row < height - 1 && col > 0) {
          r += bayer[bot_left];
          count++;
        }
        if (row > 0 && col < width - 1) {
          r += bayer[top_right];
          count++;
        }
        if (row < height - 1 && col < width - 1) {
          r += bayer[bot_right];
          count++;
        }

        if (count > 0)
          r /= count;
      }

      // Get value for g
      if (channel == 1) {
        // Green channel. Just take value.
        g = bayer[index];
      } else {
        // Red or Blue channel. Green is on four sides
        g = 0;
        int count = 0;
        if (row > 0) {
          g += bayer[top];
          count++;
        }
        if (row < height - 1) {
          g += bayer[bot];
          count++;
        }
        if (col > 0) {
          g += bayer[left];
          count++;
        } 
        if (col < width - 1) {
          g += bayer[right];
          count++;
        }
          
        if (count > 0)
          g /= count;
      }

      // Get value for b
      if (channel == 2) {
        // Blue channel. Just take value.
        b = bayer[index];
      } else if (channel == 1) {
        // Green channel
        b = 0;
        int count = 0;
        if (row % 2 == 1) {
          // Blue is vertically up and down
          if (row > 0) {
            b += bayer[top];
            count++;
          } 
          if (row < height - 1) {
            b += bayer[bot];
            count++;
          }
          
          if (count > 0)
            b /= count;
        } else {
          // Blue is horizontally left and right
          if (col > 0) {
            b += bayer[left];
            count++;
          } 
          if (col < width - 1) {
            b += bayer[right];
            count++;
          }
          
          if (count > 0)
            b /= count;
        }
      } else {
        // Red channel. Blue is in four corners.
        b = 0;
        int count = 0;
        if (row > 0 && col > 0) {
          b += bayer[top_left];
          count++;
        }
        if (row < height - 1 && col > 0) {
          b += bayer[bot_left];
          count++;
        }
        if (row > 0 && col < width - 1) {
          b += bayer[top_right];
          count++;
        }
        if (row < height - 1 && col < width - 1) {
          b += bayer[bot_right];
          count++;
        }

        if (count > 0)
          b /= count;
      }

      // Got all values. Put in output array.
      rgb[3 * index] = r;
      rgb[3 * index + 1] = g;
      rgb[3 * index + 2] = b;

    }
  }
}
