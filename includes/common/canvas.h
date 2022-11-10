#ifndef COMMON_CANVAS_H
#define COMMON_CANVAS_H

#include <vector>
#include "types.h"

class Vec3;

class Canvas
{
 public:
  Canvas(int width, int height);
  virtual ~Canvas();

  std::vector<Vec3>& operator[](int index);

  byte* ConvertToPpmData();

 public:
  // Saved by pixel column.
  std::vector<std::vector<Vec3>> canvas_;

 private:
  int width_, height_;
};

#endif  // COMMON_CANVAS_H