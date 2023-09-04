#include "geo.hpp"

#include <cassert>
#include <cmath>

Geo::Line::Line(double m, double q) : a_{m}, b_{-1.}, c_{q} {}

Geo::Line::Line(Geo::Point a, Geo::Point b)
    : a_{(b.y - a.y)}, b_{(a.x - b.x)}, c_{a.y * b.x - b.y * a.x} {}

Geo::Line::Line(Geo::Particle p)
    : a_{std::sin(p.angle())},
      b_{-std::cos(p.angle())},
      c_{std::cos(p.angle()) * p.position().y -
         std::sin(p.angle()) * p.position().x} {}

double Geo::Line::a() const { return a_; }
double Geo::Line::b() const { return b_; }
double Geo::Line::c() const { return c_; }
double Geo::Line::m() const { return -(a_ / b_); }
double Geo::Line::q() const { return -(c_ / b_); }
double Geo::Line::angle() const {
  if (b_ == 0.) {
    return M_PI / 2;
  } else {
    return std::atan(-a_ / b_);
  }
}

/*void Geo::Line::set_new(Geo::Point& a, Geo::Point& b) {
  Geo::Line l{a, b};
  m_ = l.m();
  q_ = l.q();
}*/

Geo::Particle::Particle(Geo::Point p, double a) : position_{p}, angle_{a} {}

Geo::Point Geo::Particle::position() const { return position_; }
double Geo::Particle::angle() const { return angle_; }

void Geo::Particle::set_position(Geo::Point const& p) { position_ = p; }

void Geo::Particle::set_angle(double const r) { angle_ = r; }

void Geo::Particle::rotate_backward(double const angle) {
  assert(angle <= M_PI / 2 && angle >= 0);
  double a{angle_};
  a -= angle;
  if (a < -M_PI / 2) {
    a += M_PI;
  }
  angle_ = a;
  assert(std::abs(angle_) <= M_PI / 2);
}

void Geo::Particle::rotate_forward(double const angle) {
  assert(angle <= M_PI / 2 && angle >= 0);
  double a{angle_};
  a += angle;
  if (a > M_PI / 2) {
    a -= M_PI;
  }
  angle_ = a;
  assert(std::abs(angle_) <= M_PI / 2);
}

bool Geo::operator!=(Geo::Point a, Geo::Point b) {
  return std::abs((a.x - b.x)) > 0.001 || std::abs((a.y - b.y)) > 0.001;
}

const Geo::Point Geo::intsec(Geo::Line const& r, Geo::Line const& s) {
  double const det{r.a() * s.b() - r.b() * s.a()};
  Geo::Point const p{(r.b() * s.c() - s.b() * r.c()) / (det),
                     (s.a() * r.c() - r.a() * s.c()) / (det)};
  return p;
}

const Geo::Line Geo::ort(Geo::Line const& r, Geo::Point const& point) {
  double new_angle{};
  if (r.angle() >= 0) {
    new_angle = -((M_PI / 2) - r.angle());
  } else {
    new_angle = (M_PI / 2) + r.angle();
  }
  Geo::Particle const k{point, new_angle};
  Geo::Line const l{k};
  return l;
}