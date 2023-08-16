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

 public:
  Line(double m, double q);
  Line(Point a, Point b);
  Line(Particle p);
  double m() const;
  double q() const;
  double angle() const;
  void set_new(Particle const&);
};

class Billiard {
 private:
  double r1_;
  double r2_;
  double l_;
  char type_;
  int degree_;

 public:
  Billiard(double r1, double r2, double l, char type, int degree);
  double r1() const;
  double r2() const;
  double l() const;
  char type() const;
  int degree() const;
};

bool operator!=(Point a, Point b);

void move(Particle&, Billiard const&);

}  // namespace Geo



#endif