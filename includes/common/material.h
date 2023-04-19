#ifndef COMMON_MATERIAL_H
#define COMMON_MATERIAL_H
#include "hittable.h"

class Material
{
 public:
  virtual bool Scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
 public:
  Lambertian(const Vec3& albedo) : albedo_(albedo) {}

  virtual bool Scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& scattered) const
  {
    Vec3 scatter_dir = record.normal_ + random_unit_vector();
    scattered        = Ray(record.p_, scatter_dir);
    attenuation      = albedo_;
    return true;
  }

 private:
  Vec3 albedo_;
};

class Metal : public Material
{
 public:
  Metal(const Vec3& albedo, float fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

  virtual bool Scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& scattered) const
  {
    Vec3 reflected = reflect(UnitVector(ray_in.Direction()), record.normal_);
    scattered      = Ray(record.p_, reflected + fuzz_ * random_in_unit_sphere());

    attenuation = albedo_;
    return (dot(scattered.Direction(), record.normal_) > 0);
  }

 private:
  Vec3 albedo_;
  float fuzz_;
};

class Dielectric : public Material
{
 public:
  Dielectric(double ri) : ref_idx_(ri) {}

  virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
  {
    attenuation           = Vec3(1.0, 1.0, 1.0);
    double etai_over_etat = (rec.front_face_) ? (1.0 / ref_idx_) : (ref_idx_);
    Vec3 unit_direction   = UnitVector(r_in.Direction());

    double cos_theta = ffmin(dot(-unit_direction, rec.normal_), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      Vec3 reflected = reflect(unit_direction, rec.normal_);
      scattered      = Ray(rec.p_, reflected);
      return true;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);

    if (random_double() < reflect_prob) {
      Vec3 reflected = reflect(unit_direction, rec.normal_);
      scattered      = Ray(rec.p_, reflected);
      return true;
    }

    Vec3 refracted = refract(unit_direction, rec.normal_, etai_over_etat);
    scattered      = Ray(rec.p_, refracted);
    return true;
  }

 private:
  double ref_idx_;
};

#endif  // COMMON_MATERIAL_H