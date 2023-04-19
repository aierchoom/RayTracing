#ifndef COMMON_CANVAS_H
#define COMMON_CANVAS_H

#include <vector>
#include "typesdef.h"

class Vec3;

class Canvas
{
 public:
  Canvas(int width, int height);
  virtual ~Canvas();

  std::vector<Vec3>& operator[](int index);

  int GetDataSize() const;

  byte* ConvertToPpmData();

 private:
  // Saved by pixel column.
  std::vector<std::vector<Vec3>> canvas_;

  int width_, height_;
};

#endif  // COMMON_CANVAS_H