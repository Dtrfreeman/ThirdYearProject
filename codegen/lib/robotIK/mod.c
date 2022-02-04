/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mod.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

/* Include Files */
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double x
 * Return Type  : double
 */
double b_mod(double x)
{
  double q;
  double r;
  boolean_T rEQ0;
  if (rtIsNaN(x) || rtIsInf(x)) {
    r = rtNaN;
  } else if (x == 0.0) {
    r = 0.0;
  } else {
    r = fmod(x, 6.2831853071795862);
    rEQ0 = (r == 0.0);
    if (!rEQ0) {
      q = fabs(x / 6.2831853071795862);
      rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }

    if (rEQ0) {
      r = 0.0;
    } else {
      if (x < 0.0) {
        r += 6.2831853071795862;
      }
    }
  }

  return r;
}

/*
 * File trailer for mod.c
 *
 * [EOF]
 */
