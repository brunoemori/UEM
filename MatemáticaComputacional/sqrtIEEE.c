#include <stdio.h>
#include <math.h>
#define EXP_SIZE 11
#define MANTISSA_SIZE 54

typedef union {
  double num;
  struct { 
    unsigned long int mantissa : 52;
    unsigned int exponent : 11;
    unsigned int sign : 1;
  } parts;
} floatIEEE754;

void binaryExponent(unsigned int in, int size, unsigned char *out) {
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

void binaryMantissa(unsigned long int in, int size, unsigned char *out) {
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
  unsigned char sign, exponentArray[EXP_SIZE], mantissaArray[MANTISSA_SIZE];
  floatIEEE754 test;
  double inputNum;

  int i, j = 0;

  scanf("%lf", &inputNum);
  test.num = inputNum; 

  binaryExponent(test.parts.exponent, EXP_SIZE, exponentArray);
  binaryMantissa(test.parts.mantissa, MANTISSA_SIZE, mantissaArray);

  printf("Sign: %i \n", test.parts.sign);

  printf("Exponent: ");
  for (i = 0; i < EXP_SIZE; i++) 
    printf("%c ", exponentArray[i]);
  printf("\n");

  printf("Mantissa: ");
  for (i = 0; i < MANTISSA_SIZE; i++) 
    printf("%c ", mantissaArray[i]);
  printf("\n\n");
  
  printf("Sign = %i\n", test.parts.sign);
  printf("Exponent = 0x%x\n", test.parts.exponent);
  printf("Mantissa = 0x%x\n", test.parts.mantissa);

  printf("\n\n");

/*
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
