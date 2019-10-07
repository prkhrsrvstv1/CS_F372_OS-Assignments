#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "add_wrapper.h"
#define INT_MAX 0x7fffffff
#define EXP_MASK 0x7f800000
#define SIG_MASK 0x80000000
#define MAN_MASK 0x007fffff

int func(int num1, int num2) {
  int num1_exp = num1 & EXP_MASK, num2_exp = num2 & EXP_MASK;

  // If num1_exp < num2_exp, swap them
  if (num1_exp < num2_exp) {
    num1_exp += num2_exp;
    num2_exp = num1_exp - num2_exp;
    num1_exp -= num2_exp;
    num1 += num2;
    num2 = num1 - num2;
    num1 -= num2;
  }
  int num1_man = num1 & MAN_MASK, num2_man = num2 & MAN_MASK;

  // Add a 1-bit to left of 23-bit mantissa
  num1 = num1_man | 0x00800000, num2 = num2_man | 0x00800000;
  // Align the numbers according to their decimal points
  num2 = num2 >> ((num1_exp - num2_exp) >> 23);
  // Add num2 to num1;
  num1 += num2;

  // If there is overflow to 25th bit, shift right and increase exponent
  if (num1 & 0x01000000) {
    num1 = num1 >> 1;
    num1_exp += 0x00800000;
  }
  // Retain only the mantissa
  num1 &= MAN_MASK;
  // Add exponent to num1
  num1 |= num1_exp;
  return num1;
}

int main() {
  /* Testing add_wrapper() wrapper that calls custom syscall_add_syscall() */

  float *num1 = (float *)malloc(sizeof(float)),
        *num2 = (float *)malloc(sizeof(float)), num3;
  int temp1, temp2;

  // Case 0: Both parameters valid.
  printf("\n\nCase 0: Both parameters valid.\n");
  *num1 = 2.5, *num2 = 3.25;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 1: Both parameters valid.
  printf("\n\nCase 1: Both parameters valid.\n");
  *num1 = 6.46, *num2 = 8.972;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 2: Both parameters valid.
  printf("\n\nCase 2: Both parameters valid.\n");
  *num1 = 523.549, *num2 = 658.743;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 3: Both parameters valid.
  printf("\n\nCase 3: Both parameters valid.\n");
  *num1 = 2378.235, *num2 = 65.874;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 4: num1 negative.
  printf("\n\nCase 4: num1 negative.\n");
  *num1 = -237.878, *num2 = 6587.43;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 5: num1 and num2 negative.
  printf("\n\nCase 5: num1 and num2 negative.\n");
  *num1 = -237.8781, *num2 = -6587.43;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 5: num1 nan.
  printf("\n\nCase 5: num1 nan.\n");
  temp1 = 0x7ff00000;
  num1 = (float *)&temp1, *num2 = 658.456;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 6: num2 inf.
  printf("\n\nCase 6: num2 inf.\n");
  temp2 = 0x7f800000;
  *num1 = 723.1549, num2 = (float *)&temp2;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 7: num1 inf, num2 nan.
  printf("\n\nCase 7: num1 inf, num2 nan.\n");
  temp1 = 0x7f800000, temp2 = 0x7ff00000;
  num1 = (float *)&temp1, num2 = (float *)&temp2;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 8: num1 valid, num2 valid, sum invalid.
  printf("\n\nCase 8: num1 valid, num2 valid, sum inf.\n");
  temp1 = 0x7f400000, temp2 = 0x7f600000;
  num1 = (float *)&temp1, num2 = (float *)&temp2;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 9: num1 inf, num2 nan.
  printf("\n\nCase 9: num1 inf, num2 nan.\n");
  temp1 = 0x7f800000, temp2 = 0x7ff00000;
  num1 = (float *)&temp1, num2 = (float *)&temp2;
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  // Case 10: Custom test case
  printf(
      "Enter 2 space separated floats for custom testing (enter both as "
      "negative to quit) : ");
  scanf("%f %f", num1, num2);
  printf("\t%f\n+\t%f\n=\t%f\n", *num1, *num2, add_wrapper(*num1, *num2));
  printf("\nUsing '+' : %f\n\n\n", *num1 + *num2);

  return 0;
}
