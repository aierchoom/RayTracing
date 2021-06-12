#ifndef COMMON_CANVAS_H
#define COMMON_CANVAS_H

#include <vector>
#include "common/vec3.h"
using std::vector;

class Canvas {
 public:
  Canvas(int row, int col) : row_(row), col_(col), canvas_() {
    canvas_.resize(row_);
    for (int i = 0; i < row_; i++) {
      canvas_[i] = new Vec3[col_];
    }
  }
  ~Canvas() {}

  char* ToPpmData() {
    char* data = new char[row_ * col_ * 3];
    for (int i = 0; i < row_; i++) {
      for (int j = 0; j < col_; j++) {
        data[(i * col_ + j) * 3]     = canvas_[i][j].r;
        data[(i * col_ + j) * 3 + 1] = canvas_[i][j].g;
        data[(i * col_ + j) * 3 + 2] = canvas_[i][j].b;
      }
    }
    return data;
  }

  Vec3* operator[](int index) { return canvas_[index]; }

 public:
  vector<Vec3*> canvas_;

 private:
  int col_, row_;
};

#endif  // COMMON_CANVAS_H