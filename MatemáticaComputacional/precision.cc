#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;

long long unsigned int factorial(int n) {
  int k = n, result = 1;
  if (k == 0)
    return 1;
  while (k != 1) {
    result = result * k;
    k--;
  }
  return result;
}

double taylorSeries(double x, double nTerms) {
  double i = 0;
  double k = 1; 
  while (k <= nTerms) {
    double aux1 = factorial((2 * k) - 2) * pow(x, k);
    double aux2 = factorial(k) * factorial(k - 1) * pow(2, ((2*k) - 1));
    double signal = pow(-1, (k + 1));
    i = i + ((signal * aux1) / aux2);
    k++;
  }
  i = i + 1;
  return i;
}

int main() {
  double x = taylorSeries(0.9, 6);
  cout << "Using Taylor Series, with finite terms." << endl;
  printf("X (7 terms) = %.12lf\n", x);
  x = taylorSeries(0.9, 7);
  printf("X (8 terms) = %.12lf\n", x);
}
