#include "add_wrapper.h"
#include <math.h>
#include <stdio.h>

/*
 *
 * Add two positive floating point numbers as integers
 *
 * Pre-condition : - num1 & num2 are positive finite floats
 *									- num1 +
 *num2 is also a positive finite float Post-condition: num1 + num2
 *
 * Input Validation and Error Codes:
 * 	  -1	==> num1 invalid
 *		-2	==> num2 invalid
 *		-3 	==> both invalid
 *
 * Output Validation
 *		-4	==> sum of inputs invalid
 *
 */
float add_wrapper(float num1, float num2) {
  // Error check (Helper function): Returns true if number is negative/inf/nan
  inline int invalid_float(float num) { return !isfinite(num) || num < 0; }

  // Handle bad inputs according to above specs
  int num1_invalid = invalid_float(num1), num2_invalid = invalid_float(num2);
  if (num1_invalid && num2_invalid) {
    return -3;
  } else if (num2_invalid) {
    return -2;
  } else if (num1_invalid) {
    return -1;
  }

  // At this point, input is validated, call the system call
  int ret_int = syscall(434, *(int *)&num1, *(int *)&num2);
  // Validate result
  if (invalid_float(*(float *)&ret_int)) {
    return -4;
  } else {
    return *(float *)&ret_int;
  }
}
// float add_wrapper(float num1, float num2) {
//   // Store the arguments as int
//   int temp1 = *(int *)&num1, temp2 = *(int *)&num2;

//   // Error check: Returns true if number is negative / inf / nan
//   inline int invalid_float(int num) {
//     return num & SIG_MASK || ((num & EXP_MASK) == EXP_MASK);
//   }

//   // Handle bad inputs according to above specs
//   int num1_inval = invalid_float(num1), num2_inval = invalid_float(num2);
//   if (num1_inval && num2_inval) {
//     return -3;
//   } else if (num2_inval) {
//     return -2;
//   } else if (num1_inval) {
//     return -1;
//   }

//   // At this point, input is validated, call the system call
//   int ret_int = syscall(434, temp1, temp2);

//   // Validation of result
//   if (invalid_float(ret_int)) {
//     return -4;
//   } else {
//     return *(float *)&ret_int;
//   }
// }