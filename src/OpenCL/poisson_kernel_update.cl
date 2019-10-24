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


#define SHIFT_BITS 4

__constant int c_min_size = (1024*1024)/64;
__constant int c_max_size = (1024*1024*1024)/64;
__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))


__kernel void ops_poisson_kernel_update(
		__global const float16* restrict arg0,
		__global float16* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_update,
		const int xdim1_poisson_kernel_update){




	int base_index0, base_index1, end_index;

	for(int i  = 0; i < size1; i++){
		__attribute__((xcl_pipeline_workitems)){
			base_index0 = (base0 + i* xdim0_poisson_kernel_update -1) >> SHIFT_BITS;
			base_index1 = (base1 + i* xdim1_poisson_kernel_update -1) >> SHIFT_BITS;
			end_index = (xdim0_poisson_kernel_update >> SHIFT_BITS);
		}
		__attribute__((xcl_pipeline_loop(1)))
		__attribute__((xcl_loop_tripcount(c_min_size, c_max_size)))
		for(int j = 0; j < end_index; j++){
			float16 temp = arg0[base_index0 +j];
			arg1[base_index1 +j] = temp;

		}

	}

}
