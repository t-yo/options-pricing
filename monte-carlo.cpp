#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;


// A simple implementation of the Box-Muller algorithm, used to generate
// gaussian random numbers - necessary for the Monte Carlo method below
// Note that C++11 actually provides std::normal_distribution<> in
// the <random> library, which can be used instead of this function
double gaussian_box_muller() {
  double x = 0.0;
  double y = 0.0;
  double euclid_sq = 0.0;
  do {
    x = 2.0 * rand() / static_cast<double>(RAND_MAX)-1;
    y = 2.0 * rand() / static_cast<double>(RAND_MAX)-1;
    euclid_sq = x*x + y*y;
  } while (euclid_sq >= 1.0);

  return x*sqrt(-2*log(euclid_sq)/euclid_sq);
}


/*
  is_call = si 1 => on fait un call sinon on fait un put
  num_sims = nombre de simulations
  S = Spot
  K = Strike
  r = Risk-free Rate
  v = Volatility
  T = Maturity
*/
double monte_carlo_pricing(int is_call, int num_sims, double S, double K, double r, double v, double T) {
  double S_T = S * exp(T*(r-0.5*v*v));
  double S_cur = 0.0;
  double payoff = 0.0;

  //simulation de trajectoire
  default_random_engine generator;
  normal_distribution<double> distribution(0.0, 1.0);

  for (int i = 0; i < num_sims; i++)
  {
    double gauss_bm = distribution(generator);
    S_cur = S_T * exp(sqrt(v * v * T) * gauss_bm);
    if (is_call)
    {
      payoff += std::max(S_cur - K, 0.0);
    }
    else
    {
      payoff += std::max(K - S_cur, 0.0);
    }
  }
  // on fait la moyenne et on actualise la date
  return (payoff / num_sims) * exp(-r*T);
}

int main(int argc, char* argv[]) {
  int num_sims = 10000000;
  double S = 100.0;
  double K = 150.0;
  double r = 0.05;
  double v = 0.2;
  double T = 3.0;

  // On calcule le call et put en utilisant notre fonction monte_carlo_pricing
  double call = monte_carlo_pricing(1, num_sims, S, K, r, v, T);
  double put = monte_carlo_pricing(0, num_sims, S, K, r, v, T);

  // Affichage des résultats
  cout << "Pricing d'option utilisant la méthode de Monte Carlo" << endl;
  cout << "" << endl;
  cout << "Paramètres d'entrée du calcul :" << endl;
  cout << "------------------------------" << endl;
  cout << "Nb simulation:   " << num_sims << endl;
  cout << "Spot:            " << S << endl;
  cout << "Strike:          " << K << endl;
  cout << "Risk-Free Rate:  " << r << endl;
  cout << "Volatility:      " << v << endl;
  cout << "Maturity:        " << T << endl;
  cout << "------------------------------" << endl;
  cout << "Résultats : " << endl;
  cout << "Call :      " << call << endl;
  cout << "Put  :       " << put << endl;
  cout << "------------------------------" << endl;
	return 0;
}
