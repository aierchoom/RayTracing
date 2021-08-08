#ifndef COMMON_VEC3_H
#define COMMON_VEC3_H
#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3 {
 public:
  Vec3() {}
  Vec3(float xcomp, float ycomp, float zcomp) {
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
  union {
    struct {
      float x, y, z;
    };
    struct {
      float r, g, b;
    };
  };
};

inline Vec3& Vec3::operator=(const Vec3& vec) {
  this->x = vec.x;
  this->y = vec.y;
  this->z = vec.z;
  return *this;
}

inline void Vec3::makeUnitVector() {
  float k = 1.0f / length();
  x *= k;
  y *= k;
  z *= k;
}

inline Vec3& Vec3::operator+=(const Vec3& vec) {
  this->x += vec.x;
  this->y += vec.y;
  this->z += vec.z;
  return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& vec) {
  this->x -= vec.x;
  this->y -= vec.y;
  this->z -= vec.z;
  return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& vec) {
  this->x *= vec.x;
  this->y *= vec.y;
  this->z *= vec.z;
  return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& vec) {
  this->x /= vec.x;
  this->y /= vec.y;
  this->z /= vec.z;
  return *this;
}

inline Vec3& Vec3::operator*=(const float scale) {
  this->x *= scale;
  this->y *= scale;
  this->z *= scale;
  return *this;
}

inline Vec3& Vec3::operator/=(const float scale) {
  this->x /= scale;
  this->y /= scale;
  this->z /= scale;
  return *this;
}

inline std::istream& operator>>(std::istream& is, Vec3& vec) {
  is >> vec.x >> vec.y >> vec.z;
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
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

inline Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
  return Vec3((lhs.y * rhs.z - lhs.z * rhs.y), (-(lhs.x * rhs.z - lhs.z * rhs.x)), (lhs.x * rhs.y - lhs.y * rhs.x));
}

inline Vec3 UnitVector(Vec3 vec) { return vec / vec.length(); }

#endif  // COMMON_VEC3_H