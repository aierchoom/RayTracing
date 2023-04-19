#ifndef COMMON_RAY_H
#define COMMON_RAY_H

#include "rtweekend.h"
#include "vec3.h"

class Ray
{
 public:
  Ray() {}
  Ray(const Vec3& a, const Vec3& b) : a_(a), b_(b) {}
  Vec3 Origin() const { return a_; }
  Vec3 Direction() const { return b_; }
  Vec3 At(float t) const { return a_ + t * b_; }

 private:
  Vec3 a_;
  Vec3 b_;
};
#endif  // COMMON_RAY_H