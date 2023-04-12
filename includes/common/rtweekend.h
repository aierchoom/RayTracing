#ifndef RTWEEKEND_H
#define RTWEEKEND_H
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
using std::shared_ptr;
using std::make_shared;
const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

inline double random_double()
{
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

inline double clamp(double x, double min, double max)
{
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

#include "ray.h"
#include "vec3.h"

inline Vec3 anti_aliasing(Vec3 color, int samples_count)
{
  auto scale = 1.0f / samples_count;
  auto r     = clamp(scale * color.r, 0.0f, 0.999f);
  auto g     = clamp(scale * color.g, 0.0f, 0.999f);
  auto b     = clamp(scale * color.b, 0.0f, 0.999f);
  return Vec3(r, g, b);
}
#endif