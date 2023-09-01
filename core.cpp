#include <cmath>

#include "geo.hpp"
#include "move.hpp"

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
