#ifndef MOVE_HPP
#define MOVE_HPP

#include "geo.hpp"

namespace Mov {

void move(Geo::Particle&, Geo::Billiard const&);

const Geo::Point hit(Geo::Particle const&, Geo::Billiard const&);

const Geo::Line norm(Geo::Point const&, Geo::Billiard const&);

double find_angle(Geo::Line const& r, Geo::Particle const& p);

}  // namespace Mov

#endif