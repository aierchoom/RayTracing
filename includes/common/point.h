class Point {
 public:
  union {
    struct {
      float x, y, z;
    };
    struct {
      float r, g, b;
    };
  };
};