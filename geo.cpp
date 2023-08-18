#include "geo.hpp"

#include <cassert>
#include <cmath>

Geo::Line::Line(double m, double q) : m_{m}, q_{q} {}

Geo::Line::Line(Geo::Point a, Geo::Point b)
    : m_{(b.y - a.y) / (b.x - a.x)},
      q_{b.y - ((b.y - a.y) / (b.x - a.x)) * b.x} {}

Geo::Line::Line(Geo::Particle p)
    : m_{std::tan(p.angle())},
      q_{p.position().y - (std::tan(p.angle())) * p.position().x} {}

double Geo::Line::m() const { return m_; }
double Geo::Line::q() const { return q_; }
double Geo::Line::angle() const { return std::atan(m_); }
void Geo::Line::set_new(Geo::Particle const& p) {
  Geo::Line l{p};
  m_ = l.m();
  q_ = l.q();
}

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