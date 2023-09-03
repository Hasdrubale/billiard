#include <cassert>
#include <cmath>

#include "geo.hpp"
#include "move.hpp"

const Geo::Point Mov::hit(Geo::Particle const& p, Geo::Billiard const& bill) {
  Geo::Line go{p};
  Geo::Line leftborder{99999999999., 0.};
  const Geo::Point c{intsec(go, leftborder)};
  assert(std::abs(c.x) <= 0.0000001);
  if (std::abs(c.y) <= bill.r1 && c != p.position()) {
    return c;
  }

  Geo::Line rightborder{0., 0., bill.l};
  const Geo::Point d{intsec(rightborder, go)};
  assert(std::abs(d.x - bill.l) <= 0.0001);
  if (std::abs(d.y) <= bill.r2) {
    return d;
  }

  if (bill.type == 'l') {
    Geo::Point highsx{0., bill.r1};
    Geo::Point highdx{bill.l, bill.r2};

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
  }

  const Geo::Point origin{0., 0.};
  return origin;
}

const Geo::Line Mov::norm(Geo::Point const& point, Geo::Billiard const& bill) {
  if (bill.type == 'l') {
    if (point.y > 0) {
      Geo::Point a{0., bill.r1};
      Geo::Point b{bill.l, bill.r2};
      Geo::Line r{a, b};
      const Geo::Line l{Geo::ort(r, point)};
      return l;
    } else {
      Geo::Point a{0., -bill.r1};
      Geo::Point b{bill.l, -bill.r2};
      Geo::Line r{a, b};
      const Geo::Line l{Geo::ort(r, point)};
      return l;
    }
  }
  Geo::Line const l{1., 1.};
  return l;
}
