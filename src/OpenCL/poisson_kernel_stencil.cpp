#include <ap_int.h>
#include <hls_stream.h>

typedef ap_uint<512> uint512_dt;

#define MAX_SIZE_X 2048
#define MAX_DEPTH_16 (MAX_SIZE_X/16)

//user function
#define PORT_WIDTH 16
#define SHIFT_BITS 4
#define DATATYPE_SIZE 32
//#define BEAT_SHIFT_BITS 10
#define BURST_LEN MAX_DEPTH_16

#define STAGES 2

const int max_size_y = 2000;
const int min_size_y = 20;
const int avg_size_y = 2000;

const int max_block_x = 2000/16 + 1;
const int min_block_x = 20/16 + 1;
const int avg_block_x = 2000/16 + 1;

const int max_grid = max_block_x * max_size_y;
const int min_grid = min_block_x * min_size_y;
const int avg_grid = avg_block_x * avg_size_y;

const int port_width  = PORT_WIDTH;
const int max_depth_16 = MAX_DEPTH_16*2;


typedef union  {
   int i;
   float f;
} data_conv;

static void read_row(const uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer, const int xdim0_poisson_kernel_stencil, const int base0, int size1, const int count){
	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS);
	int end_row = size1+3;
	int base_index = (base0 + ((0-1) * xdim0_poisson_kernel_stencil) -1) >> SHIFT_BITS;


	for (int itr = 0; itr < end_row * end_index * count; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		rd_buffer << arg0[base_index + itr];
	}
}

static void process_a_row( hls::stream<uint512_dt> &rd_buffer, hls::stream<uint512_dt> &wr_buffer, const int size0, int size1,  const int xdim0_poisson_kernel_stencil, const int count){


	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	uint512_dt tmp1_b1, tmp2_b1, tmp3_b1;
	uint512_dt tmp1_b2, tmp2_b2, tmp3_b2;
	uint512_dt tmp1, tmp2, tmp3;
	uint512_dt update_j;



	float row_arr3[PORT_WIDTH];
	float row_arr2[PORT_WIDTH + 2];
	float row_arr1[PORT_WIDTH];
	float mem_wr[PORT_WIDTH];

	#pragma HLS ARRAY_PARTITION variable=row_arr3 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=row_arr2 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=row_arr1 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=mem_wr complete dim=1



	hls::stream<uint512_dt> row1_n("row1_n");
	hls::stream<uint512_dt> row2_n("row2_n");
	hls::stream<uint512_dt> row3_n("row3_n");

	#pragma HLS STREAM variable = row1_n depth = max_depth_16
	#pragma HLS STREAM variable = row2_n depth = max_depth_16
	#pragma HLS STREAM variable = row3_n depth = max_depth_16


	int pipeline_stage  = 0;
	int end_row = size1+3;
	int outer_loop_limit = size1+5;

	for(int itr = 0; itr < outer_loop_limit * end_index * count; itr++) {
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=2

		int i = (itr / end_index) % outer_loop_limit;
		int j = itr % end_index;


		tmp1_b2 = tmp1_b1;
		tmp2_b2 = tmp2_b1;
		tmp3_b2 = tmp3_b1;

		tmp1_b1 = tmp1;
		tmp2_b1 = tmp2;
		tmp3_b1 = tmp3;

		if(i >= pipeline_stage+2 && (i < pipeline_stage + end_row+2) && (j != 0 && j != end_index -1) ){
			tmp3 = row2_n.read();

		}

		if(i >= pipeline_stage+1 && (i < pipeline_stage + end_row+1) && (j != 0 && j != end_index -1)){
			tmp2 = row1_n.read();
			row2_n << tmp2;
		}

		if(i >= pipeline_stage && (i < pipeline_stage + end_row) && (j != 0 && j != end_index -1) ){
			tmp1 = rd_buffer.read();
			row1_n << tmp1;
		}

		vec2arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
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
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			int index = (j << SHIFT_BITS) + q - 32;
			float f1 = ( (row_arr2[q])  + (row_arr2[q+2]) ) * 0.125f;
			float f2 = ( row_arr1[q]  + row_arr3[q] ) * 0.125f;
			float f3 = row_arr2[q+1] * 0.5f;
			float result  = f1 + f2 + f3;
			mem_wr[q] = (index <= 0 || index > size0 || (i <= 1 + pipeline_stage) || (i >= size1 + 2 + pipeline_stage)) ? row_arr2[q+1] : result;
		}


		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.f = mem_wr[k];
			update_j.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		if( (i >= 1 + pipeline_stage) && j > 1 && ( i <= end_row + pipeline_stage)) {
			wr_buffer << update_j;
		}

	}
}

static void write_row( uint512_dt*  arg1, hls::stream<uint512_dt> &wr_buffer, const int xdim1_poisson_kernel_stencil, const int base1, int size1, const int count){
	int end_index = (xdim1_poisson_kernel_stencil >> SHIFT_BITS);
	int base_index = (base1 + ((0-1) * xdim1_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int End_row = size1+3;
	for (int itr = 0; itr < End_row * (end_index) * count; itr++){
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=1
		arg1[base_index   + itr] =  wr_buffer.read();
	}
}


void process (const uint512_dt*  arg0, uint512_dt*  arg1,
		const int xdim0_poisson_kernel_stencil, const int base0, const int xdim1_poisson_kernel_stencil, const int base1, const int size0, int size1, int count){


    static hls::stream<uint512_dt> streamArray[4 + 1];
    static hls::stream<uint512_dt> rd_buffer;
    static hls::stream<uint512_dt> wr_buffer;

    #pragma HLS STREAM variable = streamArray depth = 1
	#pragma HLS STREAM variable = rd_buffer depth = max_depth_16
	#pragma HLS STREAM variable = wr_buffer depth = max_depth_16


	#pragma HLS dataflow
	read_row(arg0, rd_buffer, xdim0_poisson_kernel_stencil, base0, size1, count);


	process_a_row( rd_buffer, streamArray[1], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[1], streamArray[2], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[2], streamArray[3], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[3], streamArray[4], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[4], streamArray[5], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[5], streamArray[6], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[6], streamArray[7], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[7], streamArray[8], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[8], streamArray[9], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[9], streamArray[10], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[10], streamArray[11], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[11], streamArray[12], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[12], streamArray[13], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[13], streamArray[14], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[14], streamArray[15], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[15], streamArray[16], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[16], streamArray[17], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[17], streamArray[18], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[18], streamArray[19], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[19], streamArray[20], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[20], streamArray[21], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[21], streamArray[22], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[22], streamArray[23], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[23], streamArray[24], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[24], streamArray[25], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[25], streamArray[26], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[26], streamArray[27], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[27], streamArray[28], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[28], streamArray[29], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[29], streamArray[30], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[30], streamArray[31], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[31], streamArray[32], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[32], streamArray[33], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[33], streamArray[34], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[34], streamArray[35], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[35], streamArray[36], size0, size1, xdim0_poisson_kernel_stencil, count);

	process_a_row( streamArray[36], streamArray[37], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[37], streamArray[38], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[38], streamArray[39], size0, size1, xdim0_poisson_kernel_stencil, count);
	process_a_row( streamArray[39], wr_buffer, size0, size1, xdim0_poisson_kernel_stencil, count);


	write_row(arg1, wr_buffer, xdim1_poisson_kernel_stencil, base1, size1, count);
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
		const int xdim1_poisson_kernel_stencil/*,
		const int count*/){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0 offset = slave bundle = gmem0 max_read_burst_length=256 max_write_burst_length=256
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1 offset = slave bundle = gmem0
	#pragma HLS INTERFACE s_axilite port = arg0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1 bundle = control
	#pragma HLS INTERFACE s_axilite port = base0 bundle = control
	#pragma HLS INTERFACE s_axilite port = base1 bundle = control
	#pragma HLS INTERFACE s_axilite port = size0 bundle = control
	#pragma HLS INTERFACE s_axilite port = size1 bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0_poisson_kernel_stencil bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim1_poisson_kernel_stencil bundle = control
//	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control



	//int end_row  = size1+37;
	int count  = 1;
	process(arg0, arg1, xdim0_poisson_kernel_stencil, base0, xdim0_poisson_kernel_stencil, base0, size0, size1, count);

}
}
