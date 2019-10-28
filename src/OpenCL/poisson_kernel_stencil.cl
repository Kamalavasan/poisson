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
#define PORT_WIDTH 16
#define SHIFT_BITS 4
#define BEAT_SHIFT_BITS 10
#define BURST_LEN 64

// FiX ME
#define MAX_SIZE1 10000

__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
__kernel void ops_poisson_kernel_stencil(
		__global const float16* restrict arg0,
		__global float16* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_stencil,
		const int xdim1_poisson_kernel_stencil){



//	int idx_y = get_global_id(1);
//	int idx_x = get_global_id(0);

	int beat_no = (size0 >> BEAT_SHIFT_BITS) + 1;


	local float16 mem_rd1[BURST_LEN];
	local float16 mem_rd2[BURST_LEN+1];
	local float16 mem_rd3[BURST_LEN];

	local float16 mem_row_wr[BURST_LEN];

	local float	first_element[MAX_SIZE1];


	double last_element	= 0;

	int base_index1, base_index2, base_index3, base_index0, end_index;
	double f1, f2, f3;


	for(int i  = 0; i < beat_no; i++) {
		int loop_limit;
		__attribute__((xcl_pipeline_workitems)){
			base_index1 = (base0  - xdim0_poisson_kernel_stencil + (i << BEAT_SHIFT_BITS) - 1) >> SHIFT_BITS;
			base_index2 = (base0 + (i << BEAT_SHIFT_BITS)) >> SHIFT_BITS;
			base_index3 = (base0 + xdim0_poisson_kernel_stencil + (i << BEAT_SHIFT_BITS) - 1) >> SHIFT_BITS;
			base_index0 = (base1 + (i << BEAT_SHIFT_BITS)  - 1) >> SHIFT_BITS;
			int cond = (size0 > ((i+1) << BEAT_SHIFT_BITS)) ? 1 : 0;
			int adjust_burst = ((size0 - (i << BEAT_SHIFT_BITS)) >> SHIFT_BITS) + 1;
			loop_limit = cond ? BURST_LEN : adjust_burst;
		}



		v1_row1_read: __attribute__((xcl_pipeline_loop))
		for(int k = 0; k < loop_limit; k++){
			mem_rd1[k] = arg0[base_index1 + k];
		}

		v1_row2_read: __attribute__((xcl_pipeline_loop))
		for(int k = 0; k < loop_limit + 1; k++){
			mem_rd2[k] = arg0[base_index2 + k];
		}

		v1_row3_read: __attribute__((xcl_pipeline_loop))
		for(int k = 0; k < loop_limit; k++){
			mem_rd3[k] = arg0[base_index3 + k];
		}

		local float16* ptr1;
		local float16* ptr2;
		local float16* ptr3;

		// initialising first element
		for (int k = 0; k < size1; k++){
			first_element[k] = 0;
		}

		int state = 0;

		// initial pointers
		ptr1 = mem_rd1;
		ptr2 = mem_rd2;
		ptr3 = mem_rd3;

		for(int j = 0; j < size1; j++){
			for(int p = 0; p < loop_limit; p++){
				float16 row1 = ptr1[p];
				float16 row2 = ptr2[p];
				float16 row3 = ptr3[p];

				float16 row2_n = ptr2[p+1];

				float row_arr1[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1))) = {row1.s0, row1.s1, row1.s2, row1.s3, row1.s4, row1.s5, row1.s6, row1.s7,
												row1.s8, row1.s9, row1.sa, row1.sb, row1.sc, row1.sd, row1.se, row1.sf};

				float row_arr2[PORT_WIDTH + 2] __attribute__((xcl_array_partition(complete, 1))) = {first_element[j], row2.s0, row2.s1, row2.s2, row2.s3, row2.s4, row2.s5, row2.s6, row2.s7,
												row2.s8, row2.s9, row2.sa, row2.sb, row2.sc, row2.sd, row2.se, row2.sf, row2_n.s0};

				float row_arr3[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1))) = {row3.s0, row3.s1, row3.s2, row3.s3, row3.s4, row3.s5, row3.s6, row3.s7,
								row3.s8, row3.s9, row3.sa, row3.sb, row3.sc, row3.sd, row3.se, row3.sf};

				float mem_wr[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1)));

				process: __attribute__((xcl_pipeline_loop))
				__attribute__((opencl_unroll_hint(PORT_WIDTH)))
				for(int q = 0; q < PORT_WIDTH; q++){
					int index = (i << BEAT_SHIFT_BITS) + (p << SHIFT_BITS) + q;
					float f1 = ( row_arr2[q]  + row_arr2[q+2] )*0.125f;
					float f2 = ( row_arr1[q]  + row_arr3[q] )*0.125f;
					float f3 = row_arr2[q+1] * 0.5f;
					float result  = f1 + f2 + f3;
					mem_wr[q] = (index == 0 || index > size0) ? row_arr2[q+1] : result;
				}
				first_element[j] = row2.sf;
				float16 row_wr = (float16) {mem_wr[0], mem_wr[1], mem_wr[2], mem_wr[3], mem_wr[4], mem_wr[5], mem_wr[6], mem_wr[7],
												mem_wr[8], mem_wr[9], mem_wr[10], mem_wr[11], mem_wr[12], mem_wr[13], mem_wr[14], mem_wr[15]};

				mem_row_wr[p]  = row_wr;

			}

			v1_wr: __attribute__((xcl_pipeline_loop))
			for(int k = 0; k < loop_limit; k++){
				arg1[base_index0 +k] = mem_row_wr[k];
			}
			base_index0  = base_index0 + (xdim0_poisson_kernel_stencil >> SHIFT_BITS);

			state = state + 1;
			if(state == 3 ){
				state = 0;
			}

			switch(state){
				case 0: {ptr1 = mem_rd1; ptr2 = mem_rd2; ptr3 = mem_rd3; break; }
				case 1: {ptr1 = mem_rd2; ptr2 = mem_rd3; ptr3 = mem_rd1; break; }
				case 2: {ptr1 = mem_rd3; ptr2 = mem_rd1; ptr3 = mem_rd2; break; }
				default: {ptr1 = mem_rd1; ptr2 = mem_rd2; ptr3 = mem_rd3; break; }
			}

			base_index3  = base_index3 + (xdim0_poisson_kernel_stencil >> SHIFT_BITS);
			v2_row3_read: __attribute__((xcl_pipeline_loop))
			for(int k = 0; k < loop_limit; k++){
				ptr3[k] = arg0[base_index3 + k];
			}

		}
	}
}
