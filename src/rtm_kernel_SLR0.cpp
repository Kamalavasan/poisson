
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <stdio.h>
#include <ap_int.h>



typedef ap_uint<768> uint768_dt;
typedef ap_uint<576> uint576_dt;
typedef ap_uint<512> uint512_dt;
typedef ap_uint<256> uint256_dt;
typedef ap_uint<192> uint192_dt;
typedef ap_axiu<256,0,0,0> t_pkt;

#define MAX_SIZE_X 128
#define MAX_DEPTH_16 (MAX_SIZE_X/16)

//user function
#define PORT_WIDTH 8
#define SHIFT_BITS 0
#define DIM 6
#define DATATYPE_SIZE 32
//#define BEAT_SHIFT_BITS 10
#define BURST_LEN MAX_DEPTH_16

#define STAGES 2
#define ORDER 4

const int max_size_y = MAX_SIZE_X;
const int min_size_y = 20;
const int avg_size_y = MAX_SIZE_X;

const int max_block_x = MAX_SIZE_X/1 + 1;
const int min_block_x = 20/1 + 1;
const int avg_block_x = MAX_SIZE_X/1 + 1;

const int max_grid = max_block_x * max_size_y * max_size_y;
const int min_grid = min_block_x * min_size_y * min_size_y;
const int avg_grid = avg_block_x * avg_size_y * avg_size_y;

const int max_grid_2 = (max_block_x * max_size_y * max_size_y)/2;
const int min_grid_2 = (min_block_x * min_size_y * min_size_y)/2;
const int avg_grid_2 = (avg_block_x * avg_size_y * avg_size_y)/2;

const int port_width  = PORT_WIDTH;
const int max_depth_16 = MAX_DEPTH_16 * 8;
const int max_depth_8 = MAX_DEPTH_16 * 8;

const int plane_buff_size = 64*64;
const int line_buff_size = 80;


typedef union  {
   int i;
   float f;
} data_conv;

struct data_G{
	unsigned short sizex;
	unsigned short sizey;
	unsigned short sizez;
	unsigned short grid_sizex;
	unsigned short grid_sizey;
	unsigned short grid_sizez;
	unsigned short limit_z;
	unsigned int gridsize_pr;
	unsigned int plane_diff;
	unsigned int plane_size;
	unsigned int plane_size_3;
	unsigned int line_diff;
	unsigned short outer_loop_limit;
};

#include "derives_calc_ytep_k1.cpp"
#include "derives_calc_ytep_k2.cpp"
#include "derives_calc_ytep_k3.cpp"
#include "derives_calc_ytep_k4.cpp"


static void axis2_fifo256(hls::stream <t_pkt> &in, hls::stream<uint256_dt> &out, const int gridsize_da){
	for (int itr = 0; itr < gridsize_da; itr++){
//		printf("%d \n", itr);
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp = in.read();
		out << tmp.data;
	}
}

static void fifo256_2axis(hls::stream <uint256_dt> &in, hls::stream<t_pkt> &out, const int gridsize_da){
	for (int itr = 0; itr < gridsize_da; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp;
		tmp.data = in.read();
		out.write(tmp);
	}
}

void process_rtm_SLR0 ( hls::stream <t_pkt> &in0, hls::stream <t_pkt> &out0,
						hls::stream <t_pkt> &in1, hls::stream <t_pkt> &out1,
						hls::stream <t_pkt> &in2, hls::stream <t_pkt> &out2,
					   const int sizex, const int sizey, const int sizez, const int xdim_aigned, const int batch){


    static hls::stream<uint768_dt> streamArray[40 + 1];
    static hls::stream<uint768_dt> streamArray_yy[40 + 1];
    static hls::stream<uint768_dt> streamArray_yy_final[40 + 1];
    static hls::stream<uint256_dt> rd_bufferArr[3];
    static hls::stream<uint256_dt> wr_bufferArr[3];

    #pragma HLS STREAM variable = streamArray depth = 2
    #pragma HLS STREAM variable = streamArray_yy depth = 2
	#pragma HLS STREAM variable = streamArray_yy_final depth = 2

	#pragma HLS STREAM variable = rd_bufferArr depth = max_depth_8
	#pragma HLS STREAM variable = wr_bufferArr depth = max_depth_8

    struct data_G data_g;
    data_g.sizex = sizex;
    data_g.sizey = sizey;
    data_g.sizez = sizez;
	data_g.grid_sizex = sizex+2*ORDER;
	data_g.grid_sizey = sizey+2*ORDER;
	data_g.grid_sizez = sizez+2*ORDER;
	data_g.limit_z = sizez+3*ORDER;


	unsigned short grid_sizey_4 = (data_g.grid_sizey - 4);
	data_g.plane_size = xdim_aigned * data_g.grid_sizey;
	data_g.plane_size_3 = xdim_aigned * data_g.grid_sizey;

	data_g.plane_diff = xdim_aigned * grid_sizey_4;
	data_g.line_diff = xdim_aigned - 2;
	data_g.gridsize_pr = data_g.plane_size_3 * (data_g.limit_z) * batch;

	unsigned int gridsize_da = data_g.plane_size_3 * (data_g.grid_sizez)* batch;


	#pragma HLS dataflow

	axis2_fifo256(in0, rd_bufferArr[0], gridsize_da);
	axis2_fifo256(in1, rd_bufferArr[1], gridsize_da);
	axis2_fifo256(in2, rd_bufferArr[2], gridsize_da);

//	stream_convert_1536_768(rd_bufferArr[0], rd_bufferArr[1], rd_bufferArr[2], streamArray[0], gridsize_da);



	derives_calc_ytep_k1( rd_bufferArr[0], rd_bufferArr[0], rd_bufferArr[0], wr_bufferArr[0], wr_bufferArr[1], wr_bufferArr[2],streamArray_yy[0], streamArray_yy_final[0], data_g);
//	derives_calc_ytep_k2( streamArray[1], streamArray_yy[0],streamArray_yy_final[0], streamArray[2], streamArray_yy[1],streamArray_yy_final[1], data_g);
//	derives_calc_ytep_k3( streamArray[2], streamArray_yy[1],streamArray_yy_final[1], streamArray[3], streamArray_yy[2],streamArray_yy_final[2], data_g);
//	derives_calc_ytep_k4( streamArray[3], streamArray_yy[2],streamArray_yy_final[2], streamArray[4],streamArray_yy[3],streamArray_yy_final[3], data_g);

//	fifo256_2axis(streamArray[1], out, gridsize_da);
//	axis2_fifo256(in, streamArray[5], gridsize_da);

//	stream_convert_768_1536(streamArray[0], wr_bufferArr[0], wr_bufferArr[1], wr_bufferArr[2], gridsize_da);
	fifo256_2axis(wr_bufferArr[0],out0, gridsize_da);
	fifo256_2axis(wr_bufferArr[1],out1, gridsize_da);
	fifo256_2axis(wr_bufferArr[2],out2, gridsize_da);


}


//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT
//--sc stencil_SLR0_1.out:stencil_SLR1_1.in
//--sc stencil_SLR1_1.out:stencil_SLR0_1.in

extern "C" {
void rtm_SLR0(
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim_aligned,
		const int count,
		const int batch,
		hls::stream <t_pkt> &in0,
		hls::stream <t_pkt> &out0,
		hls::stream <t_pkt> &in1,
		hls::stream <t_pkt> &out1,
		hls::stream <t_pkt> &in2,
		hls::stream <t_pkt> &out2){

	#pragma HLS INTERFACE s_axilite port = sizex bundle = control
	#pragma HLS INTERFACE s_axilite port = sizey bundle = control
	#pragma HLS INTERFACE s_axilite port = sizez bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim_aligned bundle = control
	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = batch bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control
	#pragma HLS INTERFACE axis port = in0 register
	#pragma HLS INTERFACE axis port = out0 register
	#pragma HLS INTERFACE axis port = in1 register
	#pragma HLS INTERFACE axis port = out1 register
	#pragma HLS INTERFACE axis port = in2 register
	#pragma HLS INTERFACE axis port = out2 register


	for(int i =  0; i < 2*count; i++){
	#pragma HLS loop_tripcount min=10 max=1000 avg=1000
		process_rtm_SLR0(in0, out0, in1, out1, in2, out2, sizex, sizey, sizez, xdim_aligned, batch);
//		process_rtm_SLR0(arg1, arg0, in, out, sizex, sizey, sizez, xdim_aligned, batch);
	}

}
}
