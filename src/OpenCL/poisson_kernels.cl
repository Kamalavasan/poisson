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
#define P_FACTOR 64


__attribute__((xcl_dataflow))
__attribute__((noinline))
void ops_poisson_kernel_initial(
		float* restrict arg0,
		const int base0,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_initial){


	for(int i  = 0; i < size1; i++){

		int base_index = base0  + i* xdim0_poisson_kernel_initial;
		__attribute__((opencl_unroll_hint(P_FACTOR)))
		for(int j = 1; j < size0 ; j++){
			arg0[base_index+ j] = 0;
		}
	}

}

__attribute__((xcl_dataflow))
__attribute__((noinline))
void ops_poisson_kernel_populate(
		const int arg0,
		const int arg1,
		float* arg3,
		float* arg5,
		const float dx,
		const float dy,
		const int base3,
		const int base4,
		const int base5,
		int arg_idx0, int arg_idx1,
		const int size0,
		const int size1,
		const int xdim3_poisson_kernel_populate,
		const int xdim4_poisson_kernel_populate,
		const int xdim5_poisson_kernel_populate){


		v1_rd: __attribute__((xcl_pipeline_loop))
		for(int i  = 0; i < size1; i++){
			int arg_idx[2];
			double x, y;
			int base_index3, base_index4, base_index5, loop_limit;

			__attribute__((xcl_pipeline_workitems)){
				base_index3 = (base3  + i* xdim3_poisson_kernel_populate);
				base_index4 = (base4  + i* xdim4_poisson_kernel_populate);
				base_index5 = (base5  + i* xdim5_poisson_kernel_populate);
				arg_idx[1] = arg_idx1+ i;
			}

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
__attribute__((noinline))
void ops_poisson_kernel_stencil(
		const float* restrict arg0,
		float* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_stencil,
		const int xdim1_poisson_kernel_stencil){


	float first_element = 0;

	for(int i = 0; i < size1; i++){
		int base_index1, base_index2, base_index3, base_index0;
		__attribute__((xcl_pipeline_workitems)){
			base_index1 = (base0  + (i-1) * xdim0_poisson_kernel_stencil);
			base_index2 = (base0 + (i) * xdim0_poisson_kernel_stencil);
			base_index3 = (base0  + (i+1) * xdim0_poisson_kernel_stencil);
			base_index0 = (base0 + (i) * xdim0_poisson_kernel_stencil);
		}


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
__attribute__((noinline))
void ops_poisson_kernel_update(
		const uint* restrict arg0,
		uint* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_update,
		const int xdim1_poisson_kernel_update){


	for(int i = 0; i < size1; i++){
		int base_index0 = base0 + i * xdim0_poisson_kernel_update;
		int base_index1 = base1 + i * xdim1_poisson_kernel_update;

		__attribute__((opencl_unroll_hint(P_FACTOR)))
		for(int j = 0; j < size0; j++){
			arg1[base_index0+j] = arg0[base_index1+j];
		}
	}

}
__attribute__((xcl_dataflow))
__attribute__((noinline))
void ops_poisson_kernel_error(
		float* restrict arg0,
		const float* restrict arg1,
		__global float* restrict arg2,
		__local float* scratch2,
		int r_bytes2,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_error,
		const int xdim1_poisson_kernel_error){


	float g_sum = 0;

	int row_shift0 = 0;
	int row_shift1 = 0;
	float arr_focus[P_FACTOR] __attribute__((xcl_array_partition(complete, 1)));
	for(int i  = 0; i < size1; i++){
			int base_index0, base_index1;
			__attribute__((xcl_pipeline_workitems)){
				base_index0 = base0  + row_shift0;
				base_index1 = base1  + row_shift1;
			}


			__attribute__((opencl_unroll_hint(P_FACTOR)))
			for(int j = 0; j < size0 ; j++){
					float diff = arg0[base_index0+j] - arg1[base_index1 + j];
					arr_focus[j%P_FACTOR] = arr_focus[j%P_FACTOR] + diff * diff;
				}
			row_shift0  = row_shift0 + xdim0_poisson_kernel_error;
			row_shift1  = row_shift1 + xdim1_poisson_kernel_error;

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
		const int xdim_poisson_kernel,
		const int n_iters){

	float U[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(block,P_FACTOR,1)));
	float U2[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(block,P_FACTOR,1)));
	float ref[MAX_GRID_SIZE * MAX_GRID_SIZE] __attribute__((xcl_array_partition(block,P_FACTOR,1)));

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
			size1+2,
			xdim_poisson_kernel,
			xdim_poisson_kernel,
			xdim_poisson_kernel);



	ops_poisson_kernel_update(
			U,
			U2,
			0,
			0,
			size0+2,
			size1+2,
			xdim_poisson_kernel,
			xdim_poisson_kernel);



	ops_poisson_kernel_initial(
			U,
			xdim_poisson_kernel+1,
			size0,
			size1,
			xdim_poisson_kernel);


	for (int iter = 0; iter < n_iters; iter++) {

		ops_poisson_kernel_stencil(
				U,
				U2,
				xdim_poisson_kernel+1,
				xdim_poisson_kernel+1,
				size0,
				size1,
				xdim_poisson_kernel,
				xdim_poisson_kernel);

		ops_poisson_kernel_update(
				U2,
				U,
				0,
				0,
				size0+2,
				size1+2,
				xdim_poisson_kernel,
				xdim_poisson_kernel);
	}


	ops_poisson_kernel_error(
			U,
			ref,
			arg2,
			scratch2,
			r_bytes2,
			xdim_poisson_kernel+1,
			xdim_poisson_kernel+1,
			size0,
			size1,
			xdim_poisson_kernel,
			xdim_poisson_kernel);

}
