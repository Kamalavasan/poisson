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


#define MAX_GRID_SIZE 600
#define MAX_X_DIM 512
#define P_FACTOR 64
#define SHIFT_BITS 6
#define SINGLE_RAM_SIZE 4096

typedef struct __attribute__ ((packed)) bram_cluster
{
    float u0[SINGLE_RAM_SIZE];
    float u1[SINGLE_RAM_SIZE];
    float u2[SINGLE_RAM_SIZE];
    float u3[SINGLE_RAM_SIZE];
    float u4[SINGLE_RAM_SIZE];
	float u5[SINGLE_RAM_SIZE];
	float u6[SINGLE_RAM_SIZE];
	float u7[SINGLE_RAM_SIZE];
	float u8[SINGLE_RAM_SIZE];
	float u9[SINGLE_RAM_SIZE];
	float u10[SINGLE_RAM_SIZE];
	float u11[SINGLE_RAM_SIZE];
	float u12[SINGLE_RAM_SIZE];
	float u13[SINGLE_RAM_SIZE];
	float u14[SINGLE_RAM_SIZE];
	float u15[SINGLE_RAM_SIZE];
    float u16[SINGLE_RAM_SIZE];
    float u17[SINGLE_RAM_SIZE];
    float u18[SINGLE_RAM_SIZE];
    float u19[SINGLE_RAM_SIZE];
    float u20[SINGLE_RAM_SIZE];
	float u21[SINGLE_RAM_SIZE];
	float u22[SINGLE_RAM_SIZE];
	float u23[SINGLE_RAM_SIZE];
	float u24[SINGLE_RAM_SIZE];
	float u25[SINGLE_RAM_SIZE];
	float u26[SINGLE_RAM_SIZE];
	float u27[SINGLE_RAM_SIZE];
	float u28[SINGLE_RAM_SIZE];
	float u29[SINGLE_RAM_SIZE];
	float u30[SINGLE_RAM_SIZE];
	float u31[SINGLE_RAM_SIZE];
    float u32[SINGLE_RAM_SIZE];
    float u33[SINGLE_RAM_SIZE];
    float u34[SINGLE_RAM_SIZE];
    float u35[SINGLE_RAM_SIZE];
    float u36[SINGLE_RAM_SIZE];
	float u37[SINGLE_RAM_SIZE];
	float u38[SINGLE_RAM_SIZE];
	float u39[SINGLE_RAM_SIZE];
	float u40[SINGLE_RAM_SIZE];
	float u41[SINGLE_RAM_SIZE];
	float u42[SINGLE_RAM_SIZE];
	float u43[SINGLE_RAM_SIZE];
	float u44[SINGLE_RAM_SIZE];
	float u45[SINGLE_RAM_SIZE];
	float u46[SINGLE_RAM_SIZE];
	float u47[SINGLE_RAM_SIZE];
    float u48[SINGLE_RAM_SIZE];
    float u49[SINGLE_RAM_SIZE];
    float u50[SINGLE_RAM_SIZE];
    float u51[SINGLE_RAM_SIZE];
    float u52[SINGLE_RAM_SIZE];
	float u53[SINGLE_RAM_SIZE];
	float u54[SINGLE_RAM_SIZE];
	float u55[SINGLE_RAM_SIZE];
	float u56[SINGLE_RAM_SIZE];
	float u57[SINGLE_RAM_SIZE];
	float u58[SINGLE_RAM_SIZE];
	float u59[SINGLE_RAM_SIZE];
	float u60[SINGLE_RAM_SIZE];
	float u61[SINGLE_RAM_SIZE];
	float u62[SINGLE_RAM_SIZE];
	float u63[SINGLE_RAM_SIZE];
};




__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_initial(
		struct bram_cluster u,
		const int base0,
		const int size0,
		const int size1){

	int end_loc =  (size0 >> 6) + 1;
	const int row_block = SINGLE_RAM_SIZE/MAX_X_DIM;

	for(int i  = 0; i < size1; i++){
		int base_index = base0  + i * MAX_X_DIM;

		__attribute__((opencl_unroll_hint(P_FACTOR)))
		for(int j = 1; j < end_loc ; j++){
			arg0[base_index+ j] = 0;
		}
	}

}





__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_populate(
		const int arg0,
		const int arg1,
		local float* arg3,
		local float* arg5,
		const float dx,
		const float dy,
		const int base3,
		const int base4,
		const int base5,
		int arg_idx0, int arg_idx1,
		const int size0,
		const int size1){


		v1_rd: __attribute__((xcl_pipeline_loop))
		for(int i  = 0; i < size1; i++){
			int arg_idx[2];
			double x, y;
			int base_index3, base_index4, base_index5, loop_limit;

			__attribute__((xcl_pipeline_workitems)){
				base_index3 = (base3  + i * MAX_X_DIM);
				base_index4 = (base4  + i * MAX_X_DIM);
				base_index5 = (base5  + i * MAX_X_DIM);
				arg_idx[1] = arg_idx1+ i;
			}

			__attribute__((xcl_pipeline_loop))
			__attribute__((opencl_unroll_hint(P_FACTOR)))
			for(int j = 0; j < size0; j++){

					arg_idx[0] = arg_idx0+ j;

					x = dx * (double)(arg_idx[0]+arg0);
					y = dy * (double)(arg_idx[1]+arg1);

					float f3 = myfun(native_sin(M_PI*x),native_cos(2.0*M_PI*y))-1.0;
					float f4 = -5.0*M_PI*M_PI*native_sin(M_PI*x)*native_cos(2.0*M_PI*y);
					float f5 = native_sin(M_PI*x)*native_cos(2.0*M_PI*y);

					arg3[base_index3 +j] = f3;
					arg5[base_index5 +j] = f5;
			}
		}

}

__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_stencil(
		local const float* restrict arg0,
		local float* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	float first_element = 0;

	for(int i = 0; i < size1; i++){
		int base_index1, base_index2, base_index3, base_index0;
		__attribute__((xcl_pipeline_workitems)){
			base_index1 = (base0  + (i-1) * MAX_X_DIM);
			base_index2 = (base0 + (i) * MAX_X_DIM);
			base_index3 = (base0  + (i+1) * MAX_X_DIM);
			base_index0 = (base0 + (i) * MAX_X_DIM);
		}

		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR)))
		for(int j = 0; j < size0; j++){
				float f1 = ( arg0[base_index1+j]  + arg0[base_index3+j] ) * 0.125f;
				float f2 = ( arg0[base_index2+j-1]  + arg0[base_index2+j+1] ) * 0.125f;
				float f3 = arg0[base_index2 + j] * 0.5f;
				float result  = f1 + f2 + f3;
				arg1[base_index0+j] = result;
		}
	}
}

__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_update(
		local const float* restrict arg0,
		local float* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	for(int i = 0; i < size1; i++){
		int base_index0 = base0 + i * MAX_X_DIM;
		int base_index1 = base1 + i * MAX_X_DIM;

		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR)))
		for(int j = 0; j < size0; j++){
			arg1[base_index0+j] = arg0[base_index1+j];
		}
	}

}
__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_error(
		local float* restrict arg0,
		local const float* restrict arg1,
		__global float* restrict arg2,
		__local float* scratch2,
		int r_bytes2,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	float g_sum = 0;

	int row_shift0 = 0;
	int row_shift1 = 0;
	float arr_focus[P_FACTOR] __attribute__((xcl_array_partition(complete, 1)));
	for(int i = 0; i < P_FACTOR; i++){
		arr_focus[i]   = 0;
	}

	for(int i  = 0; i < size1; i++){
			int base_index0, base_index1;
			__attribute__((xcl_pipeline_workitems)){
				base_index0 = base0  + row_shift0;
				base_index1 = base1  + row_shift1;
			}

			__attribute__((xcl_pipeline_loop))
			__attribute__((opencl_unroll_hint(P_FACTOR)))
			for(int j = 0; j < size0 ; j++){
					float diff = arg0[base_index0+j] - arg1[base_index1 + j];
					arr_focus[j%P_FACTOR] = arr_focus[j%P_FACTOR] + diff * diff;
				}
			row_shift0  = row_shift0 + MAX_X_DIM;
			row_shift1  = row_shift1 + MAX_X_DIM;

		}

		float sum32[P_FACTOR/2] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/2)))
		for(int k = 0; k < P_FACTOR/2; k++){
			sum32[k] = arr_focus[2*k] + arr_focus[2*k + 1];
		}

		float sum16[P_FACTOR/4] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/4)))
		for(int k = 0; k < P_FACTOR/4; k++){
			sum16[k] = sum32[2*k] + sum32[2*k + 1];
		}

		float sum8[P_FACTOR/8] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/8)))
		for(int k = 0; k < P_FACTOR/8; k++){
			sum8[k] = sum16[2*k] + sum16[2*k + 1];
		}

		float sum4[P_FACTOR/16] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/16)))
		for(int k = 0; k < P_FACTOR/16; k++){
			sum4[k] = sum8[2*k] + sum8[2*k + 1];
		}

		float sum2[P_FACTOR/32] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/32)))
		for(int k = 0; k < P_FACTOR/32; k++){
			sum2[k] = sum4[2*k] + sum4[2*k + 1];
		}

		float sum = sum2[0] + sum2[1];
		g_sum = sum;
		arg2[r_bytes2] = g_sum;
}



__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
__kernel void ops_poisson_kernel(
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


	struct bram_cluster u;
	struct bram_cluster u2;
	struct bram_cluster Ref;

	local float U[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(cyclic,P_FACTOR,1)));
	local float U2[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(cyclic,P_FACTOR,1)));
	local float ref[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(cyclic,P_FACTOR,1)));

	ops_poisson_kernel_populate(
			populate_arg0,
			populate_arg1,
			U,
			ref,
			populate_dx,
			populate_dy,
			0,
			0,
			0,
			populate_arg_idx0, populate_arg_idx1,
			size0+2,
			size1+2);



	ops_poisson_kernel_update(
			U,
			U2,
			0,
			0,
			size0+2,
			size1+2);



	ops_poisson_kernel_initial(
			U,
			MAX_X_DIM+1,
			size0,
			size1);


	for (int iter = 0; iter < n_iters; iter++) {

		ops_poisson_kernel_stencil(
				U,
				U2,
				MAX_X_DIM+1,
				MAX_X_DIM+1,
				size0,
				size1);

		ops_poisson_kernel_update(
				U2,
				U,
				0,
				0,
				size0+2,
				size1+2);
	}


	ops_poisson_kernel_error(
			U,
			ref,
			arg2,
			scratch2,
			r_bytes2,
			MAX_X_DIM+1,
			MAX_X_DIM+1,
			size0,
			size1);

}
