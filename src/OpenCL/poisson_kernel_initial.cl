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

/*__attribute__((always_inline)) void poisson_kernel_initial(ptr_double u) {
	OPS_ACCS(u, 0,0) = 0.0;
}*/


__kernel void ops_poisson_kernel_initial(
		__global double* restrict arg0,
		const int base0,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_initial){


	int idx_y = get_global_id(1);
	int idx_x = get_global_id(0);

	//int xdim0_poisson_kernel_initial = 22;

	if (idx_x < size0 && idx_y < size1) {
		ptr_double ptr0 = { &arg0[base0 + idx_x * 1*1 + idx_y * 1*1 * xdim0_poisson_kernel_initial], xdim0_poisson_kernel_initial};
		//poisson_kernel_initial(ptr0);
		OPS_ACCS(ptr0, 0,0) = 0.0;
	}

}
