#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/rtweekend.h"
#include "common/hittable_list.h"
#include "common/sphere.h"
#include "common/camera.h"

Vec3 RayColor(const Ray &ray, const Hittable &world)
{
  Hittable::HitRecord hit_rec;
  if (world.Hit(ray, 0, infinity, hit_rec)) {
    return 0.5 * (hit_rec.normal + Vec3(1, 1, 1));
  }
  Vec3 unit_direction = UnitVector(ray.direction());
  auto t              = 0.5f * (unit_direction.y + 1.0);
  return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main()
{
  int width  = 400;
  int height = 200;

  const int samples_per_pixel = 100;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(width, height);

  Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
  Vec3 horizontal(4.0f, 0.0f, 0.0f);
  Vec3 vertical(0.0f, 2.0f, 0.0f);
  Vec3 origin(0.0f, 0.0f, 0.0f);

  HittableList world;
  world.Add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
  world.Add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

  Camera camera;

  ppm_file << "P6\n" << width << " " << height << "\n255\n";
  // 1.以左下角作为基准点去扫描(行扫描)
  // 2.图片绘制的习惯为从左上角开始绘制
  // 3.y=height-1,x=0;即将j光标移动到左下角开始绘制
  // 4.综上，需要翻转y轴的v值
  // 5.左下角uv为(0,0)
  // 6.左下角的光线方向为(0,0,0)->(-2,-1,-1)
  // 7.右上角的光线方向为(0,0,0)->(2,1,-1)
  for (int y = height - 1; y >= 0; y--) {
    std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
    for (int x = 0; x < width; x++) {
      Vec3 color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u  = (x + random_double()) / (width);
        auto v  = (height - y + random_double()) / (height);
        Ray ray = camera.GetRay(u, v);
        color += RayColor(ray, world);
      }
      canvas[x][y] = anti_aliasing(color, samples_per_pixel) * 255.99f;
    }
  }

  std::cerr << "\nDone.\n";

  ppm_file.write(canvas.ConvertToPpmData(), width * height * 3);
  ppm_file.flush();
  ppm_file.close();

  std::string input_path = "image.ppm";
  int w, h, channel;
  unsigned char *data = stbi_load(input_path.c_str(), &w, &h, &channel, 0);

  std::string output_path = "image.png";
  stbi_write_png(output_path.c_str(), w, h, channel, data, 0);

  system("mspaint image.png");
}