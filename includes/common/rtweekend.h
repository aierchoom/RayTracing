#ifndef COMMON_RTWEEKEND_H
#define COMMON_RTWEEKEND_H
#include <cmath>
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

inline double clamp(double x, double min, double max)
{
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

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

inline double schlick(double cosine, double ref_idx)
{
  auto r0 = (1 - ref_idx) / (1 + ref_idx);
  r0      = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif  // COMMON_RTWEEKEND_H