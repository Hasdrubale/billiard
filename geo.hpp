#ifndef GEO_HPP
#define GEO_HPP

namespace Geo {

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

class Line {
 private:
  // y=mx+q
  double m_;
  double q_;
  double infinity_;

 public:
  Line(double m, double q, double infinity);
  Line(double m, double q);
  Line(Point a, Point b);
  Line(Particle p);
  double m() const;
  double q() const;
  double angle() const;
  double infinity() const;
  // void set_new(Point& a, Point& b);
};

struct Billiard {
  double r1{0.};
  double r2{0.};
  double l{0.};
  char type{'l'};
  int degree{0};
};

bool operator!=(Point a, Point b);

const Point intsec(Line const& r, Line const& s);

const Line ort(Line const&, Point const&);
}  // namespace Geo

#endif