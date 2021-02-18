#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;

/*
  is_call = si 1 => on fait un call sinon on fait un put
  num_sims = nombre de simulations
  S = Spot
  K = Strike
  r = Risk-free Rate
  v = Volatility
  T = Maturity
*/
static double monte_carlo_pricing(int is_call, int num_sims, double S, double K, double r, double v, double T) {
  double S_adjust = S * exp(T*(r-0.5*v*v));
  double S_cur = 0.0;
  double payoff_sum = 0.0;
  default_random_engine generator;
  normal_distribution<double> distribution(0.0, 1.0);

  for (int i = 0; i < num_sims; i++)
  {
    double gauss_bm = distribution(generator);
    S_cur = S_adjust * exp(sqrt(v * v * T) * gauss_bm);
    if (is_call)
    {
      payoff_sum += std::max(S_cur - K, 0.0);
    }
    else
    {
      payoff_sum += std::max(K - S_cur, 0.0);
    }
  }
  return (payoff_sum / static_cast<double>(num_sims)) * exp(-r*T);
}

int main(int argc, char* argv[]) {
  int num_sims = 10000000;
  double S = 25.0;
  double K = 15.0;
  double r = 0.05;
  double v = 0.2;
  double T = 1.0;

  // On calcule le call et put en utilisant notre fonction monte_carlo_pricing
  double call = monte_carlo_pricing(1, num_sims, S, K, r, v, T);
  double put = monte_carlo_pricing(0, num_sims, S, K, r, v, T);

  // Affichage des résultats
  cout << "Number of Simulations: " << num_sims << endl;
  cout << "Spot:            " << S << endl;
  cout << "Strike:          " << K << endl;
  cout << "Risk-Free Rate:  " << r << endl;
  cout << "Volatility:      " << v << endl;
  cout << "Maturity:        " << T << endl;
  cout << "Call Price:      " << call << endl;
  cout << "Put Price:       " << put << endl;

	return 0;
}
