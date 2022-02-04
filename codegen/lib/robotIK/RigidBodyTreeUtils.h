/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: RigidBodyTreeUtils.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

#ifndef RIGIDBODYTREEUTILS_H
#define RIGIDBODYTREEUTILS_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void RigidBodyTreeUtils_distance(const double config1[16], const double
    config2_data[], const int config2_size[2], double dist_data[], int
    dist_size[1]);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for RigidBodyTreeUtils.h
 *
 * [EOF]
 */
