#ifndef COMMON_HITTABLE_LIST_H
#define COMMON_HITTABLE_LIST_H
#include <memory>
#include <vector>

#include "hittable.h"

class HittableList : public Hittable
{
 public:
  HittableList();
  HittableList(std::shared_ptr<Hittable> object);

  void Clear();
  void Add(std::shared_ptr<Hittable> object);

  virtual bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& record) const;

 private:
  std::vector<std::shared_ptr<Hittable>> objects_;
};
#endif  // COMMON_HITTABLE_LIST_H