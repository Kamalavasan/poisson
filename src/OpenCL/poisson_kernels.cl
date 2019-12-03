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
#define ZERO_float 0.0;
#define INFINITY_float INFINITY;
#define ZERO_double 0.0f;
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


#define MAX_X_DIM 256
#define P_FACTOR 64
#define SHIFT_BITS 6
#define SINGLE_RAM_SIZE 4096


__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
__kernel void ops_poisson_kernel_stencil(
		__global float* arg0,
		const int populate_arg0,
		const int populate_arg1,
		const float populate_dx,
		const float populate_dy,
		int populate_arg_idx0,
		int populate_arg_idx1,
		__global float* restrict arg2,
		__local float* scratch2,
		int r_bytes2,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int n_iters){


	int ram_select = 0;

	// RAM u
	local float u1[MAX_X_DIM*MAX_X_DIM] __attribute__((xcl_array_partition(cyclic, MAX_X_DIM, 1)));


	// Reading the input
    __attribute__((xcl_pipeline_loop(1)))
	__attribute__((xcl_loop_tripcount(1*1, MAX_X_DIM*MAX_X_DIM)))
    readA: for (int itr = 0, i = 0, j = 0; itr < size0 * size0; itr++, j++) {
        if (j == size0) { j = 0; i++; }
        u1[i*MAX_X_DIM + j] = arg0[itr];
    }

    __attribute__((xcl_loop_tripcount(1, 10000, 9999)))
	for(int count = 0; count < n_iters; count++){

		float row0_rd[MAX_X_DIM] __attribute__((xcl_array_partition(complete, 1)));
		float row1_rd[MAX_X_DIM] __attribute__((xcl_array_partition(complete, 1)));
		float row2_rd[MAX_X_DIM] __attribute__((xcl_array_partition(complete, 1)));

		float row0_wr[MAX_X_DIM] __attribute__((xcl_array_partition(complete, 1)));
		float row1_wr[MAX_X_DIM] __attribute__((xcl_array_partition(complete, 1)));

		__attribute__((xcl_pipeline_workitems)){
			__attribute__((opencl_unroll_hint(MAX_X_DIM)))
			for(int j = 0; j < MAX_X_DIM; j++){
				row0_wr[j] = row0_rd[j]= u1[0*MAX_X_DIM +j];

			}

			__attribute__((opencl_unroll_hint(MAX_X_DIM)))
			for(int j = 0; j < MAX_X_DIM; j++){
				row1_rd[j]= u1[1*MAX_X_DIM +j];
			}

			__attribute__((opencl_unroll_hint(MAX_X_DIM)))
			for(int j = 0; j < MAX_X_DIM; j++){
				row2_rd[j]= u1[2*MAX_X_DIM +j];
			}
		}

		unsigned char rd_select = 0;
		unsigned char wr_select = 0;

		__attribute__((xcl_loop_tripcount(1, MAX_X_DIM)))
		__attribute__((xcl_pipeline_loop(1)))
		for(int i = 0; i < size1; i++){

			__attribute__((xcl_pipeline_loop()))
			__attribute__((opencl_unroll_hint(MAX_X_DIM-2)))
			__attribute__((xcl_loop_tripcount(1, MAX_X_DIM)))
			for(int j = 1; j < MAX_X_DIM-1; j++){
				float a1 = row0_rd[j];
				float b1 = row2_rd[j];
				float c1 = row1_rd[j-1];
				float d1 = row1_rd[j+1];
				float f1 = row1_rd[j];

				float a2 = row1_rd[j];
				float b2 = row0_rd[j];
				float c2 = row2_rd[j-1];
				float d2 = row2_rd[j+1];
				float f2 = row2_rd[j];

				float a3 = row2_rd[j];
				float b3 = row1_rd[j];
				float c3 = row0_rd[j-1];
				float d3 = row0_rd[j+1];
				float f3 = row0_rd[j];

				float result1 = (a1+b1)*0.125 + (c1+d1)*0.125 + f1*0.5;
				float result2 = (a2+b2)*0.125 + (c2+d2)*0.125 + f2*0.5;
				float result3 = (a3+b3)*0.125 + (c3+d3)*0.125 + f3*0.5;

				float result;

				switch(rd_select){
					case 0: {result = result1; break;}
					case 1: {result = result2; break;}
					case 2: {result = result3; break;}
					default: {result = result1; break;}
				}

				if(j < size0) u1[i*MAX_X_DIM + j] = result;

			}
			rd_select = rd_select + 1;
			if(rd_select >= 3) rd_select = 0;

			__attribute__((opencl_unroll_hint(MAX_X_DIM)))
			for(int j = 0; j < MAX_X_DIM; j++){
				switch(rd_select){
					case 0: {row0_rd[j]= u1[(i+1)*MAX_X_DIM +j]; break;}
					case 1: {row1_rd[j]= u1[(i+1)*MAX_X_DIM +j]; break;}
					case 2: {row2_rd[j]= u1[(i+1)*MAX_X_DIM +j]; break;}
					default: {row0_rd[j]= u1[(i+1)*MAX_X_DIM +j]; break;}
				}
			}
		}

	}

	// Reading the input
    __attribute__((xcl_pipeline_loop(1)))
    __attribute__((xcl_loop_tripcount(1*1, MAX_X_DIM*MAX_X_DIM)))
    writeA: for (int itr = 0, i = 0, j = 0; itr < size0 * size0; itr++, j++) {
        if (j == size0) { j = 0; i++; }
        arg0[itr] = u1[i*MAX_X_DIM + j];
    }

}

