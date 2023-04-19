#include "camera.h"

Camera::Camera(Vec3 look_from, Vec3 look_at, Vec3 up, double fov, double aspect, double aperture, double focus_dist)
{
  {
    origin_      = look_from;
    lens_radius_ = aperture / 2;

    double theta       = degrees_to_radians(fov);
    double half_height = tan(theta / 2);
    double half_width  = aspect * half_height;

    w_ = UnitVector(look_from - look_at);
    u_ = UnitVector(cross(up, w_));
    v_ = cross(w_, u_);

    lower_left_corner_ = origin_ - half_width * focus_dist * u_ - half_height * focus_dist * v_ - focus_dist * w_;
    horizontal_        = 2 * half_width * focus_dist * u_;
    vertical_          = 2 * half_height * focus_dist * v_;
  }
}

Ray Camera::GetRay(double s, double t)
{
  Vec3 rd     = lens_radius_ * random_in_unit_disk();
  Vec3 offset = u_ * rd.x + v_ * rd.y;

  return Ray(origin_ + offset, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset);
}