//
// auto-generated by ops.py
//

#ifdef OCL_FMA
#pragma OPENCL FP_CONTRACT ON
#else
#pragma OPENCL FP_CONTRACT OFF
#endif
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#include "user_types.h"
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

void poisson_kernel_stencil(const ptr_double u, ptr_double u2) {
  double x = 0, y = 0, z = 0;

  x = OPS_ACCS(u, -4, 0, 0) * 0.01f + OPS_ACCS(u, -3, 0, 0) * 0.02f +
      OPS_ACCS(u, -2, 0, 0) * 0.03f + OPS_ACCS(u, -1, 0, 0) * 0.04f +
      OPS_ACCS(u, 0, 0, 0) * 0.05f + OPS_ACCS(u, 1, 0, 0) * 0.06f +
      OPS_ACCS(u, 2, 0, 0) * 0.07f + OPS_ACCS(u, 3, 0, 0) * 0.08f +
      OPS_ACCS(u, 4, 0, 0) * 0.09f;

  y = OPS_ACCS(u, 0, -4, 0) * 0.01f + OPS_ACCS(u, 0, -3, 0) * 0.02f +
      OPS_ACCS(u, 0, -2, 0) * 0.03f + OPS_ACCS(u, 0, -1, 0) * 0.04f +
      OPS_ACCS(u, 0, 0, 0) * 0.05f + OPS_ACCS(u, 0, 1, 0) * 0.06f +
      OPS_ACCS(u, 0, 2, 0) * 0.07f + OPS_ACCS(u, 0, 3, 0) * 0.08f +
      OPS_ACCS(u, 0, 4, 0) * 0.09f;

  z = OPS_ACCS(u, 0, 0, -4) * 0.01f + OPS_ACCS(u, 0, 0, -3) * 0.02f +
      OPS_ACCS(u, 0, 0, -2) * 0.03f + OPS_ACCS(u, 0, 0, -1) * 0.04f +
      OPS_ACCS(u, 0, 0, 0) * 0.05f + OPS_ACCS(u, 0, 0, 1) * 0.06f +
      OPS_ACCS(u, 0, 0, 2) * 0.07f + OPS_ACCS(u, 0, 0, 3) * 0.08f +
      OPS_ACCS(u, 0, 0, 4) * 0.09f;

  OPS_ACCS(u2, 0, 0, 0) = x + y + z;
}

__kernel void ops_poisson_kernel_stencil(__global const double *restrict arg0,
                                         __global double *restrict arg1,
                                         const int base0, const int base1,
                                         const int size0, const int size1,
                                         const int size2) {

  int idx_y = get_global_id(1);
  int idx_z = get_global_id(2);
  int idx_x = get_global_id(0);

  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    const ptr_double ptr0 = {
        &arg0[base0 + idx_x * 1 * 1 +
              idx_y * 1 * 1 * xdim0_poisson_kernel_stencil +
              idx_z * 1 * 1 * xdim0_poisson_kernel_stencil *
                  ydim0_poisson_kernel_stencil],
        xdim0_poisson_kernel_stencil, ydim0_poisson_kernel_stencil};
    ptr_double ptr1 = {&arg1[base1 + idx_x * 1 * 1 +
                             idx_y * 1 * 1 * xdim1_poisson_kernel_stencil +
                             idx_z * 1 * 1 * xdim1_poisson_kernel_stencil *
                                 ydim1_poisson_kernel_stencil],
                       xdim1_poisson_kernel_stencil,
                       ydim1_poisson_kernel_stencil};
    poisson_kernel_stencil(ptr0, ptr1);
  }
}