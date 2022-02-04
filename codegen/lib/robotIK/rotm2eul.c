/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: rotm2eul.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

/* Include Files */
#include "rotm2eul.h"
#include "robotIK_rtwutil.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double R[9]
 *                double eul[3]
 * Return Type  : void
 */
void rotm2eul(const double R[9], double eul[3])
{
  double r_data[1];
  double y_data[1];
  double eul_tmp;
  double sy;
  sy = sqrt(R[5] * R[5] + R[2] * R[2]);
  eul[0] = rt_atan2d_snf(R[5], R[2]);
  eul_tmp = rt_atan2d_snf(sy, R[8]);
  eul[1] = eul_tmp;
  eul[2] = rt_atan2d_snf(R[7], -R[6]);
  if (sy < 2.2204460492503131E-15) {
    y_data[0] = -R[1];
    r_data[0] = rt_atan2d_snf(y_data[0], R[4]);
    y_data[0] = eul_tmp;
    eul[0] = r_data[0];
    eul[1] = y_data[0];
    eul[2] = 0.0;
  }

  eul[0] = -eul[0];
  eul[1] = -eul[1];
  eul[2] = -eul[2];
  sy = eul[0];
  eul[0] = eul[2];
  eul[2] = sy;
}

/*
 * File trailer for rotm2eul.c
 *
 * [EOF]
 */
