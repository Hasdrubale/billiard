#include "move.hpp"

#include <cassert>
#include <cmath>

#include "geo.hpp"

void Mov::move(Geo::Particle& p, Geo::Billiard const& bill) {
  while (true) {
    assert(std::abs(p.position().x) <= bill.l);

    const Geo::Point h{Mov::hit(p, bill)};

    if (std::abs(h.x) <= 0.0001 || std::abs(h.x - bill.l) <= 0.0001) {
      p.set_position(h);
      break;
    }

    const Geo::Line perp{Mov::norm(h, bill)};
    double angle = Mov::find_angle(perp, p);
    p.rotate_forward(angle);
    if (std::abs(p.angle() - std::atan(perp.m())) < 0.0001) {
      p.rotate_forward(angle);
    } else {
      p.rotate_backward(angle);
      p.rotate_backward(angle);
      p.rotate_backward(angle);
    }
    p.set_position(h);
  }
}

double Mov::find_angle(Geo::Line const& r, Geo::Particle const& p) {
  double r_ang{};
  double s_ang{p.angle()};
  if (r.m() < 0) {
    r_ang = M_PI + std::atan(r.m());
  } else {
    r_ang = std::atan(r.m());
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

const Geo::Point Mov::hit(Geo::Particle const& p, Geo::Billiard const& bill) {
  if (bill.type == 'l') {
    Geo::Point highsx{0., bill.r1};
    Geo::Point highdx{bill.l, bill.r2};
    Geo::Line go{p};

    Geo::Line upborder{highsx, highdx};
    const Geo::Point a{intsec(go, upborder)};
    if (std::abs(a.x) >= 0 && std::abs(a.x) <= bill.l &&
        std::abs(a.y) >= bill.r2 && std::abs(a.y) <= bill.r1 &&
        a != p.position()) {
      return a;
    }

    Geo::Point lowsx{0., -bill.r1};
    Geo::Point lowdx{bill.l, -bill.r2};

    Geo::Line downborder{lowsx, lowdx};
    const Geo::Point b{intsec(go, downborder)};
    if (std::abs(b.x) >= 0 && std::abs(b.x) <= bill.l &&
        std::abs(b.y) >= bill.r2 && std::abs(b.y) <= bill.r1 &&
        b != p.position()) {
      return b;
    }

    Geo::Line leftborder{lowsx, highsx};
    const Geo::Point c{intsec(go, leftborder)};
    if (std::abs(c.y) <= bill.r1) {
      return c;
    }

    Geo::Line rightborder{lowdx, highdx};
    const Geo::Point d{intsec(go, rightborder)};
    if (std::abs(d.y) <= bill.r1) {
      return d;
    }
  }

  const Geo::Point origin{0., 0.};
  return origin;
}

const Geo::Line Mov::norm(Geo::Point const& point, Geo::Billiard const& bill) {
  const Geo::Point x{bill.r1, bill.r2};
  const Geo::Line l{point, x};
  return l;
}
