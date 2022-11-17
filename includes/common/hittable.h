#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"

class Hittable
{
 public:
  struct HitRecord {
    Vec3 p;
    Vec3 normal;
    double t;
    bool front_face;
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
    {
      front_face = dot(r.direction(), outward_normal) < 0;
      normal     = front_face ? outward_normal : -outward_normal;
    }
  };

 public:
  virtual bool Hit(const Ray& r, double tmin, double tmax, HitRecord& record) const = 0;
};

#endif  // HITTABLE_H