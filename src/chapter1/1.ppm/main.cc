#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/vec3.h"
#include "common/ray.h"

int main() {
  int col = 200;
  int row = 100;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(row, col);

  ppm_file << "P6\n" << col << " " << row << "\n255\n";
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      Vec3 color(float(j) / float(col), float(row - i) / float(row), 0.2);
      color *= 255.99f;
      canvas[i][j] = color;
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