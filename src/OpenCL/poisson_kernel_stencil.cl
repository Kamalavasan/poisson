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


// FiX ME
#define MAX_SIZE_X 2048
#define MAX_DEPTH_16 (MAX_SIZE_X/16)

//user function
#define PORT_WIDTH 16
#define SHIFT_BITS 4
//#define BEAT_SHIFT_BITS 10
#define BURST_LEN MAX_DEPTH_16




__constant int c_min_x = 40;
__constant int c_max_x = 8000;
__constant int c_avg_x = 8000;

__constant int c_min_size = c_min_x* c_min_x/16;
__constant int c_max_size = (c_max_x*c_max_x)/16;
__constant int c_avg_size = (c_avg_x*c_avg_x)/16;



static void read_row(__global const float16* restrict arg0, float16* rd_buffer, const int xdim0_poisson_kernel_stencil, const int base0, int i, int size1){

	int base_index = (base0 + ((i-1) * xdim0_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	int end_row = size1+3;
	if(i < end_row){
		read_row: __attribute__((xcl_pipeline_loop(1)))
		//__attribute__((xcl_loop_tripcount(BURST_LEN+2, BURST_LEN+2, BURST_LEN+2)))
		for(int k =0; k < end_index; k++){
			rd_buffer[k] = arg0[base_index -1 + k];
		}
	}
}

static void process_a_row(const float16* rd_buffer, float16* wr_buffer,  float16* row1, float16* row2, float16* row3, const int size0, int size1,  const int xdim0_poisson_kernel_stencil, int i, int pipeline_stage){

	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	float16 tmp1_b1, tmp2_b1, tmp3_b1;
	float16 tmp1_b2, tmp2_b2, tmp3_b2;
	float16 tmp1, tmp2, tmp3;
	int end_row = size1+3;

	process_row: __attribute__((xcl_pipeline_loop(1)))
	//__attribute__((xcl_loop_tripcount(BURST_LEN+2, BURST_LEN+2, BURST_LEN+2)))
	for(int j =0; j < end_index; j++){

		tmp1_b2 = tmp1_b1;
		tmp2_b2 = tmp2_b1;
		tmp3_b2 = tmp3_b1;

		tmp1_b1 = tmp1;
		tmp2_b1 = tmp2;
		tmp3_b1 = tmp3;

		if(i >= pipeline_stage+2 && (i < pipeline_stage + end_row+2)){
			tmp3 = row2[j];
			row3[j] = tmp3;
		}

		if(i >= pipeline_stage+1 && (i < pipeline_stage + end_row+1)){
			tmp2 = row1[j];
			row2[j] = tmp2;
		}

		if(i >= pipeline_stage && (i < pipeline_stage + end_row)){
			tmp1 = rd_buffer[j];
			row1[j] = tmp1;
		}



		float row_arr3[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1))) = {tmp1_b1.s0, tmp1_b1.s1, tmp1_b1.s2, tmp1_b1.s3, tmp1_b1.s4, tmp1_b1.s5, tmp1_b1.s6, tmp1_b1.s7,
										tmp1_b1.s8, tmp1_b1.s9, tmp1_b1.sa, tmp1_b1.sb, tmp1_b1.sc, tmp1_b1.sd, tmp1_b1.se, tmp1_b1.sf};

		float row_arr2[PORT_WIDTH + 2] __attribute__((xcl_array_partition(complete, 1))) = {tmp2_b2.sf, tmp2_b1.s0, tmp2_b1.s1, tmp2_b1.s2, tmp2_b1.s3, tmp2_b1.s4, tmp2_b1.s5, tmp2_b1.s6, tmp2_b1.s7,
										tmp2_b1.s8, tmp2_b1.s9, tmp2_b1.sa, tmp2_b1.sb, tmp2_b1.sc, tmp2_b1.sd, tmp2_b1.se, tmp2_b1.sf, tmp2.s0};

		float row_arr1[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1))) = {tmp3_b1.s0, tmp3_b1.s1, tmp3_b1.s2, tmp3_b1.s3, tmp3_b1.s4, tmp3_b1.s5, tmp3_b1.s6, tmp3_b1.s7,
						tmp3_b1.s8, tmp3_b1.s9, tmp3_b1.sa, tmp3_b1.sb, tmp3_b1.sc, tmp3_b1.sd, tmp3_b1.se, tmp3_b1.sf};

		float mem_wr[PORT_WIDTH] __attribute__((xcl_array_partition(complete, 1)));

		process: for(int q = 0; q < PORT_WIDTH; q++){
			int index = (j << SHIFT_BITS) + q - 32;
			float f1 = ( row_arr2[q]  + row_arr2[q+2] ) * 0.125f;
			float f2 = ( row_arr1[q]  + row_arr3[q] ) * 0.125f;
			float f3 = row_arr2[q+1] * 0.5f;
			float result  = f1 + f2 + f3;
			mem_wr[q] = (index <= 0 || index > size0 || (i <= 1 + pipeline_stage) || (i >= size1 + 2 + pipeline_stage)) ? row_arr2[q+1] : result;
		}

		float16 update_j = (float16) {mem_wr[0], mem_wr[1], mem_wr[2], mem_wr[3], mem_wr[4], mem_wr[5], mem_wr[6], mem_wr[7],
											mem_wr[8], mem_wr[9], mem_wr[10], mem_wr[11], mem_wr[12], mem_wr[13], mem_wr[14], mem_wr[15]};
		if((i >= 1 + pipeline_stage) && j >= 1 && ( i <= end_row + pipeline_stage)) {
			wr_buffer[j-1] = update_j;
		}
	}
	if((i >= 1 + pipeline_stage) && ( i <= end_row + pipeline_stage)){
		wr_buffer[end_index-1] = tmp2;
	}
}



static void write_row(__global  float16* restrict arg1, const float16* wr_buffer, const int xdim1_poisson_kernel_stencil, const int base1, int i, int pipeline_stage){
	int base_index = (base1 + ((i-2 -pipeline_stage) * xdim1_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim1_poisson_kernel_stencil >> SHIFT_BITS) + 1;
	if(i >= (1 + pipeline_stage)){
		float16 tmp1 = wr_buffer[0];
		write_row: __attribute__((xcl_pipeline_loop(1)))
		//__attribute__((xcl_loop_tripcount(BURST_LEN, BURST_LEN, BURST_LEN)))
		for(int k =0; k < end_index; k++){
			arg1[base_index   + k] =  wr_buffer[k+1];
		}
	}
}


__attribute__((xcl_dataflow))
void process(__global const float16* restrict arg0, __global float16* restrict arg1,
		float16* row1_p1, float16* row2_p1, float16* row3_p1,
		float16* row1_p2, float16* row2_p2, float16* row3_p2,
		const int xdim0_poisson_kernel_stencil, const int base0, const int xdim1_poisson_kernel_stencil, const int base1, const int size0, int size1, int i){


	float16 rd_buffer_p1[BURST_LEN + 2];
	float16 rd_buffer_p2[BURST_LEN + 2];
	float16 wr_buffer_p2[BURST_LEN + 2];

	read_row(arg0, rd_buffer_p1, xdim0_poisson_kernel_stencil, base0, i, size1);
	process_a_row(rd_buffer_p1, rd_buffer_p2, row1_p1, row2_p1, row3_p1, size0, size1, xdim0_poisson_kernel_stencil, i, 0);
	//process_a_row(rd_buffer_p2, wr_buffer_p2,  row1_p2, row2_p2, row3_p2, size0, size1, xdim0_poisson_kernel_stencil, i, 1);

	write_row(arg1, rd_buffer_p2, xdim1_poisson_kernel_stencil, base1, i, 0);
}


__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
__attribute__ ((xcl_dataflow))
__kernel void ops_poisson_kernel_stencil(
		__global const float16* restrict arg0,
		__global float16* restrict arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_stencil,
		const int xdim1_poisson_kernel_stencil){


	int end_row  = size1+4;


	float16 row1_p1[BURST_LEN + 2];
	float16 row2_p1[BURST_LEN + 2];
	float16 row3_p1[BURST_LEN + 2];

	float16 row1_p2[BURST_LEN + 2];
	float16 row2_p2[BURST_LEN + 2];
	float16 row3_p2[BURST_LEN + 2];


	__attribute__((xcl_dataflow))
	__attribute__((xcl_loop_tripcount(c_min_x, c_max_x, c_avg_x)))
	loop_beats: for(int i = 0 ; i < end_row; i++){
		process(arg0, arg1, row1_p1, row2_p1, row3_p1,  row1_p2, row2_p2, row3_p2, xdim0_poisson_kernel_stencil, base0, xdim0_poisson_kernel_stencil, base0, size0, size1,  i);
	}
}
