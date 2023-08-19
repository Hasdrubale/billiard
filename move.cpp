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
    double const angle = Geo::find_angle(perp, go);
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

const Geo::Point Mov::hit(Geo::Line const& go, Geo::Billiard const& bill) {}

const Geo::Line Mov::norm(Geo::Point const& point, Geo::Billiard const& bill) {}
