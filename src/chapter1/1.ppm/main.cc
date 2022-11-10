#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/vec3.h"
#include "common/ray.h"

int main()
{
  const int width  = 200;
  const int height = 100;

  const float COLOR_SCALE_COMP = 255.0f;
  const int COLOR_TO_BYTE_SIZE = 3;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(width, height);

  ppm_file << "P6\n" << width << " " << height << "\n255\n";
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Vec3 color(float(x) / float(width), float(height - y) / float(height), 0.2);
      color *= COLOR_SCALE_COMP;
      canvas[x][y] = color;
    }
  }

  ppm_file.write(canvas.ConvertToPpmData(), width * height * COLOR_TO_BYTE_SIZE);
  ppm_file.flush();
  ppm_file.close();

  std::string input_path = "image.ppm";
  int w, h, channel;
  unsigned char *data = stbi_load(input_path.c_str(), &w, &h, &channel, 0);

  std::string output_path = "image.png";
  stbi_write_png(output_path.c_str(), w, h, channel, data, 0);

  system("mspaint image.png");
}