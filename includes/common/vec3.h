#ifndef COMMON_VEC3_H
#define COMMON_VEC3_H
#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "rtweekend.h"

class Vec3
{
 public:
  Vec3() {}
  Vec3(float xcomp, float ycomp, float zcomp)
  {
    this->x = xcomp;
    this->y = ycomp;
    this->z = zcomp;
  }

  inline const Vec3 operator+() const { return *this; }
  inline Vec3 operator-() const { return Vec3(-x, -y, -z); }

  inline Vec3& operator=(const Vec3& vec);

  inline Vec3& operator+=(const Vec3& vec);
  inline Vec3& operator-=(const Vec3& vec);
  inline Vec3& operator*=(const Vec3& vec);
  inline Vec3& operator/=(const Vec3& vec);
  inline Vec3& operator*=(const float scale);
  inline Vec3& operator/=(const float scale);

  inline float length() const { return sqrt(x * x + y * y + z * z); }
  inline float squaredLength() const { return x * x + y * y + z * z; }
  inline void makeUnitVector();

 public:
  inline static Vec3 random() { return Vec3(random_double(), random_double(), random_double()); }

  inline static Vec3 random(double min, double max) { return Vec3(random_double(min, max), random_double(min, max), random_double(min, max)); }

 public:
  union {
    struct {
      float x, y, z;
    };
    struct {
      float r, g, b;
    };
  };
};

inline Vec3& Vec3::operator=(const Vec3& vec)
{
  this->x = vec.x;
  this->y = vec.y;
  this->z = vec.z;
  return *this;
}

inline void Vec3::makeUnitVector()
{
  float k = 1.0f / length();
  x *= k;
  y *= k;
  z *= k;
}

inline Vec3& Vec3::operator+=(const Vec3& vec)
{
  this->x += vec.x;
  this->y += vec.y;
  this->z += vec.z;
  return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& vec)
{
  this->x -= vec.x;
  this->y -= vec.y;
  this->z -= vec.z;
  return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& vec)
{
  this->x *= vec.x;
  this->y *= vec.y;
  this->z *= vec.z;
  return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& vec)
{
  this->x /= vec.x;
  this->y /= vec.y;
  this->z /= vec.z;
  return *this;
}

inline Vec3& Vec3::operator*=(const float scale)
{
  this->x *= scale;
  this->y *= scale;
  this->z *= scale;
  return *this;
}

inline Vec3& Vec3::operator/=(const float scale)
{
  this->x /= scale;
  this->y /= scale;
  this->z /= scale;
  return *this;
}

inline std::istream& operator>>(std::istream& is, Vec3& vec)
{
  is >> vec.x >> vec.y >> vec.z;
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec)
{
  os << vec.x << " "
     << " " << vec.y << " " << vec.z;
  return os;
}

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }

inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }

inline Vec3 operator/(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }

inline Vec3 operator*(float scale, const Vec3& vec) { return Vec3(scale * vec.x, scale * vec.y, scale * vec.z); }

inline Vec3 operator*(const Vec3& vec, float scale) { return Vec3(scale * vec.x, scale * vec.y, scale * vec.z); }

inline Vec3 operator/(const Vec3& vec, float scale) { return Vec3(vec.x / scale, vec.y / scale, vec.z / scale); }

inline float dot(const Vec3& lhs, const Vec3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

inline Vec3 cross(const Vec3& lhs, const Vec3& rhs) { return Vec3((lhs.y * rhs.z - lhs.z * rhs.y), (-(lhs.x * rhs.z - lhs.z * rhs.x)), (lhs.x * rhs.y - lhs.y * rhs.x)); }

inline Vec3 UnitVector(Vec3 vec) { return vec / vec.length(); }

inline Vec3 random_in_unit_sphere()
{
  while (true) {
    Vec3 p = Vec3::random(-1, 1);
    if (p.squaredLength() >= 1) continue;
    return p;
  }
}

inline Vec3 random_unit_vector()
{
  auto a = random_double(0, 2 * pi);
  auto z = random_double(-1, 1);
  auto r = sqrt(1 - z * z);
  return Vec3(r * cos(a), r * sin(a), z);
}

inline Vec3 random_in_unit_disk()
{
  while (true) {
    auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.squaredLength() >= 1) continue;
    return p;
  }
}

inline Vec3 gamma_correct(const Vec3& color)
{
  auto r = sqrt(color.r);
  auto g = sqrt(color.g);
  auto b = sqrt(color.b);

  return Vec3(r, g, b);
}

inline Vec3 anti_aliasing(Vec3 color, int samples_count)
{
  auto scale = 1.0f / samples_count;

  color = color * scale;

  auto r = clamp(color.r, 0.0f, 0.999f);
  auto g = clamp(color.g, 0.0f, 0.999f);
  auto b = clamp(color.b, 0.0f, 0.999f);

  return Vec3(r, g, b);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) { return v - 2 * dot(v, n) * n; }

inline Vec3 refract(const Vec3& uv, const Vec3& n, double refraction)
{
  auto cos_theta      = fmin(dot(-uv, n), 1.0);
  Vec3 r_out_perp     = refraction * (uv + cos_theta * n);
  Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredLength())) * n;
  return r_out_perp + r_out_parallel;
}

#endif  // COMMON_VEC3_H