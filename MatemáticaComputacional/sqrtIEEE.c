#include <stdio.h>
#include <math.h>
#define TOTAL_SIZE 64
#define EXP_SIZE 11
#define MANTISSA_SIZE 54

typedef union {
  float num;
  struct { 
    unsigned long int mantissa : 54;
    unsigned int exponent : 11;
    unsigned int sign : 1;
  } parts;
} floatIEEE754;

void toBin(unsigned int in, int size, unsigned char *out) {
  int i, aux, j = 0;

  for (i = size - 1; i >= 0; i--) {
   aux = in >> i;
    aux = aux & 1;
    if (aux)
      out[j] = '1';

    else
      out[j] = '0';

    j++;
  }
}

int main() {
  floatIEEE754 test;
  test.num = 0.25;
  
  printf("sign = %x\n", test.parts.sign);
  printf("exponent = %x\n", test.parts.exponent);
  printf("mantisa = %x\n", test.parts.mantissa);



  /*unsigned char binaryArray[TOTAL_SIZE];
  float inputNum, mantissa;
  int i, j = 0;

  scanf("%f", &inputNum);

  toBin(inputNum, 16, binaryArray);

  if (inputNum < 0)
    binaryArray[0] = '1';

  else
    binaryArray[0] = '0';

  int exponent;
  mantissa = frexpf(inputNum, &exponent);
  exponent += 1023;

  unsigned char expArray[EXP_SIZE];
  toBin(exponent, EXP_SIZE, expArray);

  for (i = 1; i < 12; i++) {
    binaryArray[i] = expArray[j];
    j++;
  }

  j = 0;
  */

  return 0;
}
