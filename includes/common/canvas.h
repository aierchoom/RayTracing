#include "Point.h"
#include <vector>
using std::vector;

class Canvas {
 public:
  Canvas(int row, int col) : row_(row), col_(col), canvas_() {
    canvas_.resize(row_);
    for (int i = 0; i < row_; i++) {
      canvas_[i] = new Point[col_];
    }
  }
  ~Canvas() {}

  unsigned char* ToPpmData() {
    unsigned char* data = new unsigned char[row_ * col_ * 3];
    for (int i = 0; i < row_; i++) {
      for (int j = 0; j < col_; j++) {
        data[(i * col_ + j) * 3]     = canvas_[i][j].r;
        data[(i * col_ + j) * 3 + 1] = canvas_[i][j].g;
        data[(i * col_ + j) * 3 + 2] = canvas_[i][j].b;
      }
    }
    return data;
  }

  Point* operator[](int index) { return canvas_[index]; }

 public:
  vector<Point*> canvas_;

 private:
  int col_, row_;
};