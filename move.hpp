#include "geo.hpp"

namespace Mov {

void move(Geo::Particle&, Geo::Billiard const&);

const Geo::Point hit(Geo::Line const&, Geo::Billiard const&);

const Geo::Line norm(Geo::Point const&, Geo::Billiard const&);

}  // namespace Mov