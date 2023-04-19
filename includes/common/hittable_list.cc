#include "hittable_list.h"

using std::shared_ptr;
using std::make_shared;

HittableList::HittableList() {}
HittableList::HittableList(shared_ptr<Hittable> object) { objects_.push_back(object); }

void HittableList::Clear() { objects_.clear(); }
void HittableList::Add(shared_ptr<Hittable> object) { objects_.push_back(object); }

bool HittableList::Hit(const Ray& ray, double tmin, double tmax, HitRecord& record) const
{
  HitRecord temp_record;
  bool hit_anything     = false;
  double closest_so_far = tmax;

  for (const shared_ptr<Hittable>& object : objects_) {
    if (object->Hit(ray, tmin, closest_so_far, temp_record)) {
      hit_anything   = true;
      closest_so_far = temp_record.t_;
      record         = temp_record;
    }
  }
  return hit_anything;
}
