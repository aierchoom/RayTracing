#ifndef COMMON_HITTABLE_H
#define COMMON_HITTABLE_H

#include <memory>

#include "vec3.h"
#include "ray.h"

class Material;

struct HitRecord {
  Vec3 p_;
  Vec3 normal_;
  std::shared_ptr<Material> mat_ptr_;
  double t_;
  bool front_face_;

  inline void SetFaceNormal(const Ray& r, const Vec3& outward_normal)
  {
    front_face_ = dot(r.Direction(), outward_normal) < 0;
    normal_     = front_face_ ? outward_normal : -outward_normal;
  }
};

class Hittable
{
 public:
  virtual bool Hit(const Ray& r, double tmin, double tmax, HitRecord& record) const = 0;
};

#endif  // COMMON_HITTABLE_H