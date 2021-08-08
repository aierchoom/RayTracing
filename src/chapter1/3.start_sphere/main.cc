#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/vec3.h"
#include "common/ray.h"

float HitSphere(const Vec3 &center, float radius, const Ray &ray) {
  Vec3 origin_to_center = ray.origin() - center;
  // dot(B,B)
  float a = dot(ray.direction(), ray.direction());
  // 2*dot(B,A-C)
  float b = 2.0f * dot(ray.direction(), origin_to_center);
  // dot(A-C,A-C)-R*R
  float c = dot(origin_to_center, origin_to_center) - radius * radius;
  // t*t*dot(B,B)+2*t*dot(B,A-C)+dot(A-C,A-C)-R*R=0
  // 将上述球体方程看作二次式求t的解
  float discriminant = b * b - 4 * a * c;
  // 当方程无解(即光线未命中球体时，返回未击中)
  if (discriminant < 0) {
    return -1.0f;
  } else {
    // 求解光线击中球体的值t
    return (-b - sqrt(discriminant)) / (2.0f * a);
  }
}

Vec3 Color(const Ray &ray) {
  float t = HitSphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f, ray);
  if (t > 0.0f) {
    Vec3 normal = UnitVector(ray.pointAtParamter(t) - Vec3(0.0f, 0.0f, -1.0f));
    return Vec3(normal.x + 1, normal.y + 1, normal.z + 1) * 0.5f;
  }

  // 光线方向
  Vec3 unit_direction = UnitVector(ray.direction());
  // 光线强度(使用画布的高度作为强度，并且clamp to (0.0,1.0))
  t = 0.5f * (unit_direction.y + 1.0f);
  // (1.0f - t)翻转光线的强度方向，让底部的光线更偏向于白色
  return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() {
  int nx = 1200;
  int ny = 600;

  const char *render_target = "image.ppm";

  std::ofstream ppm_file(render_target, std::ios::binary);
  Canvas canvas(ny, nx);

  Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
  Vec3 horizontal(4.0f, 0.0f, 0.0f);
  Vec3 vertical(0.0f, 2.0f, 0.0f);
  Vec3 origin(0.0f, 0.0f, 0.0f);

  ppm_file << "P6\n" << nx << " " << ny << "\n255\n";
  // 1.以左下角作为基准点去扫描
  // 2.图片绘制的习惯为从左上角开始绘制
  // 3.j=ny-1,x=0;即将j光标移动到左下角开始绘制
  // 4.综上，需要翻转y轴的v值
  // 5.左下角uv为(0,0)
  // 6.左下角的光线方向为(0,0,0)->(-2,-1,-1)
  // 7.右上角的光线方向为(0,0,0)->(2,1,-1)
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(ny - j) / float(ny);
      // origin:光线原点
      // lower_left_corner(-2,-1,-1)+horizontal(4.0, 0.0, 0.0)+vertical(0.0, 2.0, 0.0)=upper_right_corner(2, 1, -1)
      // lower_left_corner + u * horizontal + v * vertical映射到了画布任意一个点的方向
      // Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      // 散射光
      Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      Vec3 color   = Color(ray) * 255.99;
      canvas[j][i] = color;
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