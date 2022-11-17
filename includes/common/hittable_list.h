#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

#include "hittable.h"

class HittableList
{
 public:
  HittableList();
  HittableList(shared_ptr<Hittable> object);

  void Clear();
  void Add(shared_ptr<Hittable> object);

  virtual bool Hit(const Ray& ray, double tmin, double tmax, Hittable::HitRecord& record) const;

 private:
  std::vector<shared_ptr<Hittable>> objects_;
};
#endif  // HITTABLE_LIST_H