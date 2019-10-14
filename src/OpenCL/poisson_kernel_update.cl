//
// auto-generated by ops.py
//

#ifdef OCL_FMA
#pragma OPENCL FP_CONTRACT ON
#else
#pragma OPENCL FP_CONTRACT OFF
#endif
#pragma OPENCL EXTENSION cl_khr_fp64:enable

#include "user_types.h"
#define OPS_2D
#define OPS_API 2
#define OPS_NO_GLOBALS
#include "ops_macros.h"
#include "ops_opencl_reduction.h"

#ifndef MIN
#define MIN(a,b) ((a<b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a>b) ? (a) : (b))
#endif
#ifndef SIGN
#define SIGN(a,b) ((b<0.0) ? (a*(-1)) : (a))
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

//user function

/*__attribute__((always_inline)) void poisson_kernel_update(const ptr_double u2,
		ptr_double u) {
	OPS_ACCS(u, 0,0) = OPS_ACCS(u2, 0,0);
}*/


__kernel __attribute__ ((reqd_work_group_size(OPS_block_size_x, OPS_block_size_y, 1)))
__kernel __attribute__((vec_type_hint(double)))
__kernel __attribute__((xcl_zero_global_work_offset))

__kernel void ops_poisson_kernel_update(
		__global const double* restrict arg0,
		__global double* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_update,
		const int xdim1_poisson_kernel_update){


	int idx_y = get_global_id(1);
	int idx_x = get_global_id(0);

	//int xdim0_poisson_kernel_update = 22;
	//int xdim1_poisson_kernel_update = 22;

	if (idx_x < size0 && idx_y < size1) {
		const ptr_double ptr0 = { &arg0[base0 + idx_x * 1*1 + idx_y * 1*1 * xdim0_poisson_kernel_update], xdim0_poisson_kernel_update};
		ptr_double ptr1 = { &arg1[base1 + idx_x * 1*1 + idx_y * 1*1 * xdim1_poisson_kernel_update], xdim1_poisson_kernel_update};


		/*poisson_kernel_update(ptr0,
		              ptr1);*/
		OPS_ACCS(ptr1, 0,0) = OPS_ACCS(ptr0, 0,0);
	}

}
