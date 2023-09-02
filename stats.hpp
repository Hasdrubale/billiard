#ifndef STATS_HPP
#define STATS_HPP

#include <vector>

#include "geo.hpp"
namespace Stats {

struct Statistics {
  double const mean{};
  double const sigma{};
  double const simm{};
  double const app{};
};

std::vector<double> vector_y(std::vector<Geo::Particle> const& particles);
std::vector<double> vector_ang(std::vector<Geo::Particle> const& particles);

Statistics calc(std::vector<double> const& ys);

}  // namespace Stats

#endif