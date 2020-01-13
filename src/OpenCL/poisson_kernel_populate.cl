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


#define SHIFT_BITS 4
#define PORT_WIDTH 16

__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))

__kernel void ops_poisson_kernel_populate(
		const int arg0,
		const int arg1,
		__global float16* restrict arg3,
		__global float16* restrict arg4,
		__global float16* restrict arg5,
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


		int beat_no = (size0 >> SHIFT_BITS) + 1;
		local float mem3[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1)));
		local float mem4[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1)));
		local float mem5[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1)));


		// arg3
		for(int i  = 0; i < size1; i++){
			int arg_idx[2];
			double x, y;
			int base_index3,end_index;

			__attribute__((xcl_pipeline_workitems)){
				base_index3 = (base3 +  i* xdim3_poisson_kernel_populate) >> SHIFT_BITS;
				arg_idx[1] = arg_idx1+ i;
				end_index = (xdim3_poisson_kernel_populate >> SHIFT_BITS);
			}
			//__attribute__((xcl_pipeline_loop))
			for(int j = 0; j < end_index; j++){

				v3_rd: __attribute__((xcl_pipeline_loop))
				__attribute__((opencl_unroll_hint(PORT_WIDTH/4)))
				for(int k = 0; k < PORT_WIDTH; k++){
					int index_x = (j<<SHIFT_BITS) + k;

					arg_idx[0] = arg_idx0+ index_x;

					x = dx * (double)(arg_idx[0]+arg0);
					y = dy * (double)(arg_idx[1]+arg1);

					float f3 = myfun(native_sin(M_PI*x),native_cos(2.0*M_PI*y))-1.0;
					mem3[k] = f3;
				}
				float16 f3_16 = (float16) {mem3[0], mem3[1], mem3[2], mem3[3], mem3[4], mem3[5], mem3[6], mem3[7], mem3[8], mem3[9], mem3[10], mem3[11], mem3[12], mem3[13], mem3[14], mem3[15]};
				arg3[base_index3 +j] = f3_16;
			}
		}

		// arg4
//		for(int i  = 0; i < size1; i++){
//			int arg_idx[2];
//			double x, y;
//			int base_index4,end_index;
//			__attribute__((xcl_pipeline_workitems)){
//
//				base_index4 = (base4 +  i* xdim4_poisson_kernel_populate) >> SHIFT_BITS;
//				arg_idx[1] = arg_idx1+ i;
//				end_index = (xdim3_poisson_kernel_populate >> SHIFT_BITS);
//
//			}
//			__attribute__((xcl_pipeline_loop))
//			for(int j = 0; j < end_index; j++){
//				v4_rd: __attribute__((xcl_pipeline_loop))
//				__attribute__((opencl_unroll_hint(PORT_WIDTH)))
//				for(int k = 0; k < PORT_WIDTH; k++){
//					int index_x = (j<<SHIFT_BITS) + k;
//					arg_idx[0] = arg_idx0+ index_x;
//
//					x = dx * (double)(arg_idx[0]+arg0);
//					y = dy * (double)(arg_idx[1]+arg1);
//
//					float f4 = -5.0*M_PI*M_PI*native_sin(M_PI*x)*native_cos(2.0*M_PI*y);
//					mem4[k] = f4;
//				}
//				float16 f4_16 = (float16) {mem4[0], mem4[1], mem4[2], mem4[3], mem4[4], mem4[5], mem4[6], mem4[7], mem4[8], mem4[9], mem4[10], mem4[11], mem4[12], mem4[13], mem4[14], mem4[15]};
//				arg4[base_index4 +j] = f4_16;;
//			}
//		}

		// arg5
		for(int i  = 0; i < size1; i++){
			int arg_idx[2];
			double x, y;
			int base_index5 ,end_index;
			__attribute__((xcl_pipeline_workitems)){
				base_index5 = (base5 +  i* xdim5_poisson_kernel_populate) >> SHIFT_BITS;
				arg_idx[1] = arg_idx1+ i;
				end_index = (xdim3_poisson_kernel_populate >> SHIFT_BITS);

			}
			//__attribute__((xcl_pipeline_loop))
			for(int j = 0; j < end_index; j++){
				v5_rd: __attribute__((xcl_pipeline_loop))
				__attribute__((opencl_unroll_hint(PORT_WIDTH/4)))
				for(int k = 0; k < PORT_WIDTH; k++){
					int index_x = (j<<SHIFT_BITS) + k;

					arg_idx[0] = arg_idx0+ index_x;

					x = dx * (double)(arg_idx[0]+arg0);
					y = dy * (double)(arg_idx[1]+arg1);
					float f5 = native_sin(M_PI*x)*native_cos(2.0*M_PI*y);
					mem5[k] = f5;
				}
				float16 f5_16 = (float16) {mem5[0], mem5[1], mem5[2], mem5[3], mem5[4], mem5[5], mem5[6], mem5[7], mem5[8], mem5[9], mem5[10], mem5[11], mem5[12], mem5[13], mem5[14], mem5[15]};
				arg5[base_index5 +j] = f5_16;
			}

		}
}
