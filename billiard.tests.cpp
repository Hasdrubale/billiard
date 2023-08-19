#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>

#include "doctest.h"
#include "geo.hpp"
#include "move.hpp"

TEST_CASE("Line constructors") {
  Geo::Point a{1., 2.};
  Geo::Point b{3., -5.};
  Geo::Line r{a, b};
  CHECK(r.m() == doctest::Approx(-3.5));
  CHECK(r.q() == doctest::Approx(5.5));
  CHECK(r.angle() == doctest::Approx(-1.2925));
  Geo::Particle p{a, M_PI / 4};
  Geo::Line s{p};
  CHECK(s.m() == doctest::Approx(1.));
  CHECK(s.q() == doctest::Approx(1.));
  CHECK(s.angle() == doctest::Approx(M_PI / 4));
}

TEST_CASE("Testing motion with circular billiard") {
  SUBCASE("Test 1") {
    Geo::Point pos{0., 1.};
    Geo::Particle p{pos, M_PI / 4};
    Geo::Billiard bill{5., 3., 10., 'c', 0};
    Mov::move(p, bill);
    CHECK(p.angle() == doctest::Approx(0.9036));
    CHECK(p.position().x == doctest::Approx(0.));
    CHECK(p.position().y == doctest::Approx(-0.701));
  }
}

TEST_CASE("Testing motion with linear billiard") {
  SUBCASE("Test 1") {
    Geo::Point p0{0., 4.};
    Geo::Particle p{p0, M_PI / 4};
    Geo::Billiard bill{5., 3., 10., 'l', 0};
    Mov::move(p, bill);
    CHECK(p.position().x == doctest::Approx(0.));
    CHECK(p.position().y == doctest::Approx(-4.17528));
    CHECK(p.angle() == doctest::Approx(-44.52 * 2 * M_PI / 360));
  }
  SUBCASE("Test 2") {
    Geo::Point p0{0., 2.};
    Geo::Particle p{p0, -M_PI / 6};
    Geo::Billiard bill{5., 3., 6., 'l', 0};
    Mov::move(p, bill);
    CHECK(p.position().x == doctest::Approx(6.));
    CHECK(p.position().y == doctest::Approx(-1.4641));
    CHECK(p.angle() == doctest::Approx(-M_PI / 6));
  }
  SUBCASE("Test 3") {
    Geo::Point p0{0., -1.};
    Geo::Particle p{p0, M_PI / 3};
    Geo::Billiard bill{7., 4., 14., 'l', 0};
    Mov::move(p, bill);
    CHECK(p.position().x == doctest::Approx(0.));
    CHECK(p.position().y == doctest::Approx(5.51714));
    CHECK(p.angle() == doctest::Approx(0.82784));
  }
}