#include <stdio.h>
#include <iostream>
#include <cmath>
#define PI 3.14159265358979323846
#define CTERMS 8

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

double sinSeries(double rad, int nTerms) {
  double k = 0, result = 0;
  while (k <= nTerms) {
    double signal = pow(-1, k);
    double aux1 = pow(rad, (2 * k) + 1);
    double aux2 = factorial((2 * k) + 1);
    result = result + ((signal * aux1) / aux2);
    k++;
  }
  return result;
}

double cosSeries(double rad, int nTerms) {
  double k = 0, result = 0;
  while (k <= nTerms) {
    double signal = pow(-1, k);
    double aux1 = pow(rad, 2 * k);
    double aux2 = factorial(2 * k);
    result = result + ((signal * aux1) / aux2);
    k++;
  }
  return result;
} 

double sin1quad(double rad, int nTerms) {
  double result;
  if (rad <= PI / 4) 
    result = sinSeries(rad, nTerms);
  else 
    result = cosSeries((PI / 2) - rad, nTerms);
  return result;
}

double cos1quad(double rad, int nTerms) {
  double result;
  if (rad <= PI / 4)
    result = cosSeries(rad, nTerms);
  else
    result = sinSeries((PI / 2) - rad, nTerms);
  return result;
}

double sin2quad(double rad, int nTerms) {
  //90 < rad <= 180	
	double result = sin1quad(PI - rad, nTerms);
	return result;
}

double cos2quad(double rad, int nTerms) {
	//90 < rad <= 180
	double result = cos1quad(PI - rad, nTerms);
	return result;
}

int main() {
  double input, x, deg;
	cout << "Input in degrees: ";
  cin >> input;
  while (input > 360)
    input = input - 360;
  deg = input;
  input = (input * PI) / 180;
  if (deg >= 0 && deg <= 90) {
    x = sin1quad(input, CTERMS);
    printf("sin(%.2lf) = %.12lf\n", input, x);
    x = cos1quad(input, CTERMS);
    printf("cos(%.2lf) = %.12lf\n", input, x);
  }
	else if (deg > 90 && deg <= 180) {
		x = sin2quad(input, CTERMS);
    printf("sin(%.2lf) = %.12lf\n", input, x);
		x = cos2quad(input, CTERMS);
		printf("cos(%.2lf) = %.12lf\n", input, x);
	}
  return 0;
 
