#include <stdio.h>

int main() {
  //Euler method
  double h1 = 1, h2 = 0.5;
  int n1 = 5, n2 = 10;
  

  int k;
  printf("f(t,v) = (2000 - 2 * v) / (200 - t)\n");
  printf("Using Euler for v(5), and h = 1.0\n");
  printf("v(0) = 0, t0 = 0\n");
  double v = 0, t = 0;

  for (k = 0; k < n1; k++) {
    printf("Iteration %i: ", k);
    v = v + (h1 * ((2000 - 2 * v) / 200 - t));
    t = t + h1;
    printf("v(%.2f) = %.6f\n", t, v);
  }
  
  printf("\n===============================================\n");
  printf("f(t,v) = (2000 - 2 * v) / (200 - t)\n");
  printf("Using Euler for v(5), and h = 0.5\n");
  printf("v(0) = 0, t0 = 0\n");
  v = 0, t = 0;

  for (k = 0 ; k < n2; k++) {
    printf("Iteration %i: ", k);
    v = v + (h2 * ((2000 - 2 * v) / 200 - t));
    t = t + h2;
    printf("v(%.2f) = %.6f\n", t, v);
  }

  printf("\n===============================================\n");

  v = 0, t = 0;
  printf("Using Runge-Kutta 4th order method.\n");
  for (k = 0; k < n1; k++) {
    printf("Iteration %i: ", k);
    double k1 = (2000 - 2 * v) / (200 - t);
    double k2 = (2000 - 2 * (v + (h1 * k1 / 2))) / (200 - (t + (h1 / 2)));
    double k3 = (2000 - 2 * (v + (h1 * k2 / 2))) / (200 - (t + (h1 / 2))); 
    double k4 = (2000 - 2 * (v + (h1 * k3))) / (200 - (t + h1));
    v = v + ((h1 / 6) * (k1 + (2 * k2) + (2 * k3) + k4));
    t = t + h1;
    printf("v(%.2f) = %.6f\n", t, v);
  }
}
