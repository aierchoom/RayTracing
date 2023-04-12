#ifndef COMMON_CAMERA_H
#define COMMON_CAMERA_H
#include "rtweekend.h"
class Camera
{
 public:
  Camera()
  {
    lower_left_corner_ = Vec3(-2.0f, -1.0f, -1.0f);
    horizontal_        = Vec3(4.0f, 0.0f, 0.0f);
    vertical_          = Vec3(0.0f, 2.0f, 0.0f);
    origin_            = Vec3(0.0f, 0.0f, 0.0f);
  }

  Ray GetRay(double u, double v)
  {
    // origin:光线原点
    // lower_left_corner(-2,-1,-1)+horizontal(4.0, 0.0, 0.0)+vertical(0.0, 2.0, 0.0)=upper_right_corner(2, 1, -1)
    // lower_left_corner + u * horizontal + v * vertical映射到了画布任意一个点的方向
    // Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
    // 散射光

    return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
  }

 public:
  Vec3 origin_;
  Vec3 lower_left_corner_;
  Vec3 horizontal_;
  Vec3 vertical_;
};
#endif  // COMMON_CAMERA_H