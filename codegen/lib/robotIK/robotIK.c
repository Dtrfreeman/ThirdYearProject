/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: robotIK.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

/* Include Files */
#include "robotIK.h"
#include "RigidBodyTreeUtils.h"
#include "all.h"
#include "asin.h"
#include "mod.h"
#include "robotIK_rtwutil.h"
#include "rotm2eul.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sqrt.h"
#include "unique.h"
#include "wrapToPi.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void applyJointLimits(const double inputConfig[3], const double
  jointLimits[6], double validConfig[3]);
static double rt_roundd_snf(double u);
static void solveFirstThreeDHJoints(const double jt5Pos[3], double
  outputThetas_data[], int outputThetas_size[2]);
static void solveTrigEquations(double a, double b, double c, double theta[2]);

/* Function Definitions */
/*
 * applyJointLimits Convert solutions with invalid joint limits to NaNs
 *    Given an N-element configuration, an Nx2 set of lower and upper joint
 *    limits, and an N-element vector indicating the joint type (revolute or
 *    prismatic), this function checks whether the configuration is within
 *    the joint limits. If not, the configuration is converted to NaNs.
 * Arguments    : const double inputConfig[3]
 *                const double jointLimits[6]
 *                double validConfig[3]
 * Return Type  : void
 */
static void applyJointLimits(const double inputConfig[3], const double
  jointLimits[6], double validConfig[3])
{
  double theta;
  double wrappedJointValueOffset;
  int i;
  boolean_T exitg1;

  /*    Copyright 2020 The MathWorks, Inc. */
  /*  Initialize output */
  validConfig[0] = inputConfig[0];
  validConfig[1] = inputConfig[1];
  validConfig[2] = inputConfig[2];
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 3)) {
    if ((jointLimits[i] > inputConfig[i]) || (jointLimits[i + 3] < inputConfig[i]))
    {
      /*  Compute key values */
      theta = inputConfig[i] - jointLimits[i];
      wrappedJointValueOffset = b_mod(theta);
      if ((wrappedJointValueOffset == 0.0) && (theta > 0.0)) {
        wrappedJointValueOffset = 6.2831853071795862;
      }

      if (wrappedJointValueOffset < jointLimits[i + 3] - jointLimits[i]) {
        validConfig[i] = jointLimits[i] + wrappedJointValueOffset;
        i++;
      } else {
        /*  If any element is NaN, the whole array will be thrown out so */
        /*  there is no need to continue */
        validConfig[0] = rtNaN;
        validConfig[1] = rtNaN;
        validConfig[2] = rtNaN;
        exitg1 = true;
      }
    } else {
      i++;
    }
  }
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * solveFirstThreeDHJoints Solve for the first three joint angles of a DH-parameterized robot
 *    This function computes the first three joint angles of a robot
 *    parameterized using Denavit-Hartenberg parameters. The function accepts
 *    a matrix of the fixed DH parameters, as well as the position of the
 *    fifth joint. The matrix of DH parameters is of size 6x4 for the 6
 *    non-fixed joints, where each row has the order [a alpha d 0], where a
 *    is a translation along x, alpha is a rotation about x, and d is a
 *    translation along z. The last value, which typically refers to theta
 *    (the rotation about z) for that joint, is not yet known; this function
 *    will solve for theta for the first three joints. When a robot has the
 *    last three axes intersecting, the position and orientation of the end
 *    effector can be split up: the position is entirely determined by the
 *    first three joints, while the orientation is governed by the last three
 *    joints (provided the first three are known). Furthermore, the position
 *    of any end effector can be related to the position of the fifth joint
 *    frame, which corresponds to the joint frame at the midpoint of the
 *    three intersecting joints. This frame will have the same position in
 *    any valid solution to the particular IK problem (though its orientation
 *    may differ), and its translation relative to the base frame is entirely
 *    defined by the first three joints. This function solves for those first
 *    three joints given the position of the joint 5 frame relative to the
 *    base. This solution method and notation follows Chp.3 of Pieper's 1968
 *    thesis, but adds two corrections, as well as minor notation changes and
 *    the addition of constraints to ensure only feasible solutions are
 *    output:
 *
 *    Pieper, D. The Kinematics Of Manipulators Under Computer Control.
 *    Stanford University (1968).
 * Arguments    : const double jt5Pos[3]
 *                double outputThetas_data[]
 *                int outputThetas_size[2]
 * Return Type  : void
 */
static void solveFirstThreeDHJoints(const double jt5Pos[3], double
  outputThetas_data[], int outputThetas_size[2])
{
  creal_T hSolns_data[2];
  creal_T h2;
  double possThetas[48];
  double diffMat[3];
  double theta1Constraint[2];
  double theta2Constraint[2];
  double theta2Opts[2];
  double d;
  double d1;
  double d2;
  double g_idx_0;
  double h1_im;
  double h1_re;
  double h3;
  double t4;
  double theta2Opts_tmp;
  double thetaWrap;
  int i;
  int ix;
  int iy;
  int j;
  int solIdx;
  signed char tmp_data[16];
  boolean_T x[48];
  boolean_T bv[16];
  boolean_T exitg1;
  boolean_T y;

  /*  Extract DH parameters from matrix */
  /*  Note that Pieper uses "s" instead of "d" in his solutions */
  /*  Three variables derived from jt5Pos */
  t4 = (jt5Pos[0] * jt5Pos[0] + jt5Pos[1] * jt5Pos[1]) + (jt5Pos[2] - 0.29) *
    (jt5Pos[2] - 0.29);

  /*  The first step is to solve for theta3. This is achieved by eliminating */
  /*  theta1 and theta2 through a number of substitutions and sum-of-squares */
  /*  operations. The resultant equation for theta3, is a function of */
  /*  sin(theta3) and cos(theta3), but this can be further mapped to a */
  /*  polynomial in h, where h = tan(theta3/2). This substitutions is made */
  /*  possible by the Weierstrass transformation, which maps sin(theta3) to */
  /*  2*h/(1 + h^2) and cos(theta3) to (1-h^2)/(1+h^2). The goal is then to */
  /*  solve the polynomial for h and map the solutions back to theta3. */
  /*  Since a1 = 0, the solution arises from R3 = F3, which produces a quadratic in h */
  /* solveForHCaseZeroA1 Solve for h when a1 is zero */
  /*    To solve for theta3, it is necessary to reparameterize a trigonometric */
  /*    equation in terms of a new parameter h = tan(theta3/2) using the */
  /*    Weierstrass equation. This function solves equation 3.25 in the Pieper */
  /*    inverse kinematics solution for h in the case where DH parameter a1 is */
  /*    zero. In that case, equation 3.25 becomes: */
  /*       R3 = F3 */
  /*    Here F1 to F4 are functions of theta3 (and the constant DH parameters), */
  /*    and R3 is a function of P, a known position input from the IK problem, */
  /*    and the DH parameter d1: */
  /*       R3 = P(1)^2 + P(2)^2 + (P(3) ? d1)^2 */
  /*    Equation 3.25 may then may be reparameterized in h, producing a */
  /*    quadratic polynomial in h. This function solves that polynomial for the */
  /*    values of h given R3 and the DH parameters of the associated serial */
  /*    manipulator. */
  /*    Copyright 2020 The MathWorks, Inc. */
  /*  These solutions solve the equation above, R3 = F3, which becomes a */
  /*  quadratic in h = tan(theta3/2). As the polynomial is quadratic, the */
  /*  solution has a format that matches that of the solutions to the quadratic */
  /*  equation A*h^2 + B*h + C = 0. */
  /*  Compute the terms of the quadratic equation */
  /*  There are three possible solution cases */
  /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
  /*    This is a convenience function designed for inputs with up to two */
  /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
  /*    returned. */
  h3 = fabs(((0.035099999999999951 - t4) + 0.0049000000000000146) + 0.091204);
  diffMat[0] = h3;
  diffMat[1] = 0.32616;
  diffMat[2] = fabs((((0.037800000000000056 - t4) + 0.0729) +
                     0.0049000000000000146) + 0.091204);
  y = true;
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 3)) {
    if (!(diffMat[i] < 1.0E-6)) {
      y = false;
      exitg1 = true;
    } else {
      i++;
    }
  }

  if (y) {
    /*  The trivial case happens whenever any value of theta3 solves the */
    /*  quadratic derived from equation 3.25. In that case, any theta3 may */
    /*  solve the problem, though it may be further constrained by equation */
    /*  3.26. Physically, this happens when theta3 has no impact on the */
    /*  position solution, or when its solution is intertwined with theta2. */
    iy = 1;
    hSolns_data[0].re = 0.0;
    hSolns_data[0].im = 0.0;
  } else {
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    if (h3 < 1.0E-6) {
      /*  When the first term is zero, the equation is linear */
      iy = 1;
      hSolns_data[0].re = -((((0.037800000000000056 - t4) + 0.0729) +
        0.0049000000000000146) + 0.091204) / -0.32616;
      hSolns_data[0].im = 0.0;
    } else {
      /*  The equation is quadratic */
      d = 0.10638034560000001 - 4.0 * (((0.035099999999999951 - t4) +
        0.0049000000000000146) + 0.091204) * ((((0.037800000000000056 - t4) +
        0.0729) + 0.0049000000000000146) + 0.091204);
      if (d < 0.0) {
        /*  This solution will be complex */
        h2.re = d;
        h2.im = 0.0;
        b_sqrt(&h2);
        h3 = 2.0 * (((0.035099999999999951 - t4) + 0.0049000000000000146) +
                    0.091204);
        if (0.0 - h2.im == 0.0) {
          h1_re = (0.32616 - h2.re) / h3;
          h1_im = 0.0;
        } else if (0.32616 - h2.re == 0.0) {
          h1_re = 0.0;
          h1_im = (0.0 - h2.im) / h3;
        } else {
          h1_re = (0.32616 - h2.re) / h3;
          h1_im = (0.0 - h2.im) / h3;
        }

        h2.re = d;
        h2.im = 0.0;
        b_sqrt(&h2);
        if (h2.im == 0.0) {
          t4 = (0.32616 + h2.re) / h3;
          h3 = 0.0;
        } else if (0.32616 + h2.re == 0.0) {
          t4 = 0.0;
          h3 = h2.im / h3;
        } else {
          t4 = (0.32616 + h2.re) / h3;
          h3 = h2.im / h3;
        }

        h2.re = t4;
        h2.im = h3;
      } else {
        /*  This solution will be real */
        g_idx_0 = sqrt(d);
        h3 = 2.0 * (((0.035099999999999951 - t4) + 0.0049000000000000146) +
                    0.091204);
        h1_re = (0.32616 - g_idx_0) / h3;
        h1_im = 0.0;
        h2.re = (g_idx_0 + 0.32616) / h3;
        h2.im = 0.0;
      }

      iy = 2;
      hSolns_data[0].re = h1_re;
      hSolns_data[0].im = h1_im;
      hSolns_data[1] = h2;
    }
  }

  /*  Initialize the matrix of possible solutions */
  memset(&possThetas[0], 0, 48U * sizeof(double));

  /*  After all solutions are processed, rows with NaNs will be removed */
  /*  Initialize theta3 to NaN and replace based on actual solutions */
  for (solIdx = 0; solIdx < 16; solIdx++) {
    possThetas[solIdx + 32] = rtNaN;
  }

  for (i = 0; i < iy; i++) {
    /*  Ensure only real solutions to h are converted */
    /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
    /*    This function replaces imaginary values with NaNs. This is useful when */
    /*    the element is part of a matrix, and rendering one element of the */
    /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
    /*    may be used to filter invalid solutions. */
    /*    Copyright 2020 The MathWorks, Inc. */
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    if (!(fabs(hSolns_data[i].im) < 1.0E-6)) {
      h3 = rtNaN;
    } else {
      h3 = hSolns_data[i].re;
    }

    if (rtIsNaN(h3)) {
      /*  When h3 is imaginary, theta3 = NaN */
      possThetas[i + 32] = rtNaN;
      possThetas[i + 36] = rtNaN;
    } else {
      /*  When h3 is real, there are two possible equivalent values of theta3 */
      possThetas[i + 32] = 2.0 * rt_atan2d_snf(h3, 1.0);
      possThetas[i + 36] = 2.0 * rt_atan2d_snf(-h3, -1.0);
    }
  }

  for (i = 0; i < 8; i++) {
    /*  If theta3 is NaN or imaginary, replace whole row with NaNs and skip to next row */
    d = possThetas[i + 32];

    /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
    /*    This function replaces imaginary values with NaNs. This is useful when */
    /*    the element is part of a matrix, and rendering one element of the */
    /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
    /*    may be used to filter invalid solutions. */
    /*    Copyright 2020 The MathWorks, Inc. */
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    if (rtIsNaN(d)) {
      possThetas[i] = rtNaN;
      possThetas[i + 16] = rtNaN;
      possThetas[i + 32] = rtNaN;
    } else {
      /*  Compute key subexpressions f1 to f3 and F1 to F4, which are functions of theta3 */
      /* computef13SupportingEquations Compute f1 to f3 supporting equations */
      /*    This function computes f1 to f3, which are functions of theta3. For a */
      /*    given robot with three consecutive revolute axes, the position of a */
      /*    joint a distance s4 along the joint 3 axis can be described as: */
      /*       P = T1*T2*T3*[0; 0; s4; 1], */
      /*    where Ti represent transformation matrices associated with links. Then */
      /*    this equation may be rewritten as P = T1*T2*f. This function computes */
      /*    the values of f that satisfy the rewritten equation. */
      /*  Helper functions */
      /*    Copyright 2020 The MathWorks, Inc. */
      /*  Initialize output */
      /*  Compute component terms */
      h3 = cos(d);
      t4 = sin(d);

      /*  Assemble outputs. Note that there is technically a fourth output, f(4) = */
      /*  1, but its value is unused, so it is not computed or returned. */
      d1 = 0.0700000000000001 * h3 + -0.302 * t4;
      d2 = 0.0700000000000001 * t4 - -0.302 * h3;

      /* computeF14SupportingEquations Compute intermediate variables F1 to F4 */
      /*    This function computes F1 to F4, which are intermediate variables in */
      /*    Pieper's derivation that are functions of the theta3 joint position */
      /*    (and constant parameters). The function accepts several DH parameters, */
      /*    as well as the intermediate variables f1 to f3, which are functions of */
      /*    theta3, and outputs the four F1 to F4 intermediate variables. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /*  Initialize output */
      /*  Compute theta2. The exact approach depends on the DH */
      /*  parameters, but the gist is the same: since the equations */
      /*  output multiple solutions, but some are actually just results */
      /*  of the sum of squares, i.e., they solve the local problem, */
      /*  but do not actually solve the overlying problem. Rather than */
      /*  compute all solutions and filter at the end, we filter here */
      /*  by always solving using two different equations. Then we */
      /*  choose only the solution that satisfies both equations. */
      /*  Since a1 is zero and sin(alpha1) is nonzero, solve for theta2 using equation 3.26 and the third element of 3.20 */
      theta2Opts_tmp = d2 * 0.0 + -1.0544273643306262E-15;
      solveTrigEquations(-d2, -(d1 + 0.27), (jt5Pos[2] - 0.29) -
                         -3.4914813388431334E-15 * theta2Opts_tmp, theta2Opts);
      solveTrigEquations(-d2, -(d1 + 0.27), (jt5Pos[2] - 0.29) -
                         -3.4914813388431334E-15 * (0.0 * d2 +
        -1.0544273643306262E-15), theta2Constraint);

      /*  Choose the solution(s) that solve both equations */
      /* chooseCorrectSolution Choose the solution that appears in both solutionPair1 and solutionPair2 */
      /*    This helper function is used to choose a correct solution when two */
      /*    solutions are provided, e.g. as the result of a sum of squares. The */
      /*    function accepts two 2-element vectors, solutionPair1 and */
      /*    solutionPair2, which represent the solution options from the source and */
      /*    constraint equations, respectively. The correct solution will be the */
      /*    solution both of the source equation, as well as a constraint equation */
      /*    for the same problem. This helper simply chooses the value that occurs */
      /*    in both the original and constraint solutions, within a tolerance. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /*  Filter any imaginary values out of the solution pairs by replacing them */
      /*  with NaNs */
      /*  To check equivalence, it's insufficient to just check whether the values */
      /*  are equal, because they are periodic. For example, -pi and pi are both */
      /*  valid outcomes of wrapToPi that fail a basic equality test but are */
      /*  equivalent in this context. Therefore, it's necessary to check that the */
      /*  difference of the two values, when wrapped to pi, is inside the expected tolerance. */
      /*  Have to wrap to pi so that the two values are comparable */
      /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
      /*    This function replaces imaginary values with NaNs. This is useful when */
      /*    the element is part of a matrix, and rendering one element of the */
      /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
      /*    may be used to filter invalid solutions. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
      h3 = theta2Opts[0];
      if (fabs(theta2Opts[0]) > 3.1415926535897931) {
        thetaWrap = b_mod(theta2Opts[0] + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (theta2Opts[0] + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        h3 = thetaWrap - 3.1415926535897931;
      }

      /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
      /*    This function replaces imaginary values with NaNs. This is useful when */
      /*    the element is part of a matrix, and rendering one element of the */
      /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
      /*    may be used to filter invalid solutions. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
      t4 = theta2Constraint[0];
      if (fabs(theta2Constraint[0]) > 3.1415926535897931) {
        thetaWrap = b_mod(theta2Constraint[0] + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (theta2Constraint[0] + 3.1415926535897931 >
             0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        t4 = thetaWrap - 3.1415926535897931;
      }

      theta2Opts[0] = rtNaN;

      /*  Have to wrap to pi so that the two values are comparable */
      /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
      /*    This function replaces imaginary values with NaNs. This is useful when */
      /*    the element is part of a matrix, and rendering one element of the */
      /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
      /*    may be used to filter invalid solutions. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
      h1_re = theta2Opts[1];
      if (fabs(theta2Opts[1]) > 3.1415926535897931) {
        thetaWrap = b_mod(theta2Opts[1] + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (theta2Opts[1] + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        h1_re = thetaWrap - 3.1415926535897931;
      }

      /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
      /*    This function replaces imaginary values with NaNs. This is useful when */
      /*    the element is part of a matrix, and rendering one element of the */
      /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
      /*    may be used to filter invalid solutions. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
      h1_im = theta2Constraint[1];
      if (fabs(theta2Constraint[1]) > 3.1415926535897931) {
        thetaWrap = b_mod(theta2Constraint[1] + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (theta2Constraint[1] + 3.1415926535897931 >
             0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        h1_im = thetaWrap - 3.1415926535897931;
      }

      theta2Opts[1] = rtNaN;
      g_idx_0 = h3 - t4;
      if (fabs(g_idx_0) > 3.1415926535897931) {
        thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        g_idx_0 = thetaWrap - 3.1415926535897931;
      }

      if (fabs(g_idx_0) < 1.0E-6) {
        theta2Opts[0] = h3;
      }

      g_idx_0 = h3 - h1_im;
      if (fabs(g_idx_0) > 3.1415926535897931) {
        thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        g_idx_0 = thetaWrap - 3.1415926535897931;
      }

      if (fabs(g_idx_0) < 1.0E-6) {
        theta2Opts[0] = h3;
      }

      g_idx_0 = h1_re - t4;
      if (fabs(g_idx_0) > 3.1415926535897931) {
        thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        g_idx_0 = thetaWrap - 3.1415926535897931;
      }

      if (fabs(g_idx_0) < 1.0E-6) {
        theta2Opts[1] = h1_re;
      }

      g_idx_0 = h1_re - h1_im;
      if (fabs(g_idx_0) > 3.1415926535897931) {
        thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
        if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
          thetaWrap = 6.2831853071795862;
        }

        g_idx_0 = thetaWrap - 3.1415926535897931;
      }

      if (fabs(g_idx_0) < 1.0E-6) {
        theta2Opts[1] = h1_re;
      }

      /*  Sort the output so that if there is one correct solution it is always in */
      /*  the first element slot */
      sort(theta2Opts);

      /*  Theta2 is a 2-element vector with up to two valid solutions (invalid */
      /*  solutions are represented by NaNs). Iterate over the possible values */
      /*  and add the second solution set in the latter half of the matrix (so */
      /*  they aren't overwritten by subsequent loops). */
      for (iy = 0; iy < 2; iy++) {
        /*  Update the local index so it's reflective of the indexed value of theta2 */
        solIdx = i + (iy << 3);

        /*  Update the value of theta3 in case it was previously set to NaN, */
        /*  and replace any invalid values of theta2 with NaN */
        possThetas[solIdx + 32] = d;
        h3 = theta2Opts[iy];
        possThetas[solIdx + 16] = h3;

        /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
        /*    This function replaces imaginary values with NaNs. This is useful when */
        /*    the element is part of a matrix, and rendering one element of the */
        /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
        /*    may be used to filter invalid solutions. */
        /*    Copyright 2020 The MathWorks, Inc. */
        /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
        /*    This is a convenience function designed for inputs with up to two */
        /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
        /*    returned. */
        /*  If any of the joint variables in NaN, replace it and all the */
        /*  remaining joints to solve with NaNs and move on to the next loop */
        if (rtIsNaN(h3)) {
          possThetas[solIdx] = rtNaN;
          possThetas[solIdx + 16] = rtNaN;
        } else {
          /*  Compute theta1 from the first two elements of eq 3.20 */
          /* computeg12SupportingEquations Compute g1 and g2 supporting equations */
          /*    This function computes g1 and g2, which are functions of theta2 and */
          /*    theta3. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /*  Initialize output */
          /*  Compute component terms */
          t4 = cos(h3);
          h3 = sin(h3);

          /*  Assemble outputs */
          g_idx_0 = t4 * (d1 + 0.27) - h3 * d2;
          h3 = (-theta2Opts_tmp - -3.4914813388431334E-15 * h3 * (d1 + 0.27)) -
            -3.4914813388431334E-15 * t4 * d2;
          solveTrigEquations(g_idx_0, h3, jt5Pos[0], theta2Constraint);
          solveTrigEquations(-h3, g_idx_0, jt5Pos[1], theta1Constraint);

          /* chooseCorrectSolution Choose the solution that appears in both solutionPair1 and solutionPair2 */
          /*    This helper function is used to choose a correct solution when two */
          /*    solutions are provided, e.g. as the result of a sum of squares. The */
          /*    function accepts two 2-element vectors, solutionPair1 and */
          /*    solutionPair2, which represent the solution options from the source and */
          /*    constraint equations, respectively. The correct solution will be the */
          /*    solution both of the source equation, as well as a constraint equation */
          /*    for the same problem. This helper simply chooses the value that occurs */
          /*    in both the original and constraint solutions, within a tolerance. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /*  Filter any imaginary values out of the solution pairs by replacing them */
          /*  with NaNs */
          /*  To check equivalence, it's insufficient to just check whether the values */
          /*  are equal, because they are periodic. For example, -pi and pi are both */
          /*  valid outcomes of wrapToPi that fail a basic equality test but are */
          /*  equivalent in this context. Therefore, it's necessary to check that the */
          /*  difference of the two values, when wrapped to pi, is inside the expected tolerance. */
          /*  Have to wrap to pi so that the two values are comparable */
          /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
          /*    This function replaces imaginary values with NaNs. This is useful when */
          /*    the element is part of a matrix, and rendering one element of the */
          /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
          /*    may be used to filter invalid solutions. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
          /*    This is a convenience function designed for inputs with up to two */
          /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
          /*    returned. */
          h3 = theta2Constraint[0];
          if (fabs(theta2Constraint[0]) > 3.1415926535897931) {
            thetaWrap = b_mod(theta2Constraint[0] + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (theta2Constraint[0] + 3.1415926535897931 >
                 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            h3 = thetaWrap - 3.1415926535897931;
          }

          /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
          /*    This function replaces imaginary values with NaNs. This is useful when */
          /*    the element is part of a matrix, and rendering one element of the */
          /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
          /*    may be used to filter invalid solutions. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
          /*    This is a convenience function designed for inputs with up to two */
          /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
          /*    returned. */
          t4 = theta1Constraint[0];
          if (fabs(theta1Constraint[0]) > 3.1415926535897931) {
            thetaWrap = b_mod(theta1Constraint[0] + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (theta1Constraint[0] + 3.1415926535897931 >
                 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            t4 = thetaWrap - 3.1415926535897931;
          }

          theta2Constraint[0] = rtNaN;

          /*  Have to wrap to pi so that the two values are comparable */
          /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
          /*    This function replaces imaginary values with NaNs. This is useful when */
          /*    the element is part of a matrix, and rendering one element of the */
          /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
          /*    may be used to filter invalid solutions. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
          /*    This is a convenience function designed for inputs with up to two */
          /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
          /*    returned. */
          h1_re = theta2Constraint[1];
          if (fabs(theta2Constraint[1]) > 3.1415926535897931) {
            thetaWrap = b_mod(theta2Constraint[1] + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (theta2Constraint[1] + 3.1415926535897931 >
                 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            h1_re = thetaWrap - 3.1415926535897931;
          }

          /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
          /*    This function replaces imaginary values with NaNs. This is useful when */
          /*    the element is part of a matrix, and rendering one element of the */
          /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
          /*    may be used to filter invalid solutions. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
          /*    This is a convenience function designed for inputs with up to two */
          /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
          /*    returned. */
          h1_im = theta1Constraint[1];
          if (fabs(theta1Constraint[1]) > 3.1415926535897931) {
            thetaWrap = b_mod(theta1Constraint[1] + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (theta1Constraint[1] + 3.1415926535897931 >
                 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            h1_im = thetaWrap - 3.1415926535897931;
          }

          theta2Constraint[1] = rtNaN;
          g_idx_0 = h3 - t4;
          if (fabs(g_idx_0) > 3.1415926535897931) {
            thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            g_idx_0 = thetaWrap - 3.1415926535897931;
          }

          if (fabs(g_idx_0) < 1.0E-6) {
            theta2Constraint[0] = h3;
          }

          g_idx_0 = h3 - h1_im;
          if (fabs(g_idx_0) > 3.1415926535897931) {
            thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            g_idx_0 = thetaWrap - 3.1415926535897931;
          }

          if (fabs(g_idx_0) < 1.0E-6) {
            theta2Constraint[0] = h3;
          }

          g_idx_0 = h1_re - t4;
          if (fabs(g_idx_0) > 3.1415926535897931) {
            thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            g_idx_0 = thetaWrap - 3.1415926535897931;
          }

          if (fabs(g_idx_0) < 1.0E-6) {
            theta2Constraint[1] = h1_re;
          }

          g_idx_0 = h1_re - h1_im;
          if (fabs(g_idx_0) > 3.1415926535897931) {
            thetaWrap = b_mod(g_idx_0 + 3.1415926535897931);
            if ((thetaWrap == 0.0) && (g_idx_0 + 3.1415926535897931 > 0.0)) {
              thetaWrap = 6.2831853071795862;
            }

            g_idx_0 = thetaWrap - 3.1415926535897931;
          }

          if (fabs(g_idx_0) < 1.0E-6) {
            theta2Constraint[1] = h1_re;
          }

          /*  Sort the output so that if there is one correct solution it is always in */
          /*  the first element slot */
          sort(theta2Constraint);

          /*  Since theta1 is the last value that is solved for, only one */
          /*  of the solutions will be valid, and chooseCorrectSolution */
          /*  sorts the results so that if there is only one solution, it */
          /*  is always the first element (and the other element is nan) */
          possThetas[solIdx] = theta2Constraint[0];

          /*  Update the array of possible theta values */
          /* replaceImagWithNaN Replace imaginary and empty elements with NaNs */
          /*    This function replaces imaginary values with NaNs. This is useful when */
          /*    the element is part of a matrix, and rendering one element of the */
          /*    matrix imaginary will make the entire matrix imaginary. Furthermore, it */
          /*    may be used to filter invalid solutions. */
          /*    Copyright 2020 The MathWorks, Inc. */
          /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
          /*    This is a convenience function designed for inputs with up to two */
          /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
          /*    returned. */
        }
      }
    }
  }

  /*  Now we are left with an 8x3 matrix where some values are NaN. The */
  /*  function will only output the rows where all elements are non-NaN. */
  for (solIdx = 0; solIdx < 48; solIdx++) {
    x[solIdx] = !rtIsNaN(possThetas[solIdx]);
  }

  for (i = 0; i < 16; i++) {
    bv[i] = true;
  }

  iy = -1;
  solIdx = 0;
  i = 32;
  for (j = 0; j < 16; j++) {
    solIdx++;
    i++;
    iy++;
    ix = solIdx;
    exitg1 = false;
    while ((!exitg1) && (ix <= i)) {
      if (!x[ix - 1]) {
        bv[iy] = false;
        exitg1 = true;
      } else {
        ix += 16;
      }
    }
  }

  j = 0;
  iy = 0;
  for (i = 0; i < 16; i++) {
    if (bv[i]) {
      j++;
      tmp_data[iy] = (signed char)(i + 1);
      iy++;
    }
  }

  outputThetas_size[0] = j;
  outputThetas_size[1] = 3;
  for (solIdx = 0; solIdx < 3; solIdx++) {
    for (iy = 0; iy < j; iy++) {
      outputThetas_data[iy + j * solIdx] = possThetas[(tmp_data[iy] + (solIdx <<
        4)) - 1];
    }
  }
}

/*
 * solveTrigEquations Solve equations of the form a*cos(theta) + b*sin(theta) = c for theta
 *    This function solves the common trigonometric equality by equating the
 *    solution to cos(phi)sin(theta) + sin(phi)cos(theta) = sin(phi + theta).
 *    The function returns two possible solutions for theta.
 * Arguments    : double a
 *                double b
 *                double c
 *                double theta[2]
 * Return Type  : void
 */
static void solveTrigEquations(double a, double b, double c, double theta[2])
{
  creal_T dc;
  double cPrime;

  /*    Copyright 2020 The MathWorks, Inc. */
  theta[0] = rtNaN;
  theta[1] = rtNaN;

  /*  Handle the trivial case */
  /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
  /*    This is a convenience function designed for inputs with up to two */
  /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
  /*    returned. */
  cPrime = fabs(a);
  if ((cPrime < 1.0E-6) && (fabs(b) < 1.0E-6) && (fabs(c) < 1.0E-6)) {
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    theta[0] = 0.0;
  } else {
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    if ((!(cPrime < 1.0E-6)) || (!(fabs(b) < 1.0E-6)) || (fabs(c) < 1.0E-6)) {
      /*  As long as a or b are nonzero, a set of general solutions may be found */
      cPrime = c / sqrt(a * a + b * b);
      if ((cPrime < 1.0) || (cPrime - 1.0 < 1.0E-6)) {
        /*  Throw out the imaginary solutions, which occur when cPrime > 1 */
        dc.re = cPrime;
        dc.im = 0.0;
        b_asin(&dc);
        theta[0] = dc.re - rt_atan2d_snf(a, b);
        dc.re = cPrime;
        dc.im = 0.0;
        b_asin(&dc);
        theta[1] = -dc.re - rt_atan2d_snf(-a, -b);
      } else {
        /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
        /*    This is a convenience function designed for inputs with up to two */
        /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
        /*    returned. */
      }
    } else {
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
      /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
      /*    This is a convenience function designed for inputs with up to two */
      /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
      /*    returned. */
    }
  }
}

/*
 * robotIK Function for generating closed-form inverse kinematics solutions to the DH robot given by the parameters specified below
 *    $Revision: $ $Date: $
 *
 *    Generated on 31-Jan-2022 14:36:01
 * Arguments    : const double eeTform[16]
 *                unsigned char enforceJointLimits
 *                unsigned char sortByDistance
 *                const double referenceConfig[16]
 *                double qOpts_data[]
 *                int qOpts_size[2]
 * Return Type  : void
 */
void robotIK(const double eeTform[16], unsigned char enforceJointLimits,
             unsigned char sortByDistance, const double referenceConfig[16],
             double qOpts_data[], int qOpts_size[2])
{
  static const double dhParams[24] = { 0.0, 0.27, 0.0700000000000001, 0.0, 0.0,
    0.0, -1.5707963267949, 0.0, -1.5707963267949, 1.5707963267949,
    -1.5707963267949, -1.5707963267949, 0.29, 0.0, 0.0, 0.302, 0.0, 0.072, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0 };

  static const double b[16] = { 6.12323399573677E-17, 1.0, 0.0, 0.0, -1.0,
    6.12323399573677E-17, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

  static const double b_b[16] = { 1.0, 0.0, 0.0, 0.0, 0.0, -1.0,
    1.22464679914735E-16, 0.0, 0.0, -1.22464679914735E-16, -1.0, 0.0, 0.0, 0.072,
    -4.40872847693047E-18, 1.0 };

  static const double jointLimits[12] = { -2.87979, -1.91986, -1.91986, -2.79253,
    -2.094395, -6.98132, 2.87979, 1.91986, 1.22173, 2.79253, 2.094395, 6.98132 };

  static const double dv1[9] = { 1.0, 0.0, 0.0, 0.0, -3.4914813388431334E-15,
    1.0, 0.0, -1.0, -3.4914813388431334E-15 };

  static const double a[6] = { 0.0, -1.5707963267949, 0.0, 0.0, 0.0,
    -1.5707963267949 };

  static const double dv2[6] = { -2.87979, -1.91986, -1.91986, 2.87979, 1.91986,
    1.22173 };

  static const double dv3[6] = { -2.79253, -2.094395, -6.98132, 2.79253,
    2.094395, 6.98132 };

  static double dv[2] = { 0.0, 0.0 };

  static const signed char b_a[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 1 };

  static const signed char c_b[9] = { 1, 0, 0, 0, -1, 0, 0, 0, 1 };

  double a_data[192];
  double allSolnOpts_data[192];
  double qOptsAllSolns_data[192];
  double q456Opts_data[96];
  double varargin_1_data[96];
  double q123Opts_data[48];
  double dist_data[32];
  double c_a[16];
  double c_jt4ZeroPose[16];
  double eePose[16];
  double jt4ZeroPose[16];
  double shiftedJointLimits[12];
  double b_jt4ZeroPose[9];
  double jt4ToEERot[9];
  double b_eulAngles[6];
  double orientationSolns[6];
  double d_b[4];
  double b_y[3];
  double eulAngles[3];
  double y[2];
  double TFixed_tmp;
  double d;
  double diffRotation;
  double eulAltUnwrapped_idx_0;
  double eulAltUnwrapped_idx_1;
  double eulAltUnwrapped_idx_2;
  int iidx_data[32];
  int a_size[2];
  int b_a_size[2];
  int q123Opts_size[2];
  int tmp_size[2];
  int dist_size[1];
  int isValidRowIdx_size[1];
  int i;
  int iacol;
  int ibcol;
  int ibtile;
  int jcol;
  int jtIdx;
  int k;
  int nrows;
  int q456Opts_size_idx_0_tmp;
  int trueCount;
  signed char c_tmp_data[2];
  signed char d_tmp_data[2];
  signed char i1;
  signed char i2;
  signed char input_sizes_idx_1;
  signed char sizes_idx_1;
  boolean_T b_tmp_data[192];
  boolean_T tmp_data[192];
  boolean_T isValidRowIdx_data[32];
  boolean_T x_data[3];
  boolean_T isRevJointFullRange[2];
  boolean_T empty_non_axis_sizes;
  boolean_T exitg1;
  boolean_T guard1 = false;
  dv[0U] = rtGetMinusInf();
  dv[1U] = rtGetInf();

  /*  Compute the shifted joint limits, which are the limits during solution, where theta offsets are not yet in play */
  for (ibcol = 0; ibcol < 2; ibcol++) {
    ibtile = ibcol * 6;
    for (k = 0; k < 6; k++) {
      shiftedJointLimits[ibtile + k] = a[k];
    }
  }

  for (i = 0; i < 12; i++) {
    shiftedJointLimits[i] += jointLimits[i];
  }

  /*  Convert the end effector pose in the global frame to the end effector described by the DH parameters relative to the DH-described origin */
  for (i = 0; i < 4; i++) {
    input_sizes_idx_1 = b_a[i];
    sizes_idx_1 = b_a[i + 4];
    i1 = b_a[i + 8];
    i2 = b_a[i + 12];
    for (iacol = 0; iacol < 4; iacol++) {
      ibcol = iacol << 2;
      c_a[i + ibcol] = (((double)input_sizes_idx_1 * eeTform[ibcol] + (double)
                         sizes_idx_1 * eeTform[ibcol + 1]) + (double)i1 *
                        eeTform[ibcol + 2]) + (double)i2 * eeTform[ibcol + 3];
    }

    d = c_a[i];
    eulAltUnwrapped_idx_2 = c_a[i + 4];
    eulAltUnwrapped_idx_0 = c_a[i + 8];
    eulAltUnwrapped_idx_1 = c_a[i + 12];
    for (iacol = 0; iacol < 4; iacol++) {
      ibcol = iacol << 2;
      eePose[i + ibcol] = ((d * b[ibcol] + eulAltUnwrapped_idx_2 * b[ibcol + 1])
                           + eulAltUnwrapped_idx_0 * b[ibcol + 2]) +
        eulAltUnwrapped_idx_1 * b[ibcol + 3];
    }
  }

  /*  Parse optional inputs */
  /*  If joint limits are not enforced, set the shifted joint limits to have infinite range */
  if (enforceJointLimits == 0) {
    for (jcol = 0; jcol < 2; jcol++) {
      ibtile = jcol * 6;
      for (ibcol = 0; ibcol < 6; ibcol++) {
        shiftedJointLimits[ibtile + ibcol] = dv[jcol];
      }
    }
  }

  /*  Map the desired end effector pose to the pose of the central intersecting joint. */
  /*  Solve for the position of the first three joints from the pose of joint 5 */
  for (i = 0; i < 4; i++) {
    d = eePose[i];
    eulAltUnwrapped_idx_2 = eePose[i + 4];
    eulAltUnwrapped_idx_0 = eePose[i + 8];
    eulAltUnwrapped_idx_1 = eePose[i + 12];
    for (iacol = 0; iacol < 4; iacol++) {
      ibcol = iacol << 2;
      c_a[i + ibcol] = ((d * b_b[ibcol] + eulAltUnwrapped_idx_2 * b_b[ibcol + 1])
                        + eulAltUnwrapped_idx_0 * b_b[ibcol + 2]) +
        eulAltUnwrapped_idx_1 * b_b[ibcol + 3];
    }
  }

  solveFirstThreeDHJoints(*(double (*)[3])&c_a[12], q123Opts_data, q123Opts_size);

  /*  Solve for the positions of the intersecting axes */
  /*  For each position solution, this configuration of the last three axes produces at least two possible orientation solutions */
  q456Opts_size_idx_0_tmp = q123Opts_size[0] << 1;
  ibcol = q456Opts_size_idx_0_tmp * 3;
  if (0 <= ibcol - 1) {
    memset(&q456Opts_data[0], 0, ibcol * sizeof(double));
  }

  /*  The next step seeks to find the orientation, which is entirely governed by the last three joints. This means that rotation from the fourth joint to the end effector can be mapped to three rotations in-place about the fifth joint. Since the rotations are in-place, they can be defined relative to the fourth joint axes, assuming a fixed pose rotation at the end to align with the end effector. The fixed rotation is found using the DH parameters, and corresponds to the rotation of the end effector relative to the fourth joint when the last three joints are all zero. */
  i = q123Opts_size[0];
  for (jtIdx = 0; jtIdx < i; jtIdx++) {
    /*  Get the position of the fourth joint at its zero position when the first three joints are positioned for IK */
    /* getJoint4PoseFromDH Get the pose of the fourth joint when the first three joints set to q123 and joint4 angle is zero */
    /*  Initialize output */
    memset(&jt4ZeroPose[0], 0, 16U * sizeof(double));
    jt4ZeroPose[0] = 1.0;
    jt4ZeroPose[5] = 1.0;
    jt4ZeroPose[10] = 1.0;
    jt4ZeroPose[15] = 1.0;
    for (jcol = 0; jcol < 3; jcol++) {
      d = q123Opts_data[jtIdx + q123Opts_size[0] * jcol];
      eulAltUnwrapped_idx_1 = sin(d);
      eulAltUnwrapped_idx_0 = cos(d);
      eulAltUnwrapped_idx_2 = dhParams[jcol + 6];
      diffRotation = sin(eulAltUnwrapped_idx_2);
      TFixed_tmp = cos(eulAltUnwrapped_idx_2);
      c_a[0] = eulAltUnwrapped_idx_0;
      c_a[4] = -eulAltUnwrapped_idx_1;
      c_a[8] = 0.0;
      c_a[12] = 0.0;
      c_a[1] = eulAltUnwrapped_idx_1;
      c_a[5] = eulAltUnwrapped_idx_0;
      c_a[9] = 0.0;
      c_a[13] = 0.0;
      c_a[2] = 0.0;
      c_a[3] = 0.0;
      c_a[6] = 0.0;
      c_a[7] = 0.0;
      c_a[10] = 1.0;
      c_a[11] = 0.0;
      c_a[14] = 0.0;
      c_a[15] = 1.0;
      for (iacol = 0; iacol < 4; iacol++) {
        d = jt4ZeroPose[iacol];
        eulAltUnwrapped_idx_2 = jt4ZeroPose[iacol + 4];
        eulAltUnwrapped_idx_0 = jt4ZeroPose[iacol + 8];
        eulAltUnwrapped_idx_1 = jt4ZeroPose[iacol + 12];
        for (ibcol = 0; ibcol < 4; ibcol++) {
          nrows = ibcol << 2;
          c_jt4ZeroPose[iacol + nrows] = ((d * c_a[nrows] +
            eulAltUnwrapped_idx_2 * c_a[nrows + 1]) + eulAltUnwrapped_idx_0 *
            c_a[nrows + 2]) + eulAltUnwrapped_idx_1 * c_a[nrows + 3];
        }
      }

      c_a[0] = 1.0;
      c_a[4] = 0.0;
      c_a[8] = 0.0;
      c_a[12] = dhParams[jcol];
      c_a[1] = 0.0;
      c_a[5] = TFixed_tmp;
      c_a[9] = -diffRotation;
      c_a[13] = 0.0;
      c_a[2] = 0.0;
      c_a[6] = diffRotation;
      c_a[10] = TFixed_tmp;
      c_a[14] = dhParams[jcol + 12];
      c_a[3] = 0.0;
      c_a[7] = 0.0;
      c_a[11] = 0.0;
      c_a[15] = 1.0;
      for (iacol = 0; iacol < 4; iacol++) {
        d = c_jt4ZeroPose[iacol];
        eulAltUnwrapped_idx_2 = c_jt4ZeroPose[iacol + 4];
        eulAltUnwrapped_idx_0 = c_jt4ZeroPose[iacol + 8];
        eulAltUnwrapped_idx_1 = c_jt4ZeroPose[iacol + 12];
        for (ibcol = 0; ibcol < 4; ibcol++) {
          nrows = ibcol << 2;
          jt4ZeroPose[iacol + nrows] = ((d * c_a[nrows] + eulAltUnwrapped_idx_2 *
            c_a[nrows + 1]) + eulAltUnwrapped_idx_0 * c_a[nrows + 2]) +
            eulAltUnwrapped_idx_1 * c_a[nrows + 3];
        }
      }
    }

    /*  Compute the rotation matrix needed to get to the end */
    /*  The orientation of the end effector in the world frame can be written: */
    /*     eeRot = jt4ZeroRot*(Rotation about axes 4-6)*eeFixedRotation */
    /*  Then the goal is to solve for the rotation about the axes and relate them to he known form from the DH parameters, if a valid solution exists: */
    /*     (Rotation about axes 4-6) = jt4ZeroRot'*eeRot*eeFixedRotation' */
    for (iacol = 0; iacol < 3; iacol++) {
      ibcol = iacol << 2;
      for (nrows = 0; nrows < 3; nrows++) {
        ibtile = nrows << 2;
        b_jt4ZeroPose[iacol + 3 * nrows] = (jt4ZeroPose[ibcol] * eePose[ibtile]
          + jt4ZeroPose[ibcol + 1] * eePose[ibtile + 1]) + jt4ZeroPose[ibcol + 2]
          * eePose[ibtile + 2];
      }

      d = b_jt4ZeroPose[iacol];
      eulAltUnwrapped_idx_2 = b_jt4ZeroPose[iacol + 3];
      eulAltUnwrapped_idx_0 = b_jt4ZeroPose[iacol + 6];
      for (ibcol = 0; ibcol < 3; ibcol++) {
        jt4ToEERot[iacol + 3 * ibcol] = (d * dv1[3 * ibcol] +
          eulAltUnwrapped_idx_2 * dv1[3 * ibcol + 1]) + eulAltUnwrapped_idx_0 *
          dv1[3 * ibcol + 2];
      }
    }

    /*  This orientation produces at least two configurations for every solution, when joint limits allow */
    /* convertRotationToZYZAxesAngles Convert desired orientation to rotation about Z-Y-Z */
    /*    This function is used to three angles of rotation corresponding to */
    /*    consecutive joint angles whose joint axes intersect at a single, common */
    /*    point. This function addresses the case where the first joint rotation */
    /*    is about Z, and the subsequent angles, in order and defined relative to */
    /*    the first joint frame, are about Y and then Z. The function accepts the */
    /*    rotation of the last joint relative to the origin of the first one, as */
    /*    well as the sign of each axes. The second output indicates joints that */
    /*    are in gimbal lock, where 1 indicates that they are, and zero indicates */
    /*    that they are not. When joints are in gimbal lock, the affected joint */
    /*    axes align and an infinite combination of joint angles are possible (as */
    /*    long as the total rotation still matches the target). The default */
    /*    assumption is that rotation is divided over the joint along the */
    /*    affected axis. */
    /*    Copyright 2020 The MathWorks, Inc. */
    rotm2eul(jt4ToEERot, eulAngles);

    /*  The jointsInGimalLock variable indicates redundant joints, i.e. joints */
    /*  that complement each other and can have an infinite pair of values in the */
    /*  directJointAngleMaps output. Initialize this value to zeros (no joints in */
    /*  gimbal lock). This is a flag that is consistent across rotation functions */
    /*  and may be used by the caller function. */
    /*  When the middle angle is zero, the first and last joints are co-axial, */
    /*  meaning there are an infinite set of solutions. Use a helper function to */
    /*  distribute the values consistently given knowledge of the joint limits. */
    /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
    /*    This is a convenience function designed for inputs with up to two */
    /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
    /*    returned. */
    if (fabs(eulAngles[1]) < 1.0E-6) {
      /* distributeRotationOverJoints Distribute a rotation over several in-line revolute joints */
      /*    When revolute joints are co-axial, the total rotation can be distributed */
      /*    over the joints in a number of ways. This function assigns a default */
      /*    behavior that respects the joint limits. For the case where no joint */
      /*    limits are required, they should be provided as infinite, i.e [-inf */
      /*    inf] for each joint. The default behaviors are as follows: */
      /*  */
      /*       - If any joint limits have a range of at minimum 2*pi, all total */
      /*         rotation amounts are achievable and the rotation is distributed */
      /*         evenly over the joints with infinite range, assuming the other */
      /*         joints are centered within their range. */
      /*  */
      /*       - If all joints have finite ranges with total range less than 2*pi, */
      /*         some total rotation amounts may not be feasible and the rotation */
      /*         is distributed as much as possible on the distal joints (unused */
      /*         more proximal joints are centered). If the solution is infeasible, */
      /*         a NaN-vector is returned. */
      /*  */
      /*    The goal of these distributions is to favor solutions that are */
      /*    efficient. This function accepts the total rotation (in radians) to be */
      /*    divided over N joints, the signs of those N joints (whether rotation is */
      /*    positive or negative about the axes), and the joint limits, given as an */
      /*    Nx2 matrix. */
      /*  */
      /*    If joint limits are ignored, they can be provided as infinite; the */
      /*    behavior is equivalent. This function returns an N-element row vector. */
      /*    Copyright 2020 The MathWorks, Inc. */
      /*  Get the total number of joints from the joint limit input */
      /*  Initialize the output */
      jt4ToEERot[6] = 0.0;
      jt4ToEERot[2] = 0.0;
      y[0] = 0.0;
      jt4ToEERot[7] = 0.0;
      jt4ToEERot[5] = 0.0;
      y[1] = 0.0;
      jt4ToEERot[8] = 1.0;
      rotm2eul(jt4ToEERot, eulAngles);
      eulAltUnwrapped_idx_1 = eulAngles[0] + eulAngles[2];

      /*  Remap the joint limits to fit the assumption that all axes are positive. */
      /*  Since the joint limits can contain infinite elements, it is necessary to */
      /*  use element-wise multiplication, as matrix multiplication can result in */
      /*  NaNs when it causes sums of infinities. */
      /*  Re-order the joint limits to ensure the lower limit always comes first */
      /*  (in case the of a sign flip in the previous line) */
      d_b[0] = shiftedJointLimits[3];
      d_b[1] = shiftedJointLimits[5];
      d_b[2] = shiftedJointLimits[9];
      d_b[3] = shiftedJointLimits[11];
      b_sort(d_b);

      /*  Determine the total ranges of each joint. Since all joints are revolute, */
      /*  a range of 2*pi or greater is equivalent to an infinite range as the IK */
      /*  problem does not distinguish between periodic equivalents. Note that a */
      /*  downstream helper in the IK solution, applyJointLimits, includes a check */
      /*  that wraps periodic equivalents back to their values given the joint */
      /*  limits. */
      d = d_b[2] - d_b[0];
      eulAltUnwrapped_idx_0 = d;

      /*  Use a tolerance check on the equality. Since isEqualWithinTolerance */
      /*  returns a scalar, it is necessary to do this inside a for-loop */
      if ((d > 6.2831853071795862) || (fabs(d - 6.2831853071795862) < 1.0E-6)) {
        isRevJointFullRange[0] = true;
      } else {
        /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
        /*    This is a convenience function designed for inputs with up to two */
        /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
        /*    returned. */
        isRevJointFullRange[0] = false;
      }

      d = d_b[3] - d_b[1];

      /*  Use a tolerance check on the equality. Since isEqualWithinTolerance */
      /*  returns a scalar, it is necessary to do this inside a for-loop */
      if ((d > 6.2831853071795862) || (fabs(d - 6.2831853071795862) < 1.0E-6)) {
        isRevJointFullRange[1] = true;
      } else {
        /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
        /*    This is a convenience function designed for inputs with up to two */
        /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
        /*    returned. */
        isRevJointFullRange[1] = false;
      }

      /*  There are two primary cases: when some joints have full range, any */
      /*  solution is feasible and the variable values are distributed over these */
      /*  joints. When all of the joints have range of less than 2*pi, the problem */
      /*  is more complex, as some solutions may not be feasible. */
      empty_non_axis_sizes = false;
      k = 0;
      exitg1 = false;
      while ((!exitg1) && (k <= 1)) {
        if (!isRevJointFullRange[k]) {
          k++;
        } else {
          empty_non_axis_sizes = true;
          exitg1 = true;
        }
      }

      guard1 = false;
      if (empty_non_axis_sizes) {
        /*  If any of the joint have infinite ranges, use that to distribute the */
        /*  total rotation. First, place the joints with finite ranges in the */
        /*  middle of their respective ranges, then distribute the remaining */
        /*  joint rotation evenly over the joints with at least 2*pi range. */
        trueCount = 0;
        if (!isRevJointFullRange[0]) {
          trueCount = 1;
        }

        if (!isRevJointFullRange[1]) {
          trueCount++;
        }

        for (jcol = 0; jcol < trueCount; jcol++) {
          iacol = 0;
          if (!isRevJointFullRange[0]) {
            c_tmp_data[0] = 1;
            iacol = 1;
          }

          if (!isRevJointFullRange[1]) {
            c_tmp_data[iacol] = 2;
          }

          ibcol = c_tmp_data[jcol] - 1;
          y[ibcol] = (d_b[ibcol] + d_b[c_tmp_data[jcol] + 1]) / 2.0;
        }

        /*  Compute the remaining rotation and wrap it to the interval [-pi, pi], */
        /*  then distribute over the joints with complete range */
        eulAltUnwrapped_idx_2 = eulAltUnwrapped_idx_1 - (y[0] + y[1]);
        if (fabs(eulAltUnwrapped_idx_2) > 3.1415926535897931) {
          TFixed_tmp = b_mod(eulAltUnwrapped_idx_2 + 3.1415926535897931);
          if ((TFixed_tmp == 0.0) && (eulAltUnwrapped_idx_2 + 3.1415926535897931
               > 0.0)) {
            TFixed_tmp = 6.2831853071795862;
          }

          eulAltUnwrapped_idx_2 = TFixed_tmp - 3.1415926535897931;
        }

        trueCount = 0;
        if (isRevJointFullRange[0]) {
          trueCount = 1;
        }

        if (isRevJointFullRange[1]) {
          trueCount++;
        }

        for (ibtile = 0; ibtile < trueCount; ibtile++) {
          iacol = 0;
          ibcol = 0;
          if (isRevJointFullRange[0]) {
            d_tmp_data[0] = 1;
            iacol = 1;
            ibcol = 1;
          }

          if (isRevJointFullRange[1]) {
            d_tmp_data[iacol] = 2;
            ibcol++;
          }

          y[d_tmp_data[ibtile] - 1] = eulAltUnwrapped_idx_2 / (double)ibcol;
        }

        guard1 = true;
      } else {
        /*  Use an algorithm that favors loading distal joints, which are */
        /*  typically easier to change: first set all the joints to their */
        /*  mid-range values. Then iterate over the joints from first to last, */
        /*  moving each joint up or down based on the difference in the current */
        /*  total from the desired total, until the desired total is reached. */
        /*  This is essentially a cascaded bang-bang controller. */
        /*  Initialize the joint angles to their mid-range values */
        y[0] = (d_b[0] + d_b[2]) / 2.0;
        y[1] = (d_b[1] + d_b[3]) / 2.0;

        /*  Iterate over the joints, using a feedback law to move them closer */
        /*  to the desired total */
        if (fabs(eulAltUnwrapped_idx_1) > 3.1415926535897931) {
          TFixed_tmp = b_mod(eulAltUnwrapped_idx_1 + 3.1415926535897931);
          if ((TFixed_tmp == 0.0) && (eulAltUnwrapped_idx_1 + 3.1415926535897931
               > 0.0)) {
            TFixed_tmp = 6.2831853071795862;
          }

          eulAltUnwrapped_idx_1 = TFixed_tmp - 3.1415926535897931;
        }

        diffRotation = eulAltUnwrapped_idx_1 - (y[0] + y[1]);
        if (fabs(diffRotation) > 3.1415926535897931) {
          TFixed_tmp = b_mod(diffRotation + 3.1415926535897931);
          if ((TFixed_tmp == 0.0) && (diffRotation + 3.1415926535897931 > 0.0))
          {
            TFixed_tmp = 6.2831853071795862;
          }

          diffRotation = TFixed_tmp - 3.1415926535897931;
        }

        eulAltUnwrapped_idx_2 = diffRotation;
        if (diffRotation < 0.0) {
          eulAltUnwrapped_idx_2 = -1.0;
        } else if (diffRotation > 0.0) {
          eulAltUnwrapped_idx_2 = 1.0;
        } else {
          if (diffRotation == 0.0) {
            eulAltUnwrapped_idx_2 = 0.0;
          }
        }

        y[0] += eulAltUnwrapped_idx_2 * fmin(fabs(diffRotation),
          eulAltUnwrapped_idx_0 / 2.0);
        diffRotation = eulAltUnwrapped_idx_1 - (y[0] + y[1]);
        if (fabs(diffRotation) > 3.1415926535897931) {
          TFixed_tmp = b_mod(diffRotation + 3.1415926535897931);
          if ((TFixed_tmp == 0.0) && (diffRotation + 3.1415926535897931 > 0.0))
          {
            TFixed_tmp = 6.2831853071795862;
          }

          diffRotation = TFixed_tmp - 3.1415926535897931;
        }

        eulAltUnwrapped_idx_2 = diffRotation;
        if (diffRotation < 0.0) {
          eulAltUnwrapped_idx_2 = -1.0;
        } else if (diffRotation > 0.0) {
          eulAltUnwrapped_idx_2 = 1.0;
        } else {
          if (diffRotation == 0.0) {
            eulAltUnwrapped_idx_2 = 0.0;
          }
        }

        y[1] += eulAltUnwrapped_idx_2 * fmin(fabs(diffRotation), d / 2.0);

        /*  Check if the sum of the joint angles reaches the desired total. If */
        /*  not, the solution is infeasible and a vector of NaNs is returned. */
        /* isEqualWithinTolerance Check if two matrices are equal within a set tolerance */
        /*    This is a convenience function designed for inputs with up to two */
        /*    dimensions. If the input has 3+ dimensions, a non-scalar output will be */
        /*    returned. */
        d = y[0] + y[1];
        if (fabs(d) > 3.1415926535897931) {
          TFixed_tmp = b_mod(d + 3.1415926535897931);
          if ((TFixed_tmp == 0.0) && (d + 3.1415926535897931 > 0.0)) {
            TFixed_tmp = 6.2831853071795862;
          }

          d = TFixed_tmp - 3.1415926535897931;
        }

        if (!(fabs(d - eulAltUnwrapped_idx_1) < 1.0E-6)) {
          eulAngles[0] = rtNaN;
          eulAngles[2] = rtNaN;
        } else {
          guard1 = true;
        }
      }

      if (guard1) {
        /*  Factor back in the axes signs. Since all valid joint angles are finite, */
        /*  matrix multiplication is the most efficient approach. */
        eulAngles[0] = y[0] + y[1] * 0.0;
        eulAngles[2] = y[0] * 0.0 + y[1];
      }

      /*  In this case the alternate Euler angles aren't required, as they will */
      /*  also result in a set of co-axial joints. However, to ensure codegen */
      /*  compatibility, the size must stay the same Therefore, return a set of */
      /*  NaN angles (so the second solution may be thrown out). Note that the */
      /*  axes sign is handled inside the distributeRotationOverJoints helper */
      /*  function. */
      orientationSolns[0] = eulAngles[0];
      orientationSolns[1] = rtNaN;
      orientationSolns[2] = eulAngles[1];
      orientationSolns[3] = rtNaN;
      orientationSolns[4] = eulAngles[2];
      orientationSolns[5] = rtNaN;
    } else {
      /*  For finite solutions when the middle angle is non-zero, there are two possible solutions to this problem */
      /*  that can be derived from the first solution set */
      eulAltUnwrapped_idx_0 = eulAngles[0] + 3.1415926535897931;
      eulAltUnwrapped_idx_2 = eulAngles[2] + 3.1415926535897931;
      eulAltUnwrapped_idx_1 = (-eulAngles[1] + 3.1415926535897931) -
        3.1415926535897931;

      /*  Output the angles given the axes signs */
      x_data[0] = (fabs(eulAngles[0] + 3.1415926535897931) > 3.1415926535897931);
      x_data[1] = (fabs((-eulAngles[1] + 3.1415926535897931) -
                        3.1415926535897931) > 3.1415926535897931);
      x_data[2] = (fabs(eulAngles[2] + 3.1415926535897931) > 3.1415926535897931);
      empty_non_axis_sizes = false;
      k = 0;
      exitg1 = false;
      while ((!exitg1) && (k <= 2)) {
        if (!x_data[k]) {
          k++;
        } else {
          empty_non_axis_sizes = true;
          exitg1 = true;
        }
      }

      if (empty_non_axis_sizes) {
        d = b_mod((eulAngles[0] + 3.1415926535897931) + 3.1415926535897931);
        if ((d == 0.0) && ((eulAngles[0] + 3.1415926535897931) +
                           3.1415926535897931 > 0.0)) {
          d = 6.2831853071795862;
        }

        eulAltUnwrapped_idx_0 = d - 3.1415926535897931;
        d = b_mod(((-eulAngles[1] + 3.1415926535897931) - 3.1415926535897931) +
                  3.1415926535897931);
        if ((d == 0.0) && (((-eulAngles[1] + 3.1415926535897931) -
                            3.1415926535897931) + 3.1415926535897931 > 0.0)) {
          d = 6.2831853071795862;
        }

        eulAltUnwrapped_idx_1 = d - 3.1415926535897931;
        d = b_mod((eulAngles[2] + 3.1415926535897931) + 3.1415926535897931);
        if ((d == 0.0) && ((eulAngles[2] + 3.1415926535897931) +
                           3.1415926535897931 > 0.0)) {
          d = 6.2831853071795862;
        }

        eulAltUnwrapped_idx_2 = d - 3.1415926535897931;
      }

      b_eulAngles[0] = eulAngles[0];
      b_eulAngles[1] = eulAltUnwrapped_idx_0;
      b_eulAngles[2] = eulAngles[1];
      b_eulAngles[3] = eulAltUnwrapped_idx_1;
      b_eulAngles[4] = eulAngles[2];
      b_eulAngles[5] = eulAltUnwrapped_idx_2;
      for (iacol = 0; iacol < 2; iacol++) {
        d = b_eulAngles[iacol];
        eulAltUnwrapped_idx_2 = b_eulAngles[iacol + 2];
        eulAltUnwrapped_idx_0 = b_eulAngles[iacol + 4];
        for (ibcol = 0; ibcol < 3; ibcol++) {
          orientationSolns[iacol + (ibcol << 1)] = (d * (double)c_b[3 * ibcol] +
            eulAltUnwrapped_idx_2 * (double)c_b[3 * ibcol + 1]) +
            eulAltUnwrapped_idx_0 * (double)c_b[3 * ibcol + 2];
        }
      }
    }

    ibtile = jtIdx + q123Opts_size[0];

    /*  Offset theta to reflect the source robot configuration */
    q456Opts_data[jtIdx] = orientationSolns[0];
    q456Opts_data[ibtile] = orientationSolns[1];
    ibcol = jtIdx + q456Opts_size_idx_0_tmp;
    q456Opts_data[ibcol] = orientationSolns[2];
    q456Opts_data[ibtile + q456Opts_size_idx_0_tmp] = orientationSolns[3];
    q123Opts_data[ibtile] -= -1.5707963267949;
    nrows = jtIdx + q456Opts_size_idx_0_tmp * 2;
    q456Opts_data[nrows] = orientationSolns[4];
    q456Opts_data[ibtile + q456Opts_size_idx_0_tmp * 2] = orientationSolns[5];
    iacol = jtIdx + q123Opts_size[0] * 2;
    q456Opts_data[nrows] -= -1.5707963267949;
    q456Opts_data[ibtile + q456Opts_size_idx_0_tmp * 2] -= -1.5707963267949;

    /*  Remove solutions that violate joint limits */
    if (enforceJointLimits != 0) {
      eulAngles[0] = q123Opts_data[jtIdx];
      eulAngles[1] = q123Opts_data[ibtile];
      eulAngles[2] = q123Opts_data[iacol];
      applyJointLimits(eulAngles, dv2, b_y);
      q123Opts_data[jtIdx] = b_y[0];
      q123Opts_data[ibtile] = b_y[1];
      q123Opts_data[iacol] = b_y[2];
      eulAngles[0] = q456Opts_data[jtIdx];
      eulAngles[1] = q456Opts_data[ibcol];
      eulAngles[2] = q456Opts_data[nrows];
      applyJointLimits(eulAngles, dv3, b_y);
      q456Opts_data[jtIdx] = b_y[0];
      eulAngles[0] = q456Opts_data[ibtile];
      q456Opts_data[ibcol] = b_y[1];
      eulAngles[1] = q456Opts_data[ibtile + q456Opts_size_idx_0_tmp];
      q456Opts_data[nrows] = b_y[2];
      eulAngles[2] = q456Opts_data[ibtile + q456Opts_size_idx_0_tmp * 2];
      applyJointLimits(eulAngles, dv3, b_y);
      q456Opts_data[ibtile] = b_y[0];
      q456Opts_data[ibtile + q456Opts_size_idx_0_tmp] = b_y[1];
      q456Opts_data[ibtile + q456Opts_size_idx_0_tmp * 2] = b_y[2];
    }
  }

  /*  Filter out any remaining rows with NaNs in them by getting the index of the valid rows and only assembling those in the final output */
  nrows = q123Opts_size[0];
  for (jcol = 0; jcol < 3; jcol++) {
    iacol = jcol * nrows;
    ibtile = jcol * (nrows << 1) - 1;
    for (k = 0; k < nrows; k++) {
      varargin_1_data[(ibtile + k) + 1] = q123Opts_data[iacol + k];
    }

    ibcol = ibtile + nrows;
    for (k = 0; k < nrows; k++) {
      varargin_1_data[(ibcol + k) + 1] = q123Opts_data[iacol + k];
    }
  }

  if ((signed char)q456Opts_size_idx_0_tmp != 0) {
    nrows = (signed char)q456Opts_size_idx_0_tmp;
  } else if (q456Opts_size_idx_0_tmp != 0) {
    nrows = q456Opts_size_idx_0_tmp;
  } else {
    nrows = 0;
  }

  empty_non_axis_sizes = (nrows == 0);
  if (empty_non_axis_sizes || ((signed char)q456Opts_size_idx_0_tmp != 0)) {
    input_sizes_idx_1 = 3;
  } else {
    input_sizes_idx_1 = 0;
  }

  if (empty_non_axis_sizes || (q456Opts_size_idx_0_tmp != 0)) {
    sizes_idx_1 = 3;
  } else {
    sizes_idx_1 = 0;
  }

  jtIdx = input_sizes_idx_1 + sizes_idx_1;
  ibcol = input_sizes_idx_1;
  for (i = 0; i < ibcol; i++) {
    for (iacol = 0; iacol < nrows; iacol++) {
      allSolnOpts_data[iacol + nrows * i] = varargin_1_data[iacol + (signed char)
        q456Opts_size_idx_0_tmp * i];
    }
  }

  ibcol = sizes_idx_1;
  for (i = 0; i < ibcol; i++) {
    for (iacol = 0; iacol < nrows; iacol++) {
      allSolnOpts_data[iacol + nrows * (i + input_sizes_idx_1)] =
        q456Opts_data[iacol + q456Opts_size_idx_0_tmp * i];
    }
  }

  ibcol = nrows * jtIdx;
  for (i = 0; i < ibcol; i++) {
    tmp_data[i] = rtIsNaN(allSolnOpts_data[i]);
  }

  tmp_size[0] = nrows;
  tmp_size[1] = jtIdx;
  ibcol = nrows * jtIdx;
  for (i = 0; i < ibcol; i++) {
    b_tmp_data[i] = !tmp_data[i];
  }

  all(b_tmp_data, tmp_size, isValidRowIdx_data, isValidRowIdx_size);
  ibtile = isValidRowIdx_size[0] - 1;
  trueCount = 0;
  iacol = 0;
  for (jcol = 0; jcol <= ibtile; jcol++) {
    if (isValidRowIdx_data[jcol]) {
      trueCount++;
      iidx_data[iacol] = jcol + 1;
      iacol++;
    }
  }

  for (i = 0; i < jtIdx; i++) {
    for (iacol = 0; iacol < trueCount; iacol++) {
      qOptsAllSolns_data[iacol + trueCount * i] = allSolnOpts_data
        [(iidx_data[iacol] + nrows * i) - 1];
    }
  }

  /*  Create a copy of the solutions that wraps all revolute joints to pi, then round within solution tolerance. */
  a_size[0] = trueCount;
  a_size[1] = jtIdx;
  ibcol = trueCount * jtIdx;
  if (0 <= ibcol - 1) {
    memcpy(&allSolnOpts_data[0], &qOptsAllSolns_data[0], ibcol * sizeof(double));
  }

  wrapToPi(allSolnOpts_data, a_size);

  /*  Find the indices of all unique values after wrapping to pi */
  ibcol = a_size[0] * a_size[1];
  for (i = 0; i < ibcol; i++) {
    allSolnOpts_data[i] *= 1.0E+6;
  }

  for (k = 0; k < ibcol; k++) {
    allSolnOpts_data[k] = rt_roundd_snf(allSolnOpts_data[k]);
  }

  b_a_size[0] = a_size[0];
  b_a_size[1] = a_size[1];
  for (i = 0; i < ibcol; i++) {
    a_data[i] = allSolnOpts_data[i] / 1.0E+6;
  }

  unique_rows(a_data, b_a_size, allSolnOpts_data, a_size, iidx_data,
              isValidRowIdx_size);

  /*  Select only unique solutions from the original set of solutions */
  dist_size[0] = isValidRowIdx_size[0];
  ibcol = isValidRowIdx_size[0];
  for (i = 0; i < ibcol; i++) {
    dist_data[i] = iidx_data[i];
  }

  c_sort(dist_data, dist_size);
  ibcol = jtIdx - 1;
  qOpts_size[0] = dist_size[0];
  qOpts_size[1] = jtIdx;
  for (i = 0; i <= ibcol; i++) {
    ibtile = dist_size[0];
    for (iacol = 0; iacol < ibtile; iacol++) {
      qOpts_data[iacol + qOpts_size[0] * i] = qOptsAllSolns_data[((int)
        dist_data[iacol] + trueCount * i) - 1];
    }
  }

  /*  Sort results using a distance metric */
  if (sortByDistance != 0) {
    /* sortByEuclideanDistance Sort a matrix of solution configurations relative to a reference configuration by Euclidean norm */
    /*    This function sorts a matrix of configurations using a pre-defined */
    /*    distance metric. The computed distance between any state and the */
    /*    reference state, referenceConfig, is a Euclidean norm of difference */
    /*    between a revolute joint's values which is then wrapped to [-pi, pi], */
    /*    and a displacement between a prismatic joint's values. */
    /*  Helper functions */
    /*    Copyright 2020 The MathWorks, Inc. */
    /*  Compute the distances between each configuration and the reference */
    /* , */
    RigidBodyTreeUtils_distance(referenceConfig, qOpts_data, qOpts_size,
      dist_data, dist_size);

    /*  Sort the outputs */
    d_sort(dist_data, dist_size, iidx_data, isValidRowIdx_size);
    ibcol = jtIdx - 1;
    a_size[0] = isValidRowIdx_size[0];
    for (i = 0; i <= ibcol; i++) {
      ibtile = isValidRowIdx_size[0];
      for (iacol = 0; iacol < ibtile; iacol++) {
        allSolnOpts_data[iacol + a_size[0] * i] = qOpts_data[(iidx_data[iacol] +
          qOpts_size[0] * i) - 1];
      }
    }

    qOpts_size[0] = isValidRowIdx_size[0];
    qOpts_size[1] = jtIdx;
    ibcol = isValidRowIdx_size[0] * jtIdx;
    if (0 <= ibcol - 1) {
      memcpy(&qOpts_data[0], &allSolnOpts_data[0], ibcol * sizeof(double));
    }
  }
}

/*
 * File trailer for robotIK.c
 *
 * [EOF]
 */
