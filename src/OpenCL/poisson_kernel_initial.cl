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


#define SHIFT_BITS 4

__constant int c_min_size = (1024*64)/64;
__constant int c_max_size = (1024*1024*1024)/64;


__kernel __attribute__((xcl_dataflow))
__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
__kernel void ops_poisson_kernel_initial(
		__global float16* restrict arg0,
		const int base0,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_initial){


	const float16 f16 = (float16)(0,0,0,0,   0,0,0,0,  0,0,0,0,  0,0,0,0);

	for(int i  = 0; i < size1; i++){

		int base_index, end_index;

		v1_index: __attribute__((xcl_pipeline_loop)){
			base_index = (base0  + i* xdim0_poisson_kernel_initial - 1) >> SHIFT_BITS;
			end_index = (xdim0_poisson_kernel_initial >> SHIFT_BITS);
		}



		v1_wr: __attribute__((xcl_pipeline_loop))
		__attribute__((xcl_loop_tripcount(c_min_size, c_max_size)))
		for(int j = 1; j < (end_index -1) ; j++){
			arg0[base_index+ j] = f16;

		}

		// initial row handle
		v1_wr_initial: __attribute__((xcl_pipeline_loop)) {
			float16 tmp_f16;
			float16 tmp_wr;
			tmp_f16 = arg0[base_index];
			tmp_wr = (float16){ tmp_f16.s0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			arg0[base_index] = tmp_wr;
		}


		// end row handle
		v1_wr_end: __attribute__((xcl_pipeline_loop)) {
			float16 tmp_f16;
			float16 tmp_wr;

			tmp_f16 = arg0[base_index + end_index -1];
			float arr_f16[16] = {tmp_f16.s0, tmp_f16.s1, tmp_f16.s2, tmp_f16.s3, tmp_f16.s4, tmp_f16.s5, tmp_f16.s6, tmp_f16.s7,
								tmp_f16.s8, tmp_f16.s9, tmp_f16.sa, tmp_f16.sb, tmp_f16.sc, tmp_f16.sd, tmp_f16.se, tmp_f16.sf};
			float arr_wr_16[16];
			__attribute__((opencl_unroll_hint(16)))
			for (int p = 0; p < 16; p++){
				if(p >= ((size0+1) & 0xf)){
					arr_wr_16[p] = arr_f16[p];
				} else {
					arr_wr_16[p] = 0.0;
				}
			}

			tmp_wr = (float16){ arr_wr_16[0], arr_wr_16[1], arr_wr_16[2], arr_wr_16[3],
				arr_wr_16[4], arr_wr_16[5], arr_wr_16[6], arr_wr_16[7],
				arr_wr_16[8], arr_wr_16[9], arr_wr_16[10], arr_wr_16[11],
				arr_wr_16[12], arr_wr_16[13], arr_wr_16[14], arr_wr_16[15]};

			arg0[base_index + end_index -1] = tmp_wr;
		}

	}

}