#include <iostream>
#include <fstream>

#include "common/Canvas.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION

struct Color {
  float r, g, b;
};

int main() {
  int col = 2000;  // nx
  int row = 1000;  // ny

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(row, col);

  ppm_file << "P6\n" << col << " " << row << "\n255\n";
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      float r = float(j) / float(col);
      float g = float(row - i) / float(row);
      float b = 0.2;
      int ir  = int(255.99 * r);
      int ig  = int(255.99 * g);
      int ib  = int(255.99 * b);

      canvas[i][j].r = ir;
      canvas[i][j].g = ig;
      canvas[i][j].b = ib;
    }
  }

  ppm_file.write((char *)canvas.ToPpmData(), col * row * 3);
  ppm_file.flush();
  ppm_file.close();

  std::string input_path = "image.ppm";
  int w, h, channel;
  unsigned char *data = stbi_load(input_path.c_str(), &w, &h, &channel, 0);

  std::string output_path = "image.png";
  stbi_write_png(output_path.c_str(), w, h, channel, data, 0);

  system("mspaint image.png");
}