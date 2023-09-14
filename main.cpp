#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

#include "move.hpp"
#include "stats.hpp"

int main() {
  Geo::Billiard bill{};
  double mean_y{};
  double sigma_y{};
  double mean_ang{};
  double sigma_ang{};

  std::ifstream in_file{"input.txt"};
  std::ofstream out_init{"outinit.txt"};
  std::ofstream out_fin{"outfin.txt"};
  std::vector<Geo::Particle> input{};
  std::vector<Geo::Particle> output{};

  /*Nel file input.txt inserire i parametri del biliardo e delle distribuzioni
  gaussiane nel seguente ordine:
  r1
  r2
  l
  tipo parete
  grado del polinomio (0 per biliardo lineare, circolare o parabolico)
  media y iniziale
  media angolo iniziale
  media sigma iniziale
  media sigma finale
  */

  in_file >> bill.r1;
  in_file >> bill.r2;
  assert(bill.r2 > 0. && bill.r2 < bill.r1);
  in_file >> bill.l;
  in_file >> bill.type;
  assert(bill.type == 'l' || bill.type == 'c' || bill.type == 'p' ||
         bill.type == 'd');
  in_file >> bill.degree;
  if (bill.type != 'd') {
    assert(bill.degree == 0);
  }
  in_file >> mean_y;
  assert(mean_y < bill.r1);
  in_file >> sigma_y;
  in_file >> mean_ang;
  assert(mean_ang > -M_PI / 2 && mean_ang < M_PI / 2);
  in_file >> sigma_ang;

  std::cout << "Comandi ammessi:\nm: inserisce manualmente una "
               "particella\na: genera n particelle distribuite "
               "gaussianamente, con i parametri letti da input.txt"
            << "(inserire il numero n dopo il comando)\ns: calcola le "
               "statistiche\nq: esce dal programma\n";

  while (true) {
    char command{};
    std::cout << "Comando: ";
    std::cin >> command;
    assert(command == 'm' || command == 'a' || command == 'q' ||
           command == 's');

    if (command == 'm') {
      double ang{};
      double y{};
      Geo::Billiard billiard{};

      std::cout << "Inserire r1: ";
      std::cin >> billiard.r1;
      std::cout << "Inserire r2: ";
      std::cin >> billiard.r2;
      assert(billiard.r2 > 0 && billiard.r2 < billiard.r1);
      std::cout << "Inserire l: ";
      std::cin >> billiard.l;
      std::cout << "Inserire tipo bordo: ";
      std::cin >> billiard.type;
      assert(billiard.type == 'l' || billiard.type == 'c' ||
             billiard.type == 'p' || billiard.type == 'd');
      if (billiard.type == 'd') {
        std::cout << "Inserire grado polinomio: ";
        std::cin >> billiard.degree;
      }
      std::cout << "Inserire y0: ";
      std::cin >> y;
      assert(abs(y) < billiard.r1);
      std::cout << "Inserire angolo di partenza (in gradi): ";
      std::cin >> ang;
      ang = (ang / 360.) * 2 * M_PI;
      assert(ang < M_PI / 2 && ang > -M_PI / 2);

      Geo::Point pos{0, y};
      Geo::Particle p{pos, ang};
      Mov::move(p, billiard);

      if (std::abs(p.position().x - billiard.l) < 0.0001) {
        std::cout << "Y finale: " << p.position().y << "\n"
                  << "Angolo (radianti):" << p.angle() << "\n"
                  << "Angolo (gradi): " << ((p.angle() / (2 * M_PI)) * 360)
                  << "\n";
      } else {
        std::cout << "Particella non uscita dal biliardo\n";
      }
      continue;
    }

    if (command == 'a') {
      int n{};
      int exit{0};
      std::cin >> n;
      assert(std::cin.good() == true);

      std::random_device r;
      std::default_random_engine eng{r()};

      Geo::Point def{0., 0.};
      Geo::Particle defaultp{def, 0.};
      std::vector<Geo::Particle> particles{};
      particles.resize(n, defaultp);
      std::generate_n(particles.begin(), n, [&]() {
        std::normal_distribution<double> dist_y{mean_y, sigma_y};
        std::normal_distribution<double> dist_ang{mean_ang, sigma_ang};
        Geo::Point pos{0., dist_y(eng)};
        Geo::Particle p{pos, dist_ang(eng)};

        while (p.angle() <= -M_PI / 2. || p.angle() >= M_PI / 2.) {
          p.set_angle(dist_ang(eng));
        }

        while (p.position().y >= bill.r1 || p.position().y <= -bill.r1) {
          pos.y = dist_y(eng);
          p.set_position(pos);
        }
        return p;
      });

      for (Geo::Particle& p : particles) {
        out_init << p.position().x << " " << p.position().y << " " << p.angle()
                 << "\n";
        input.push_back(p);
        Mov::move(p, bill);

        // se la particella esce dal bordo destro viene salvata in outfin.txt,
        // se esce dal bordo sinistro viene contata come particella non uscita
        // dal biliardo

        if (std::abs(p.position().x - bill.l) < 0.00001) {
          out_fin << p.position().x << " " << p.position().y << " " << p.angle()
                  << "\n";
          output.push_back(p);
        } else {
          ++exit;
        }
      }

      std::cout << exit << " particelle non sono uscite dal biliardo\n";
      continue;
    }

    if (command == 's') {
      Stats::Statistics y{Stats::calc(Stats::vector_y(input))};
      Stats::Statistics ang{Stats::calc(Stats::vector_ang(input))};

      std::cout << "Posizione y iniziale:\nMedia: " << y.mean
                << "\nDeviazione standard: " << y.sigma
                << "\nCoefficiente di simmetria: " << y.simm;
      if (y.simm > 0.01) {
        std::cout << " (distribuzione asimmetrica positiva)\n";
      }
      if (y.simm < -0.01) {
        std::cout << " (distribuzione asimmetrica negativa)\n";
      }
      if (y.simm >= -0.01 && y.simm <= 0.01) {
        std::cout << " (distribuzione simmetrica)\n";
      }
      std::cout << "Coefficiente di appiattimento: " << y.app;
      if (y.app > 3.01) {
        std::cout << " (distribuzione leptocurtica)\n";
      }
      if (y.app < 2.99) {
        std::cout << " (distribuzione platicurtica)\n";
      }
      if (y.app >= 2.99 && y.app <= 3.01) {
        std::cout << " (distribuzione mesocurtica)\n";
      }

      std::cout << "\nAngolo iniziale\nMedia: " << ang.mean
                << "\nDeviazione standard: " << ang.sigma
                << "\nCoefficiente di simmetria: " << ang.simm;
      if (ang.simm > 0.01) {
        std::cout << " (distribuzione asimmetrica positiva)\n";
      }
      if (ang.simm < -0.01) {
        std::cout << " (distribuzione asimmetrica negativa)\n";
      }
      if (ang.simm >= -0.01 && ang.simm <= 0.01) {
        std::cout << " (distribuzione simmetrica)\n";
      }
      std::cout << "Coefficiente di appiattimento: " << ang.app;
      if (ang.app > 3.01) {
        std::cout << " (distribuzione leptocurtica)\n";
      }
      if (ang.app < 2.99) {
        std::cout << " (distribuzione platicurtica)\n";
      }
      if (ang.app >= 2.99 && ang.app <= 3.01) {
        std::cout << " (distribuzione mesocurtica)\n";
      }
      std::cout << "\n";

      Stats::Statistics yf{Stats::calc(Stats::vector_y(output))};
      Stats::Statistics angf{Stats::calc(Stats::vector_ang(output))};

      std::cout << "Posizione y finale:\nMedia: " << yf.mean
                << "\nDeviazione standard: " << yf.sigma
                << "\nCoefficiente di simmetria: " << yf.simm;
      if (yf.simm > 0.01) {
        std::cout << " (distribuzione asimmetrica positiva)\n";
      }
      if (yf.simm < -0.01) {
        std::cout << " (distribuzione asimmetrica negativa)\n";
      }
      if (yf.simm >= -0.01 && yf.simm <= 0.01) {
        std::cout << " (distribuzione simmetrica)\n";
      }
      std::cout << "Coefficiente di appiattimento: " << yf.app;
      if (yf.app > 3.01) {
        std::cout << " (distribuzione leptocurtica)\n";
      }
      if (yf.app < 2.99) {
        std::cout << " (distribuzione platicurtica)\n";
      }
      if (yf.app >= 2.99 && yf.app <= 3.01) {
        std::cout << " (distribuzione mesocurtica)\n";
      }

      std::cout << "\nAngolo finale\nMedia: " << angf.mean
                << "\nDeviazione standard: " << angf.sigma
                << "\nCoefficiente di simmetria: " << angf.simm;
      if (angf.simm > 0.01) {
        std::cout << " (distribuzione asimmetrica positiva)\n";
      }
      if (angf.simm < -0.01) {
        std::cout << " (distribuzione asimmetrica negativa)\n";
      }
      if (angf.simm >= -0.01 && angf.simm <= 0.01) {
        std::cout << " (distribuzione simmetrica)\n";
      }
      std::cout << "Coefficiente di appiattimento: " << angf.app;
      if (angf.app > 3.01) {
        std::cout << " (distribuzione leptocurtica)\n";
      }
      if (angf.app < 2.99) {
        std::cout << " (distribuzione platicurtica)\n";
      }
      if (angf.app >= 2.99 && angf.app <= 3.01) {
        std::cout << " (distribuzione mesocurtica)\n";
      }
      std::cout << "\n";
      continue;
    }

    if (command == 'q') {
      break;
    }
  }
}