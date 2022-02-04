/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_robotIK_api.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 03-Feb-2022 15:22:49
 */

/* Include Files */
#include "_coder_robotIK_api.h"
#include "_coder_robotIK_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131595U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "robotIK",                           /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[16];
static uint8_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *enforceJointLimits, const char_T *identifier);
static uint8_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static real_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[16];
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *eeTform,
  const char_T *identifier))[16];
static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2]);
static uint8_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);

/* Function Definitions */
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[16]
 */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[16]
{
  real_T (*y)[16];
  y = e_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *enforceJointLimits
 *                const char_T *identifier
 * Return Type  : uint8_T
 */
  static uint8_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *enforceJointLimits, const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  uint8_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(enforceJointLimits), &thisId);
  emlrtDestroyArray(&enforceJointLimits);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint8_T
 */
static uint8_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint8_T y;
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[16]
 */
static real_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[16]
{
  static const int32_T dims[2] = { 4, 4 };

  real_T (*ret)[16];
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims);
  ret = (real_T (*)[16])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *eeTform
 *                const char_T *identifier
 * Return Type  : real_T (*)[16]
 */
  static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *eeTform,
  const char_T *identifier))[16]
{
  emlrtMsgIdentifier thisId;
  real_T (*y)[16];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(eeTform), &thisId);
  emlrtDestroyArray(&eeTform);
  return y;
}

/*
 * Arguments    : const real_T u_data[]
 *                const int32_T u_size[2]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2])
{
  static const int32_T iv[2] = { 0, 0 };

  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateNumericArray(2, &iv[0], mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u_data[0]);
  emlrtSetDimensions((mxArray *)m, u_size, 2);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : uint8_T
 */
static uint8_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  uint8_T ret;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "uint8", false, 0U, &dims);
  ret = *(uint8_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray * const prhs[4]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void robotIK_api(const mxArray * const prhs[4], const mxArray *plhs[1])
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  real_T (*qOpts_data)[192];
  real_T (*eeTform)[16];
  real_T (*referenceConfig)[16];
  int32_T qOpts_size[2];
  uint8_T enforceJointLimits;
  uint8_T sortByDistance;
  st.tls = emlrtRootTLSGlobal;
  qOpts_data = (real_T (*)[192])mxMalloc(sizeof(real_T [192]));

  /* Marshall function inputs */
  eeTform = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "eeTform");
  enforceJointLimits = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]),
    "enforceJointLimits");
  sortByDistance = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]),
    "sortByDistance");
  referenceConfig = emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "referenceConfig");

  /* Invoke the target function */
  robotIK(*eeTform, enforceJointLimits, sortByDistance, *referenceConfig,
          *qOpts_data, qOpts_size);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(*qOpts_data, qOpts_size);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void robotIK_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  robotIK_xil_terminate();
  robotIK_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void robotIK_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void robotIK_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_robotIK_api.c
 *
 * [EOF]
 */
