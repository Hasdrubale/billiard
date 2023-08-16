#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "geo.hpp"

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