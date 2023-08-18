#include "geo.hpp"

void Geo::move(Geo::Particle& p, Geo::Billiard const& bill) {
  Geo::Point h{0., bill.r1};
  Geo::Point k{bill.l, bill.r2};
  Geo::Point i{0., 0.};
  Geo::Point j{bill.l, 0.};

  Geo::Line const rightborder{k, j};
  Geo::Line const leftborder{i, h};

  Geo::Line go{p}; //da finire

  while (true) {
    assert(std::abs(p.position().x) <= l_);
    h = Ric::intsec(go, upborder);
    i = Ric::intsec(go, downborder);
    j = Ric::intsec(go, leftborder);
    k = Ric::intsec(go, rightborder);

    if (std::abs(j.y) <= r1_ && j != p.position()) {
      p.set_position(j);
      p.set_angle(go.angle());
      go.set_new(p);
      break;
    }

    if (std::abs(k.y) <= r2_) {
      p.set_position(k);
      p.set_angle(go.angle());
      break;
    }

    if (std::abs(i.x) >= 0 && std::abs(i.x) <= l_ && std::abs(i.y) >= r2_ &&
        std::abs(i.y) <= r1_ && i != p.position()) {
      double const angle = Ric::find_angle(down_perp, go);
      p.rotate_forward(angle);
      if (std::abs(p.angle() - std::atan(down_perp.m())) < 0.0001) {
        p.rotate_forward(angle);
      } else {
        p.rotate_backward(angle);
        p.rotate_backward(angle);
        p.rotate_backward(angle);
      }
      p.set_position(i);
      go.set_new(p);
      continue;
    }

    if (std::abs(h.x) >= 0 && std::abs(h.x) <= l_ && std::abs(h.y) >= r2_ &&
        std::abs(h.y) <= r1_ && h != p.position()) {
      double const angle = Ric::find_angle(up_perp, go);
      p.rotate_forward(angle);
      if (std::abs(p.angle() - std::atan(up_perp.m())) < 0.0001) {
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
}