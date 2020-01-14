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
      int index = width * row + col;
      int channel = offset + (col % 2);
      int top_left = index - (width + 1),
          top = index - width,
          top_right = index - (width - 1),
          left = index - 1,
          right = index + 1,
          bot_left = index + (width - 1),
          bot = index + width,
          bot_right = index + (width + 1);
      
      int r, g, b;

      // get value for r
      if (channel == 0) {
        // red channel. Just take value.
        r = bayer[index];
      } else if (channel == 1) {
        // Green channel
        r = 0;
        int count = 0;
        if (row % 2 == 0) {
          // red is vertically up and down
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
          // red is horizontally left and right
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

      // get value for g
      if (channel == 1) {
        // green channel. Just take value.
        g = bayer[index];
      } else {
        // red or blue channel. green is on four sides
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

      // get value for b
      if (channel == 2) {
        // blue channel. Just take value.
        b = bayer[index];
      } else if (channel == 1) {
        // Green channel
        b = 0;
        int count = 0;
        if (row % 2 == 1) {
          // blue is vertically up and down
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
          // blue is horizontally left and right
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
        // red channel. blue is in four corners.
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

      // Got all values
      rgb[3 * index] = r;
      rgb[3 * index + 1] = g;
      rgb[3 * index + 2] = b;

    }
  }
}
