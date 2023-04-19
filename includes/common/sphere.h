#ifndef COMMON_SPHERE_H
#define COMMON_SPHERE_H
#include <memory>

#include "vec3.h"
#include "hittable.h"

class Sphere : public Hittable
{
 public:
  Sphere();
  Sphere(Vec3 center, double radius, std::shared_ptr<Material> mat_ptr);

  virtual bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& rec) const;

 private:
  Vec3 center_;
  double radius_;
  std::shared_ptr<Material> mat_ptr_;
};
#endif  // COMMON_SPHERE_H