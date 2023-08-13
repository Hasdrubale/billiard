#ifndef GEO_HPP
#define GEO_HPP

namespace Geo{

struct Point {
    double x{0.};
    double y{0.};
};

class Particle {
 private:
  Point position_{};
  double angle_{};

 public:
  Particle(Point p, double a);
  Point position() const;
  double angle() const;
  void set_position(Point const&);
  void set_angle(double const);
  void rotate_forward(double const);
  void rotate_backward(double const);
};



}

#endif