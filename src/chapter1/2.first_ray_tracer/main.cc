#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/vec3.h"
#include "common/ray.h"

const char *separator = "----------";

Vec3 Color(const Ray &ray)
{
  // 光线方向
  Vec3 unit_direction = UnitVector(ray.direction());
  // 光线强度(使用画布的高度作为强度，并且clamp to (0.0,1.0))
  float t = 0.5f * (unit_direction.y + 1.0f);
  // (1.0f - t)翻转光线的强度方向，让底部的光线更偏向于白色
  return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
  int width  = 1200;
  int height = 600;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(width, height);

  Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
  Vec3 horizontal(4.0f, 0.0f, 0.0f);
  Vec3 vertical(0.0f, 2.0f, 0.0f);
  Vec3 origin(0.0f, 0.0f, 0.0f);

  ppm_file << "P6\n" << width << " " << height << "\n255\n";
  // 1.以左下角作为基准点去扫描
  // 2.图片绘制的习惯为从左上角开始绘制
  // 3.y=height-1,x=0;即将j光标移动到左下角开始绘制
  // 4.综上，需要翻转y轴的v值
  // 5.左下角uv为(0,0)
  // 6.左下角的光线方向为(0,0,0)->(-2,-1,-1)
  // 7.右上角的光线方向为(0,0,0)->(2,1,-1)
  for (int y = height - 1; y >= 0; y--) {
    // std::cout << separator << "line:" << height - y << separator << std::endl;
    for (int x = 0; x < width; x++) {
      // lower_left_corner uv(0.0f,0.0f)
      float u = float(x) / float(width);
      float v = float(height - y) / float(height);
      // origin:光线出射点
      // lower_left_corner(-2,-1,-1)+horizontal(4.0, 0.0, 0.0)+vertical(0.0, 2.0, 0.0)=upper_right_corner(2, 1, -1)
      // lower_left_corner + u * horizontal + v * vertical映射到了画布任意一个点的方向
      // Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      // 散射光
      Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      // std::cout << "point" << x + 1 << ":\t" << ray.direction() << std::endl;
      Vec3 color   = Color(ray) * 255.99;
      canvas[x][y] = color;
    }
  }

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