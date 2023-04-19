#ifndef COMMON_CAMERA_H
#define COMMON_CAMERA_H
#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

class Camera
{
 public:
  Camera(Vec3 look_from, Vec3 look_at, Vec3 up, double fov, double aspect, double aperture, double focus_dist);

  Ray GetRay(double s, double t);

 private:
  Vec3 origin_;
  Vec3 lower_left_corner_;
  Vec3 horizontal_;
  Vec3 vertical_;
  Vec3 u_, v_, w_;
  double lens_radius_;
};
#endif  // COMMON_CAMERA_H