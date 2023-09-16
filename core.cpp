#include <cassert>
#include <cmath>
#include <iostream>

#include "geo.hpp"
#include "move.hpp"

const Geo::Point Mov::hit(Geo::Particle const& p, Geo::Billiard const& bill) {
  Geo::Line go{p};
  const Geo::Point origin{0., 0.};
  const Geo::Point highsx{0., bill.r1};
  Geo::Line leftborder{origin, highsx};
  const Geo::Point c{intsec(go, leftborder)};
  // assert(std::abs(c.x) <= 0.0001);
  if (std::abs(c.y) <= bill.r1 && c != p.position()) {
    return c;
  }

  const Geo::Point notorigin{bill.l, 0.};
  const Geo::Point highdx{bill.l, bill.r2};
  Geo::Line rightborder{notorigin, highdx};
  const Geo::Point d{intsec(rightborder, go)};
  // assert(std::abs(d.x - bill.l) <= 0.0001);
  if (std::abs(d.y) <= bill.r2) {
    return d;
  }

  if (bill.type == 'l') {
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

  if (bill.type == 'c') {
    // circumference is of type (x-alpha)^2 + (y-beta)^2 = gamma^2

    double alpha{bill.l};
    double beta{(bill.r1 * bill.r1 - bill.r2 * bill.r2 + bill.l * bill.l) /
                (2 * (bill.r1 - bill.r2))};
    double square_gamma{
        std::pow((std::pow((bill.r1 - bill.r2), 2.) + bill.l * bill.l) /
                     (2 * (bill.r1 - bill.r2)),
                 2.)};
    if ((std::abs(p.position().x) <= 0.0001 && go.m() > 0.) ||
        (std::abs(p.position().x) > 0.0001 && p.position().y < 0.)) {
      // the particle hits the upper border in these cases

      double a{go.m() * go.m() + 1};
      double b{-2 * (go.m() * alpha + go.q() + go.m() * beta)};
      double c{std::pow(go.m() * alpha + go.q(), 2.) +
               go.m() * go.m() * beta * beta - go.m() * go.m() * square_gamma};
      bool what{false};
      double sol{Geo::solve_eq(a, b, c, what)};
      // assert(sol >= bill.r2 && sol <= bill.r1);
      const Geo::Point newp{((sol - go.q()) / go.m()), sol};
      // assert(newp.x >= 0. && newp.x <= bill.l);
      return newp;

    } else {
      beta = -beta;
      double a{go.m() * go.m() + 1};
      double b{-2 * (go.m() * alpha + go.q() + go.m() * beta)};
      double c{std::pow(go.m() * alpha + go.q(), 2.) +
               go.m() * go.m() * beta * beta - go.m() * go.m() * square_gamma};
      bool what{true};
      double sol{Geo::solve_eq(a, b, c, what)};
      // assert(sol >= bill.r2 && sol <= bill.r1);
      const Geo::Point newp{((sol - go.q()) / go.m()), sol};
      // assert(newp.x >= 0. && newp.x <= bill.l);
      return newp;
    }
  }
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

  if (bill.type == 'c') {
    double alpha{bill.l};
    double beta{(bill.r1 * bill.r1 - bill.r2 * bill.r2 + bill.l * bill.l) /
                (2 * (bill.r1 - bill.r2))};
    double square_gamma{
        std::pow((std::pow((bill.r1 - bill.r2), 2.) + bill.l * bill.l) /
                     (2 * (bill.r1 - bill.r2)),
                 2.)};

    if (point.y > 0.) {
      double deriv{(point.x - alpha) /
                   sqrt(square_gamma - std::pow((point.x - alpha), 2.))};
      double ord{point.y - deriv * point.x};
      const Geo::Line lin{deriv, ord};
      return lin;
    } else {
      beta = -beta;
      double deriv{-(point.x - alpha) /
                   sqrt(square_gamma - std::pow((point.x - alpha), 2.))};
      double ord{point.y - deriv * point.x};
      const Geo::Line lin{deriv, ord};
      return lin;
    }
  }

  Geo::Line const l{1., 1.};
  return l;
}
