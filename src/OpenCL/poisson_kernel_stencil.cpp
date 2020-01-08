#include <ap_int.h>

typedef ap_uint<512> uint512_dt;

#define MAX_SIZE_X 2048
#define MAX_DEPTH_16 (MAX_SIZE_X/16)

//user function
#define PORT_WIDTH 16
#define SHIFT_BITS 4
#define DATATYPE_SIZE 32
//#define BEAT_SHIFT_BITS 10
#define BURST_LEN MAX_DEPTH_16


typedef union  {
   int i;
   float f;
} data_conv;


static void read_row(const uint512_dt*  arg0, uint512_dt* rd_buffer, const int xdim0_poisson_kernel_stencil, const int base0, int i, int size1){

	int base_index = (base0 + ((i-1) * xdim0_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	int end_row = size1+3;
	if(i < end_row){
		for(int k =0; k < end_index; k++){
			rd_buffer[k] = arg0[base_index -1 + k];
		}
	}
}

static void process_a_row(const uint512_dt* rd_buffer, uint512_dt* wr_buffer,  uint512_dt* row1, uint512_dt* row2, uint512_dt* row3, const int size0, int size1,  const int xdim0_poisson_kernel_stencil, int i, int pipeline_stage){

	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	uint512_dt tmp1_b1, tmp2_b1, tmp3_b1;
	uint512_dt tmp1_b2, tmp2_b2, tmp3_b2;
	uint512_dt tmp1, tmp2, tmp3;
	int end_row = size1+3;
	uint512_dt update_j;

	float row_arr3[PORT_WIDTH];
	float row_arr2[PORT_WIDTH + 2];
	float row_arr1[PORT_WIDTH];
	float mem_wr[PORT_WIDTH];


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

		for(int k = 0; k < PORT_WIDTH; k++){
			data_conv tmp1, tmp2, tmp3;
			tmp1.i = tmp1_b1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			tmp2.i = tmp2_b1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			tmp3.i = tmp3_b1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);

			row_arr3[k] =  tmp1.f;
			row_arr2[k+1] = tmp2.f;
			row_arr1[k] =  tmp3.f;
		}
		data_conv tmp1_o1, tmp2_o2;
		tmp1_o1.i = tmp2_b2.range(DATATYPE_SIZE * (15 + 1) - 1, 15 * DATATYPE_SIZE);
		tmp2_o2.i = tmp2.range(DATATYPE_SIZE * (0 + 1) - 1, 0 * DATATYPE_SIZE);
		row_arr2[0] = tmp1_o1.f;
		row_arr2[PORT_WIDTH + 1] = tmp2_o2.f;



		process: for(int q = 0; q < PORT_WIDTH; q++){
			int index = (j << SHIFT_BITS) + q - 32;
			float f1 = ( (row_arr2[q])  + (row_arr2[q+2]) ) * 0.125f;
			float f2 = ( row_arr1[q]  + row_arr3[q] ) * 0.125f;
			float f3 = row_arr2[q+1] * 0.5f;
			float result  = f1 + f2 + f3;
			mem_wr[q] = (index <= 0 || index > size0 || (i <= 1 + pipeline_stage) || (i >= size1 + 2 + pipeline_stage)) ? row_arr2[q+1] : result;
		}


		for(int k = 0; k < PORT_WIDTH; k++){
			data_conv tmp;
			tmp.f = mem_wr[k];
			update_j.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		if((i >= 1 + pipeline_stage) && j >= 1 && ( i <= end_row + pipeline_stage)) {
			wr_buffer[j-1] = update_j;
		}
	}
	if((i >= 1 + pipeline_stage) && ( i <= end_row + pipeline_stage)){
		wr_buffer[end_index-1] = tmp2;
	}
}

static void write_row( uint512_dt*  arg1, const uint512_dt* wr_buffer, const int xdim1_poisson_kernel_stencil, const int base1, int i, int pipeline_stage){
	int base_index = (base1 + ((i-2 -pipeline_stage) * xdim1_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim1_poisson_kernel_stencil >> SHIFT_BITS) + 1;
	if(i >= (1 + pipeline_stage)){
		uint512_dt tmp1 = wr_buffer[0];
		for(int k =0; k < end_index; k++){
			arg1[base_index   + k] =  wr_buffer[k+1];
		}
	}
}


void process (const uint512_dt*  arg0, uint512_dt*  arg1,
		uint512_dt* row1_p1, uint512_dt* row2_p1, uint512_dt* row3_p1,
		uint512_dt* row1_p2, uint512_dt* row2_p2, uint512_dt* row3_p2,
		const int xdim0_poisson_kernel_stencil, const int base0, const int xdim1_poisson_kernel_stencil, const int base1, const int size0, int size1, int i){


	uint512_dt rd_buffer_p1[BURST_LEN + 2];
	uint512_dt rd_buffer_p2[BURST_LEN + 2];
	uint512_dt wr_buffer_p2[BURST_LEN + 2];

	read_row(arg0, rd_buffer_p1, xdim0_poisson_kernel_stencil, base0, i, size1);
	process_a_row(rd_buffer_p1, rd_buffer_p2, row1_p1, row2_p1, row3_p1, size0, size1, xdim0_poisson_kernel_stencil, i, 0);

	write_row(arg1, rd_buffer_p2, xdim1_poisson_kernel_stencil, base1, i, 0);
}



extern "C" {
void ops_poisson_kernel_stencil(
		const uint512_dt*  arg0,
		uint512_dt*  arg1,
		const int base0,
		const int base1,
		const int size0,
		const int size1,
		const int xdim0_poisson_kernel_stencil,
		const int xdim1_poisson_kernel_stencil){

	#pragma HLS INTERFACE m_axi port = arg0 offset = slave bundle = gmem
	#pragma HLS INTERFACE m_axi port = arg1 offset = slave bundle = gmem
	#pragma HLS INTERFACE s_axilite port = arg0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1 bundle = control
	#pragma HLS INTERFACE s_axilite port = base0 bundle = control
	#pragma HLS INTERFACE s_axilite port = base1 bundle = control
	#pragma HLS INTERFACE s_axilite port = size0 bundle = control
	#pragma HLS INTERFACE s_axilite port = size1 bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0_poisson_kernel_stencil bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim1_poisson_kernel_stencil bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control



	int end_row  = size1+4;


	uint512_dt row1_p1[BURST_LEN + 2];
	uint512_dt row2_p1[BURST_LEN + 2];
	uint512_dt row3_p1[BURST_LEN + 2];

	uint512_dt row1_p2[BURST_LEN + 2];
	uint512_dt row2_p2[BURST_LEN + 2];
	uint512_dt row3_p2[BURST_LEN + 2];



	loop_beats: for(int i = 0 ; i < end_row; i++){
		process(arg0, arg1, row1_p1, row2_p1, row3_p1,  row1_p2, row2_p2, row3_p2, xdim0_poisson_kernel_stencil, base0, xdim0_poisson_kernel_stencil, base0, size0, size1,  i);
	}
}
}