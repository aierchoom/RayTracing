#include "sphere.h"

Sphere::Sphere() {}

Sphere::Sphere(const Vec3 center, double radius) : center_(center), radius_(radius) {}

bool Sphere::Hit(const Ray& ray, double tmin, double tmax, HitRecord& record) const
{
  Vec3 origin_to_center = ray.origin() - center_;
  // dot(B,B)
  float a = dot(ray.direction(), ray.direction());

  // b=2h, h*h-ac
  float half_b = dot(ray.direction(), origin_to_center);

  // dot(A-C,A-C)-R*R
  float c = dot(origin_to_center, origin_to_center) - radius_ * radius_;
  // t*t*dot(B,B)+2*t*dot(B,A-C)+dot(A-C,A-C)-R*R=0
  // 将上述球体方程看作二次式求t的解
  float discriminant = half_b * half_b - a * c;
  // 当方程无解(即光线未命中球体时，返回未击中)
  if (discriminant > 0) {
    float delta = sqrt(discriminant);
    // +root
    float root = (-half_b - delta) / a;
    if (root < tmax && root > tmin) {
      record.t            = root;
      record.p            = ray.at(record.t);
      Vec3 outward_normal = (record.p - center_) / radius_;
      record.set_face_normal(ray, outward_normal);
      return true;
    }
    // -root
    root = (-half_b - delta) / a;
    if (root < tmax && root > tmin) {
      record.t            = root;
      record.p            = ray.at(record.t);
      Vec3 outward_normal = (record.p - center_) / radius_;
      record.set_face_normal(ray, outward_normal);
      return true;
    }
  }
  return false;
}