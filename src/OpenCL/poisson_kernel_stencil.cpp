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



const int max_size_y = 2000;
const int min_size_y = 20;
const int avg_size_y = 2000;

const int max_block_x = 2000/16 + 1;
const int min_block_x = 20/16 + 1;
const int avg_block_x = 2000/16 + 1;

const int port_width  = PORT_WIDTH;
const int max_depth_16 = MAX_DEPTH_16*2;


typedef union  {
   int i;
   float f;
} data_conv;

static void read_row(const uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer, const int xdim0_poisson_kernel_stencil, const int base0, int i, int size1){

	int base_index = (base0 + ((i-1) * xdim0_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim0_poisson_kernel_stencil >> SHIFT_BITS) + 2;
	int end_row = size1+3;
	if(i < end_row){
		read_row_loop: for(int k =0; k < end_index; k++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=min_block_x max=max_block_x avg=avg_block_x
			rd_buffer << arg0[base_index -1 + k];
		}
	}
}

static void process_a_row( hls::stream<uint512_dt> &rd_buffer, hls::stream<uint512_dt> &wr_buffer,  uint512_dt* row1, uint512_dt* row2, uint512_dt* row3, const int size0, int size1,  const int xdim0_poisson_kernel_stencil, int i, int pipeline_stage){

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

	#pragma HLS ARRAY_PARTITION variable=row_arr3 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=row_arr2 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=row_arr1 complete dim=1
	#pragma HLS ARRAY_PARTITION variable=mem_wr complete dim=1


	main_process_loop: for(int j =0; j < end_index; j++){
		#pragma HLS loop_tripcount min=min_block_x max=max_block_x avg=avg_block_x
		#pragma HLS PIPELINE II=1

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
			tmp1 = rd_buffer.read();
			row1[j] = tmp1;
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

		if((i >= 1 + pipeline_stage) && j >= 1 && ( i <= end_row + pipeline_stage)) {
			wr_buffer << update_j;
		}
	}
	if((i >= 1 + pipeline_stage) && ( i <= end_row + pipeline_stage)){
		wr_buffer << tmp2;
	}
}

static void write_row( uint512_dt*  arg1, hls::stream<uint512_dt> &wr_buffer, const int xdim1_poisson_kernel_stencil, const int base1, int i, int pipeline_stage){
	int base_index = (base1 + ((i-2 -pipeline_stage) * xdim1_poisson_kernel_stencil) -1) >> SHIFT_BITS;
	int end_index = (xdim1_poisson_kernel_stencil >> SHIFT_BITS) + 1;
	if(i >= (1 + pipeline_stage)){
		uint512_dt tmp1 = wr_buffer.read();
		write_row_loop: for(int k =0; k < end_index; k++){
			#pragma HLS loop_tripcount min=min_block_x max=max_block_x avg=avg_block_x
			#pragma HLS PIPELINE II=1
			arg1[base_index   + k] =  wr_buffer.read();
		}
	}
}


void process (const uint512_dt*  arg0, uint512_dt*  arg1,
		uint512_dt* row1_p1, uint512_dt* row2_p1, uint512_dt* row3_p1,
		uint512_dt* row1_p2, uint512_dt* row2_p2, uint512_dt* row3_p2,
		uint512_dt* row1_p3, uint512_dt* row2_p3, uint512_dt* row3_p3,
		uint512_dt* row1_p4, uint512_dt* row2_p4, uint512_dt* row3_p4,
		uint512_dt* row1_p5, uint512_dt* row2_p5, uint512_dt* row3_p5,
		uint512_dt* row1_p6, uint512_dt* row2_p6, uint512_dt* row3_p6,
		uint512_dt* row1_p7, uint512_dt* row2_p7, uint512_dt* row3_p7,
		uint512_dt* row1_p8, uint512_dt* row2_p8, uint512_dt* row3_p8,
		uint512_dt* row1_p9, uint512_dt* row2_p9, uint512_dt* row3_p9,
		uint512_dt* row1_p10, uint512_dt* row2_p10, uint512_dt* row3_p10,
		uint512_dt* row1_p11, uint512_dt* row2_p11, uint512_dt* row3_p11,
		uint512_dt* row1_p12, uint512_dt* row2_p12, uint512_dt* row3_p12,
		uint512_dt* row1_p13, uint512_dt* row2_p13, uint512_dt* row3_p13,
		uint512_dt* row1_p14, uint512_dt* row2_p14, uint512_dt* row3_p14,
		uint512_dt* row1_p15, uint512_dt* row2_p15, uint512_dt* row3_p15,
		uint512_dt* row1_p16, uint512_dt* row2_p16, uint512_dt* row3_p16,
		const int xdim0_poisson_kernel_stencil, const int base0, const int xdim1_poisson_kernel_stencil, const int base1, const int size0, int size1, int i){

	static hls::stream<uint512_dt> rd_buffer_p1("rd_buffer_p1");
    static hls::stream<uint512_dt> rd_buffer_p2("rd_buffer_p2");
    static hls::stream<uint512_dt> rd_buffer_p3("rd_buffer_p3");
    static hls::stream<uint512_dt> rd_buffer_p4("rd_buffer_p4");
	static hls::stream<uint512_dt> rd_buffer_p5("rd_buffer_p5");
    static hls::stream<uint512_dt> rd_buffer_p6("rd_buffer_p6");
    static hls::stream<uint512_dt> rd_buffer_p7("rd_buffer_p7");
    static hls::stream<uint512_dt> rd_buffer_p8("rd_buffer_p8");
    static hls::stream<uint512_dt> rd_buffer_p9("rd_buffer_p9");
	static hls::stream<uint512_dt> rd_buffer_p10("rd_buffer_p10");
	static hls::stream<uint512_dt> rd_buffer_p11("rd_buffer_p11");
	static hls::stream<uint512_dt> rd_buffer_p12("rd_buffer_p12");
	static hls::stream<uint512_dt> rd_buffer_p13("rd_buffer_p13");
	static hls::stream<uint512_dt> rd_buffer_p14("rd_buffer_p14");
	static hls::stream<uint512_dt> rd_buffer_p15("rd_buffer_p15");
	static hls::stream<uint512_dt> rd_buffer_p16("rd_buffer_p16");

    static hls::stream<uint512_dt> wr_buffer("wr_buffer");

	#pragma HLS STREAM variable = rd_buffer_p1 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p2 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p3 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p4 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p5 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p6 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p7 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p8 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p9 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p10 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p11 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p12 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p13 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p14 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p15 depth = max_depth_16
	#pragma HLS STREAM variable = rd_buffer_p16 depth = max_depth_16
	#pragma HLS STREAM variable = wr_buffer depth = max_depth_16

	#pragma HLS dataflow
	read_row(arg0, rd_buffer_p1, xdim0_poisson_kernel_stencil, base0, i, size1);
	process_a_row(rd_buffer_p1, rd_buffer_p2, row1_p1, row2_p1, row3_p1, size0, size1, xdim0_poisson_kernel_stencil, i, 0);
	process_a_row(rd_buffer_p2, rd_buffer_p3, row1_p2, row2_p2, row3_p2, size0, size1, xdim0_poisson_kernel_stencil, i, 1);
	process_a_row(rd_buffer_p3, rd_buffer_p4, row1_p3, row2_p3, row3_p3, size0, size1, xdim0_poisson_kernel_stencil, i, 2);
	process_a_row(rd_buffer_p4, rd_buffer_p5, row1_p4, row2_p4, row3_p4, size0, size1, xdim0_poisson_kernel_stencil, i, 3);
	process_a_row(rd_buffer_p5, rd_buffer_p6, row1_p5, row2_p5, row3_p5, size0, size1, xdim0_poisson_kernel_stencil, i, 4);
	process_a_row(rd_buffer_p6, rd_buffer_p7, row1_p6, row2_p6, row3_p6, size0, size1, xdim0_poisson_kernel_stencil, i, 5);
	process_a_row(rd_buffer_p7, rd_buffer_p8, row1_p7, row2_p7, row3_p7, size0, size1, xdim0_poisson_kernel_stencil, i, 6);
	process_a_row(rd_buffer_p8, rd_buffer_p9, row1_p8, row2_p8, row3_p8, size0, size1, xdim0_poisson_kernel_stencil, i, 7);

	process_a_row(rd_buffer_p9,  rd_buffer_p10, row1_p9, row2_p9, row3_p9, size0, size1, xdim0_poisson_kernel_stencil, i, 8);
	process_a_row(rd_buffer_p10, rd_buffer_p11, row1_p10, row2_p10, row3_p10, size0, size1, xdim0_poisson_kernel_stencil, i, 9);
	process_a_row(rd_buffer_p11, rd_buffer_p12, row1_p11, row2_p11, row3_p11, size0, size1, xdim0_poisson_kernel_stencil, i, 10);
	process_a_row(rd_buffer_p12, rd_buffer_p13, row1_p12, row2_p12, row3_p12, size0, size1, xdim0_poisson_kernel_stencil, i, 11);
	process_a_row(rd_buffer_p13, rd_buffer_p14, row1_p13, row2_p13, row3_p13, size0, size1, xdim0_poisson_kernel_stencil, i, 12);
	process_a_row(rd_buffer_p14, rd_buffer_p15, row1_p14, row2_p14, row3_p14, size0, size1, xdim0_poisson_kernel_stencil, i, 13);
	process_a_row(rd_buffer_p15, rd_buffer_p16, row1_p15, row2_p15, row3_p15, size0, size1, xdim0_poisson_kernel_stencil, i, 14);
	process_a_row(rd_buffer_p16, wr_buffer, row1_p16, row2_p16, row3_p16, size0, size1, xdim0_poisson_kernel_stencil, i, 15);

	write_row(arg1, wr_buffer, xdim1_poisson_kernel_stencil, base1, i, 15);
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

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE s_axilite port = arg0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1 bundle = control
	#pragma HLS INTERFACE s_axilite port = base0 bundle = control
	#pragma HLS INTERFACE s_axilite port = base1 bundle = control
	#pragma HLS INTERFACE s_axilite port = size0 bundle = control
	#pragma HLS INTERFACE s_axilite port = size1 bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0_poisson_kernel_stencil bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim1_poisson_kernel_stencil bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control



	int end_row  = size1+18;


//     static hls::stream<uint512_dt> row1_p1("row1_p1");
//     static hls::stream<uint512_dt> row2_p1("row2_p1");
//     static hls::stream<uint512_dt> row3_p1("row3_p1");
//
//     static hls::stream<uint512_dt> row1_p2("row1_p2");
//     static hls::stream<uint512_dt> row2_p2("row2_p2");
//     static hls::stream<uint512_dt> row3_p2("row3_p2");


	uint512_dt row1_p1[BURST_LEN + 2];
	uint512_dt row2_p1[BURST_LEN + 2];
	uint512_dt row3_p1[BURST_LEN + 2];

	uint512_dt row1_p2[BURST_LEN + 2];
	uint512_dt row2_p2[BURST_LEN + 2];
	uint512_dt row3_p2[BURST_LEN + 2];

	uint512_dt row1_p3[BURST_LEN + 2];
	uint512_dt row2_p3[BURST_LEN + 2];
	uint512_dt row3_p3[BURST_LEN + 2];

	uint512_dt row1_p4[BURST_LEN + 2];
	uint512_dt row2_p4[BURST_LEN + 2];
	uint512_dt row3_p4[BURST_LEN + 2];

	uint512_dt row1_p5[BURST_LEN + 2];
	uint512_dt row2_p5[BURST_LEN + 2];
	uint512_dt row3_p5[BURST_LEN + 2];

	uint512_dt row1_p6[BURST_LEN + 2];
	uint512_dt row2_p6[BURST_LEN + 2];
	uint512_dt row3_p6[BURST_LEN + 2];

	uint512_dt row1_p7[BURST_LEN + 2];
	uint512_dt row2_p7[BURST_LEN + 2];
	uint512_dt row3_p7[BURST_LEN + 2];

	uint512_dt row1_p8[BURST_LEN + 2];
	uint512_dt row2_p8[BURST_LEN + 2];
	uint512_dt row3_p8[BURST_LEN + 2];

	uint512_dt row1_p9[BURST_LEN + 2];
	uint512_dt row2_p9[BURST_LEN + 2];
	uint512_dt row3_p9[BURST_LEN + 2];

	uint512_dt row1_p10[BURST_LEN + 2];
	uint512_dt row2_p10[BURST_LEN + 2];
	uint512_dt row3_p10[BURST_LEN + 2];

	uint512_dt row1_p11[BURST_LEN + 2];
	uint512_dt row2_p11[BURST_LEN + 2];
	uint512_dt row3_p11[BURST_LEN + 2];

	uint512_dt row1_p12[BURST_LEN + 2];
	uint512_dt row2_p12[BURST_LEN + 2];
	uint512_dt row3_p12[BURST_LEN + 2];

	uint512_dt row1_p13[BURST_LEN + 2];
	uint512_dt row2_p13[BURST_LEN + 2];
	uint512_dt row3_p13[BURST_LEN + 2];

	uint512_dt row1_p14[BURST_LEN + 2];
	uint512_dt row2_p14[BURST_LEN + 2];
	uint512_dt row3_p14[BURST_LEN + 2];

	uint512_dt row1_p15[BURST_LEN + 2];
	uint512_dt row2_p15[BURST_LEN + 2];
	uint512_dt row3_p15[BURST_LEN + 2];

	uint512_dt row1_p16[BURST_LEN + 2];
	uint512_dt row2_p16[BURST_LEN + 2];
	uint512_dt row3_p16[BURST_LEN + 2];


	#pragma HLS STREAM variable=row1_p1 off
	#pragma HLS STREAM variable=row2_p1 off
	#pragma HLS STREAM variable=row3_p1 off

	#pragma HLS STREAM variable=row1_p2 off
	#pragma HLS STREAM variable=row2_p2 off
	#pragma HLS STREAM variable=row3_p2 off

	#pragma HLS STREAM variable=row1_p3 off
	#pragma HLS STREAM variable=row2_p3 off
	#pragma HLS STREAM variable=row3_p3 off

	#pragma HLS STREAM variable=row1_p4 off
	#pragma HLS STREAM variable=row2_p4 off
	#pragma HLS STREAM variable=row3_p4 off

	#pragma HLS STREAM variable=row1_p5 off
	#pragma HLS STREAM variable=row2_p5 off
	#pragma HLS STREAM variable=row3_p5 off

	#pragma HLS STREAM variable=row1_p6 off
	#pragma HLS STREAM variable=row2_p6 off
	#pragma HLS STREAM variable=row3_p6 off

	#pragma HLS STREAM variable=row1_p7 off
	#pragma HLS STREAM variable=row2_p7 off
	#pragma HLS STREAM variable=row3_p7 off

	#pragma HLS STREAM variable=row1_p8 off
	#pragma HLS STREAM variable=row2_p8 off
	#pragma HLS STREAM variable=row3_p8 off


	#pragma HLS STREAM variable=row1_p9 off
	#pragma HLS STREAM variable=row2_p9 off
	#pragma HLS STREAM variable=row3_p9 off

	#pragma HLS STREAM variable=row1_p10 off
	#pragma HLS STREAM variable=row2_p10 off
	#pragma HLS STREAM variable=row3_p10 off

	#pragma HLS STREAM variable=row1_p11 off
	#pragma HLS STREAM variable=row2_p11 off
	#pragma HLS STREAM variable=row3_p11 off

	#pragma HLS STREAM variable=row1_p12 off
	#pragma HLS STREAM variable=row2_p12 off
	#pragma HLS STREAM variable=row3_p12 off

	#pragma HLS STREAM variable=row1_p13 off
	#pragma HLS STREAM variable=row2_p13 off
	#pragma HLS STREAM variable=row3_p13 off

	#pragma HLS STREAM variable=row1_p14 off
	#pragma HLS STREAM variable=row2_p14 off
	#pragma HLS STREAM variable=row3_p14 off

	#pragma HLS STREAM variable=row1_p15 off
	#pragma HLS STREAM variable=row2_p15 off
	#pragma HLS STREAM variable=row3_p15 off

	#pragma HLS STREAM variable=row1_p16 off
	#pragma HLS STREAM variable=row2_p16 off
	#pragma HLS STREAM variable=row3_p16 off

	loop_beats: for(int i = 0 ; i < end_row; i++){
		#pragma HLS loop_tripcount min=min_size_y max=max_size_y avg=avg_size_y
		#pragma HLS dataflow
		process(arg0, arg1,
				row1_p1, row2_p1, row3_p1,
				row1_p2, row2_p2, row3_p2,
				row1_p3, row2_p3, row3_p3,
				row1_p4, row2_p4, row3_p4,
				row1_p5, row2_p5, row3_p5,
				row1_p6, row2_p6, row3_p6,
				row1_p7, row2_p7, row3_p7,
				row1_p8, row2_p8, row3_p8,

				row1_p9, row2_p9, row3_p9,
				row1_p10, row2_p10, row3_p10,
				row1_p11, row2_p11, row3_p11,
				row1_p12, row2_p12, row3_p12,
				row1_p13, row2_p13, row3_p13,
				row1_p14, row2_p14, row3_p14,
				row1_p15, row2_p15, row3_p15,
				row1_p16, row2_p16, row3_p16,
				xdim0_poisson_kernel_stencil, base0, xdim0_poisson_kernel_stencil, base0, size0, size1,  i);
	}
}
}
