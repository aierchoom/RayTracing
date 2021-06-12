#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/vec3.h"
#include "common/ray.h"

Vec3 Color(const Ray &ray) {
  // 获得光线的方向
  Vec3 unit_direction = UnitVector(ray.direction());
  // 获得将y限定到[0,1]的值t
  float t = 0.5f * (unit_direction.y + 1.0f);
  /**
   * Vec3(0.5f, 0.7f, 1.0f)=sky color(blue)
   * r:(1.0-t)+t*0.5
   * g:(1.0-t)+t*0.7
   * b:(1.0-t)+t=1.0
   */
  return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() {
  int nx = 800;
  int ny = 400;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(ny, nx);

  Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
  Vec3 horizontal(4.0f, 0.0f, 0.0f);
  Vec3 vertical(0.0f, 2.0f, 0.0f);
  Vec3 origin(0.0f, 0.0f, 0.0f);

  ppm_file << "P6\n" << nx << " " << ny << "\n255\n";
  for (int i = 0; i < ny; i++) {
    for (int j = 0; j < nx; j++) {
      // u:0->1
      float u = float(j) / float(nx);
      // v:1->0
      float v = float(ny - i) / float(ny);
      Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      Vec3 color   = Color(ray) * 255.99;
      canvas[i][j] = color;
    }
  }

  ppm_file.write(canvas.ToPpmData(), nx * ny * 3);
  ppm_file.flush();
  ppm_file.close();

  std::string input_path = "image.ppm";
  int w, h, channel;
  unsigned char *data = stbi_load(input_path.c_str(), &w, &h, &channel, 0);

  std::string output_path = "image.png";
  stbi_write_png(output_path.c_str(), w, h, channel, data, 0);

  system("mspaint image.png");
}