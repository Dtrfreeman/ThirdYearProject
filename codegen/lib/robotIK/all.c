/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: all.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

/* Include Files */
#include "all.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const boolean_T x_data[]
 *                const int x_size[2]
 *                boolean_T y_data[]
 *                int y_size[1]
 * Return Type  : void
 */
void all(const boolean_T x_data[], const int x_size[2], boolean_T y_data[], int
         y_size[1])
{
  int i1;
  int i2;
  int ix;
  int iy;
  int j;
  int vstride;
  boolean_T exitg1;
  y_size[0] = (signed char)x_size[0];
  vstride = (signed char)x_size[0];
  for (i2 = 0; i2 < vstride; i2++) {
    y_data[i2] = true;
  }

  vstride = x_size[0];
  i2 = (x_size[1] - 1) * x_size[0];
  iy = -1;
  i1 = 0;
  for (j = 0; j < vstride; j++) {
    i1++;
    i2++;
    iy++;
    ix = i1;
    exitg1 = false;
    while ((!exitg1) && ((vstride > 0) && (ix <= i2))) {
      if (!x_data[ix - 1]) {
        y_data[iy] = false;
        exitg1 = true;
      } else {
        ix += vstride;
      }
    }
  }
}

/*
 * File trailer for all.c
 *
 * [EOF]
 */
