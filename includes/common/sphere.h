#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "hittable.h"
class Sphere : public Hittable
{
 public:
  Sphere();
  Sphere(Vec3 center, double radius);

  virtual bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& rec) const;

 private:
  Vec3 center_;
  double radius_;
};
#endif  // SPHERE_H