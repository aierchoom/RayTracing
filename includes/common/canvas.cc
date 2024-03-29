#include "canvas.h"
#include <vector>
#include "vec3.h"

using std::vector;

Canvas::Canvas(int width, int height) : width_(width), height_(height), canvas_()
{
  // allocate all column of memory.
  canvas_.resize(width_);
  // allocate memory for each column.
  for (int i = 0; i < width_; i++) {
    canvas_[i] = vector<Vec3>(height_);
  }
}

Canvas::~Canvas() {}

vector<Vec3>& Canvas::operator[](int x) { return canvas_[x]; }

int Canvas::GetDataSize() const { return width_ * height_ * 3; }

byte* Canvas::ConvertToPpmData()
{
  const int COLOR_TO_BYTE_SIZE = 3;

  byte* data = new byte[width_ * height_ * COLOR_TO_BYTE_SIZE];

  // row scanning.
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      data[(y * width_ + x) * COLOR_TO_BYTE_SIZE]     = canvas_[x][y].r;
      data[(y * width_ + x) * COLOR_TO_BYTE_SIZE + 1] = canvas_[x][y].g;
      data[(y * width_ + x) * COLOR_TO_BYTE_SIZE + 2] = canvas_[x][y].b;
    }
  }

  return data;
}