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
<<<<<<< HEAD
=======
  if (rad == PI / 2) 
    return 1;
  if (rad == (3 * PI / 2)) 
    return -1;
  if ((rad == 0) || (rad == PI))
    return 0;
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
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
<<<<<<< HEAD
=======
  if (rad == 0)
    return 1;
  if (rad == PI) 
    return -1;
  if ((rad == (3 * PI / 2) || rad == PI / 2))
    return 0;
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
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
<<<<<<< HEAD
  //90 < rad <= 180	
	double result;
	if (rad <= (3 * PI) / 4) 
		result = sin1quad(PI - rad, nTerms);
	else 
		result = cos1quad(PI - (PI - rad), nTerms);
	return result;
}

double cos2quad(double rad, int nTerms) {
	//90 < rad <= 180
	double result;
	if (rad <= (3 * PI) / 4)
		result = cos1quad(PI - rad, nTerms);
	else
		result = sin1quad(PI - (PI - rad);
	return result;
=======
  //90 <= rad <= 180
   
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
}

int main() {
  double input, x, deg;
<<<<<<< HEAD
	cout << "Input in degrees: ";
=======
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
  cin >> input;
  while (input > 360)
    input = input - 360;
  deg = input;
  input = (input * PI) / 180;
  if (deg >= 0 && deg <= 90) {
<<<<<<< HEAD
=======
    cout << "HELLo " << endl;
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
    x = sin1quad(input, CTERMS);
    printf("sin(%.2lf) = %.12lf\n", input, x);
    x = cos1quad(input, CTERMS);
    printf("cos(%.2lf) = %.12lf\n", input, x);
  }
<<<<<<< HEAD
	else if (deg > 90 && deg <= 180) {
		x = sin2quad(input, CTERMS);
    printf("sin(%.2lf) = %.12lf\n", input, x);
	}
=======
>>>>>>> f1cc9a42aeab616b99c9625a717a19fd94b87e04
  return 0;
 }
