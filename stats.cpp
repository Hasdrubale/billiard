#include "stats.hpp"

#include <algorithm>
#include <numeric>
#include <cmath>

std::vector<double> Stats::vector_y(std::vector<Geo::Particle> const& particles) {
  std::vector<double> ys;
  ys.resize(particles.size());
  std::transform(particles.begin(), particles.end(), ys.begin(),
                 [](const Geo::Particle p) { return p.position().y; });
  return ys;
}

std::vector<double> Stats::vector_ang(std::vector<Geo::Particle> const& particles) {
  std::vector<double> ys;
  ys.resize(particles.size());
  std::transform(particles.begin(), particles.end(), ys.begin(),
                 [](Geo::Particle p) { return p.angle(); });
  return ys;
}

Stats::Statistics Stats::calc(std::vector<double> const& ys) {
  double const mean{std::accumulate(ys.begin(), ys.end(), 0.) / ys.size()};
  double const sigma{sqrt((std::accumulate(ys.begin(), ys.end(), 0.,
                                           [=](double acc, double i) {
                                             return acc +
                                                    (i - mean) * (i - mean);
                                           }) /
                           (ys.size() - 1)))};
  double const simm{(std::accumulate(ys.begin(), ys.end(), 0.,
                                     [=](double acc, double i) {
                                       return acc + ((i - mean) / sigma) *
                                                        ((i - mean) / sigma) *
                                                        ((i - mean) / sigma);
                                     }) /
                     (ys.size()))};
  double const app{(std::accumulate(ys.begin(), ys.end(), 0.,
                                    [=](double acc, double i) {
                                      return acc + ((i - mean) / sigma) *
                                                       ((i - mean) / sigma) *
                                                       ((i - mean) / sigma) *
                                                       ((i - mean) / sigma);
                                    }) /
                    (ys.size()))};
  Statistics statistics{mean, sigma, simm, app};
  return statistics;
}