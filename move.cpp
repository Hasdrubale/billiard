#include "move.hpp"

#include <cassert>
#include <cmath>

#include "geo.hpp"

void Mov::move(Geo::Particle& p, Geo::Billiard const& bill) {
  Geo::Line go{p};

  while (true) {
    assert(std::abs(p.position().x) <= bill.l);

    Geo::Point h{Mov::hit(go, bill)};

    if (std::abs(h.x) <= 0.0001 || std::abs(h.x - bill.l) <= 0.0001) {
      p.set_position(h);
      p.set_angle(go.angle());
      break;
    }

    Geo::Line perp{Mov::norm(h, bill)};
    double angle = Mov::find_angle(perp, go);
    p.rotate_forward(angle);
    if (std::abs(p.angle() - std::atan(perp.m())) < 0.0001) {
      p.rotate_forward(angle);
    } else {
      p.rotate_backward(angle);
      p.rotate_backward(angle);
      p.rotate_backward(angle);
    }
    p.set_position(h);
    go.set_new(p);
    continue;
  }
}

double Mov::find_angle(Geo::Line const& r, Geo::Line const& s){
  double r_ang{};
  double s_ang{};
  if (r.m() < 0) {
    r_ang = M_PI + std::atan(r.m());
  } else {
    r_ang = std::atan(r.m());
  }
  if (s.m() < 0) {
    s_ang = M_PI + std::atan(s.m());
  } else {
    s_ang = std::atan(s.m());
  }
  assert(r_ang >= 0 && r_ang <= M_PI);
  assert(s_ang >= 0 && s_ang <= M_PI);
  double ang{std::abs(r_ang - s_ang)};
  assert(ang >= 0 && ang <= M_PI);
  if (ang > M_PI / 2) {
    ang = M_PI - ang;
  }
  return ang;
}

const Geo::Point Mov::hit(Geo::Line const& go, Geo::Billiard const& bill) {}

const Geo::Line Mov::norm(Geo::Point const& point, Geo::Billiard const& bill) {}
