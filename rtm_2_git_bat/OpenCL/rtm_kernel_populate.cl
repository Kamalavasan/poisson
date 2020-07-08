//
// auto-generated by ops.py
//

#ifdef OCL_FMA
#pragma OPENCL FP_CONTRACT ON
#else
#pragma OPENCL FP_CONTRACT OFF
#endif
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#define OPS_3D
#define OPS_API 2
#define OPS_NO_GLOBALS
#include "ops_macros.h"
#include "ops_opencl_reduction.h"

#ifndef MIN
#define MIN(a, b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a > b) ? (a) : (b))
#endif
#ifndef SIGN
#define SIGN(a, b) ((b < 0.0) ? (a * (-1)) : (a))
#endif
#define OPS_READ 0
#define OPS_WRITE 1
#define OPS_RW 2
#define OPS_INC 3
#define OPS_MIN 4
#define OPS_MAX 5
#define ZERO_double 0.0;
#define INFINITY_double INFINITY;
#define ZERO_float 0.0f;
#define INFINITY_float INFINITY;
#define ZERO_int 0;
#define INFINITY_int INFINITY;
#define ZERO_uint 0;
#define INFINITY_uint INFINITY;
#define ZERO_ll 0;
#define INFINITY_ll INFINITY;
#define ZERO_ull 0;
#define INFINITY_ull INFINITY;
#define ZERO_bool 0;

// user function

void rtm_kernel_populate(const int *dispx, const int *dispy, const int *dispz,
                         const int *idx, ptr_float rho, ptr_float mu,
                         ptrm_float yy, const int nx, const int ny,
                         const int nz) {

  float x = 1.0 * ((float)(idx[0] - nx / 2) / nx);
  float y = 1.0 * ((float)(idx[1] - ny / 2) / ny);
  float z = 1.0 * ((float)(idx[2] - nz / 2) / nz);

  const float C = 1.0f;
  const float r0 = 0.001f;
  OPS_ACCS(rho, 0, 0, 0) = 1000.0f;
  OPS_ACCS(mu, 0, 0, 0) = 0.001f;

  OPS_ACCM(yy, 0, 0, 0, 0) = (1. / 3.) * C * exp(-(x * x + y * y + z * z) / r0);
}

__kernel void ops_rtm_kernel_populate(
    const int arg0, const int arg1, const int arg2,
    __global float *restrict arg4, __global float *restrict arg5,
    __global float *restrict arg6, const int nx, const int ny, const int nz,
    const int base4, const int base5, const int base6, int arg_idx0,
    int arg_idx1, int arg_idx2, const int size0, const int size1,
    const int size2) {

  int idx_y = get_global_id(1);
  int idx_z = get_global_id(2);
  int idx_x = get_global_id(0);

  int arg_idx[3];
  arg_idx[0] = arg_idx0 + idx_x;
  arg_idx[1] = arg_idx1 + idx_y;
  arg_idx[2] = arg_idx2 + idx_z;
  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    ptr_float ptr4 = {&arg4[base4 + idx_x * 1 * 1 +
                            idx_y * 1 * 1 * xdim4_rtm_kernel_populate +
                            idx_z * 1 * 1 * xdim4_rtm_kernel_populate *
                                ydim4_rtm_kernel_populate],
                      xdim4_rtm_kernel_populate, ydim4_rtm_kernel_populate};
    ptr_float ptr5 = {&arg5[base5 + idx_x * 1 * 1 +
                            idx_y * 1 * 1 * xdim5_rtm_kernel_populate +
                            idx_z * 1 * 1 * xdim5_rtm_kernel_populate *
                                ydim5_rtm_kernel_populate],
                      xdim5_rtm_kernel_populate, ydim5_rtm_kernel_populate};
#ifdef OPS_SOA
    ptrm_float ptr6 = {&arg6[base6 + idx_x * 1 * 6 +
                             idx_y * 1 * 6 * xdim6_rtm_kernel_populate +
                             idx_z * 1 * 6 * xdim6_rtm_kernel_populate *
                                 ydim6_rtm_kernel_populate],
                       xdim6_rtm_kernel_populate, ydim6_rtm_kernel_populate,
                       zdim6_rtm_kernel_populate};
#else
    ptrm_float ptr6 = {&arg6[base6 + idx_x * 1 * 6 +
                             idx_y * 1 * 6 * xdim6_rtm_kernel_populate +
                             idx_z * 1 * 6 * xdim6_rtm_kernel_populate *
                                 ydim6_rtm_kernel_populate],
                       xdim6_rtm_kernel_populate, ydim6_rtm_kernel_populate, 6};
#endif
    rtm_kernel_populate(&arg0, &arg1, &arg2, arg_idx, ptr4, ptr5, ptr6, nx, ny,
                        nz);
  }
}
