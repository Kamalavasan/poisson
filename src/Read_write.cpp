#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>


#define TILE_X (256/16)
#define TILE_Y 256

static unsigned short register_it(unsigned short x){
	#pragma HLS inline off
	unsigned short tmp = x;
	return tmp;
}


static void read_to_fifo(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer0,
		uint512_dt*  arg1, hls::stream<uint512_dt> &rd_buffer1,
		uint512_dt*  arg2, hls::stream<uint512_dt> &rd_buffer2,
		uint512_dt*  arg3, hls::stream<uint512_dt> &rd_buffer3,
		uint512_dt*  arg4, hls::stream<uint512_dt> &rd_buffer4,
		uint512_dt*  arg5, hls::stream<uint512_dt> &rd_buffer5,
		uint512_dt*  arg6, hls::stream<uint512_dt> &rd_buffer6,
		uint512_dt*  arg7, hls::stream<uint512_dt> &rd_buffer7,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short tile_y = (data_g.tile_y);

	unsigned short xblocks = (data_g.xblocks >> 3);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short offset_x = (data_g.offset_x >> 3);
	unsigned int plane_size = (data_g.plane_size >> 3);

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = end_z * tile_y;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		unsigned short k = itr / tile_y;
		unsigned short i = itr % tile_y;

		unsigned short k1 = register_it(k);
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = row_offset0 + offset_x_b;
		unsigned int base_index = base_index0 + plane_offset;

		for(unsigned short j = 0; j < end_index; j++){
			#pragma HLS loop_tripcount min=8 max=32 avg=16
			#pragma HLS PIPELINE II=1
			rd_buffer0 << arg0[base_index + j];
			rd_buffer1 << arg1[base_index + j];
			rd_buffer2 << arg2[base_index + j];
			rd_buffer3 << arg3[base_index + j];

			rd_buffer4 << arg4[base_index + j];
			rd_buffer5 << arg5[base_index + j];
			rd_buffer6 << arg6[base_index + j];
			rd_buffer7 << arg7[base_index + j];

		}

	}
}


static void write_from_fifo(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer0,
		uint512_dt*  arg1, hls::stream<uint512_dt> &rd_buffer1,
		uint512_dt*  arg2, hls::stream<uint512_dt> &rd_buffer2,
		uint512_dt*  arg3, hls::stream<uint512_dt> &rd_buffer3,
		uint512_dt*  arg4, hls::stream<uint512_dt> &rd_buffer4,
		uint512_dt*  arg5, hls::stream<uint512_dt> &rd_buffer5,
		uint512_dt*  arg6, hls::stream<uint512_dt> &rd_buffer6,
		uint512_dt*  arg7, hls::stream<uint512_dt> &rd_buffer7,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short tile_y = (data_g.tile_y);

	unsigned short xblocks = (data_g.xblocks >> 3);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short offset_x = (data_g.offset_x >> 3);
	unsigned int plane_size = (data_g.plane_size >> 3);

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = end_z * tile_y;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		unsigned short k = itr / tile_y;
		unsigned short i = itr % tile_y;
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;

		for(unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=8 max=32 avg=16
			arg0[base_index0 + j] = rd_buffer0.read();
			arg1[base_index0 + j] = rd_buffer1.read();
			arg2[base_index0 + j] = rd_buffer2.read();
			arg3[base_index0 + j] = rd_buffer3.read();

			arg4[base_index0 + j] = rd_buffer4.read();
			arg5[base_index0 + j] = rd_buffer5.read();
			arg6[base_index0 + j] = rd_buffer6.read();
			arg7[base_index0 + j] = rd_buffer7.read();
		}

	}
}


static void stream_convert_512_256(hls::stream<uint512_dt> &in0, hls::stream<uint256_dt> &out0,
		hls::stream<uint512_dt> &in1, hls::stream<uint256_dt> &out1,
		hls::stream<uint512_dt> &in2, hls::stream<uint256_dt> &out2,
		hls::stream<uint512_dt> &in3, hls::stream<uint256_dt> &out3,
		hls::stream<uint512_dt> &in4, hls::stream<uint256_dt> &out4,
		hls::stream<uint512_dt> &in5, hls::stream<uint256_dt> &out5,
		hls::stream<uint512_dt> &in6, hls::stream<uint256_dt> &out6,
		hls::stream<uint512_dt> &in7, hls::stream<uint256_dt> &out7,
		struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2
		uint512_dt tmp0 = in0.read();
		uint512_dt tmp1 = in1.read();
		uint512_dt tmp2 = in2.read();
		uint512_dt tmp3 = in3.read();
		uint512_dt tmp4 = in4.read();
		uint512_dt tmp5 = in5.read();
		uint512_dt tmp6 = in6.read();
		uint512_dt tmp7 = in7.read();

		out0 << tmp0.range(255,0);
		out0 << tmp0.range(511,256);

		out1 << tmp1.range(255,0);
		out1 << tmp1.range(511,256);

		out2 << tmp2.range(255,0);
		out2 << tmp2.range(511,256);

		out3 << tmp3.range(255,0);
		out3 << tmp3.range(511,256);

		out4 << tmp4.range(255,0);
		out4 << tmp4.range(511,256);

		out5 << tmp5.range(255,0);
		out5 << tmp5.range(511,256);

		out6 << tmp6.range(255,0);
		out6 << tmp6.range(511,256);

		out7 << tmp7.range(255,0);
		out7 << tmp7.range(511,256);

	}
}

static void stream_convert_256_512(hls::stream<uint256_dt> &in0, hls::stream<uint512_dt> &out0,
		hls::stream<uint256_dt> &in1, hls::stream<uint512_dt> &out1,
		hls::stream<uint256_dt> &in2, hls::stream<uint512_dt> &out2,
		hls::stream<uint256_dt> &in3, hls::stream<uint512_dt> &out3,
		hls::stream<uint256_dt> &in4, hls::stream<uint512_dt> &out4,
		hls::stream<uint256_dt> &in5, hls::stream<uint512_dt> &out5,
		hls::stream<uint256_dt> &in6, hls::stream<uint512_dt> &out6,
		hls::stream<uint256_dt> &in7, hls::stream<uint512_dt> &out7,
		struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2
		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
		tmp0.range(255,0) = in0.read();
		tmp0.range(511,256) = in0.read();

		tmp1.range(255,0) = in1.read();
		tmp1.range(511,256) = in1.read();

		tmp2.range(255,0) = in2.read();
		tmp2.range(511,256) = in2.read();

		tmp3.range(255,0) = in3.read();
		tmp3.range(511,256) = in3.read();

		tmp4.range(255,0) = in4.read();
		tmp4.range(511,256) = in4.read();

		tmp5.range(255,0) = in5.read();
		tmp5.range(511,256) = in5.read();

		tmp6.range(255,0) = in6.read();
		tmp6.range(511,256) = in6.read();

		tmp7.range(255,0) = in7.read();
		tmp7.range(511,256) = in7.read();

		out0 << tmp0;
		out1 << tmp1;
		out2 << tmp2;
		out3 << tmp3;

		out4 << tmp4;
		out5 << tmp5;
		out6 << tmp6;
		out7 << tmp7;

	}
}


static void stream_convert_256_2048(hls::stream<uint256_dt> &in0, hls::stream<uint256_dt> &in1,
		hls::stream<uint256_dt> &in2, hls::stream<uint256_dt> &in3,
		hls::stream<uint256_dt> &in4, hls::stream<uint256_dt> &in5,
		hls::stream<uint256_dt> &in6, hls::stream<uint256_dt> &in7,
		hls::stream<uint1024_dt> &out0, hls::stream<uint1024_dt> &out1, struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short end_index = (tile_x >> (SHIFT_BITS));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=1
		uint1024_dt tmp0, tmp1;
		tmp0.range(255,0)    = in0.read();
		tmp0.range(511,256)  = in1.read();
		tmp0.range(767,512)  = in2.read();
		tmp0.range(1023,768) = in3.read();

		tmp1.range(255,0)    = in4.read();
		tmp1.range(511,256)  = in5.read();
		tmp1.range(767,512)  = in6.read();
		tmp1.range(1023,768) = in7.read();

		out0 << tmp0;
		out1 << tmp1;
	}
}

static void stream_convert_2048_256(hls::stream<uint1024_dt> &in0, hls::stream<uint1024_dt> &in1,
		hls::stream<uint256_dt> &out0, hls::stream<uint256_dt> &out1,
		hls::stream<uint256_dt> &out2, hls::stream<uint256_dt> &out3,
		hls::stream<uint256_dt> &out4, hls::stream<uint256_dt> &out5,
		hls::stream<uint256_dt> &out6, hls::stream<uint256_dt> &out7,  struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short end_index = (tile_x >> (SHIFT_BITS));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=1
		uint1024_dt tmp0, tmp1;
		tmp0 = in0.read();
		tmp1 = in1.read();

		out0 << tmp0.range(255,0);
		out1 << tmp0.range(511,256);
		out2 << tmp0.range(767,512);
		out3 << tmp0.range(1023,768);

		out4 << tmp1.range(255,0);
		out5 << tmp1.range(511,256);
		out6 << tmp1.range(767,512);
		out7 << tmp1.range(1023,768);
	}
}


static void process_ReadWrite (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
								uint512_dt*  arg0_1, uint512_dt*  arg1_1,
								uint512_dt*  arg0_2, uint512_dt*  arg1_2,
								uint512_dt*  arg0_3, uint512_dt*  arg1_3,
								uint512_dt*  arg0_4, uint512_dt*  arg1_4,
								uint512_dt*  arg0_5, uint512_dt*  arg1_5,
								uint512_dt*  arg0_6, uint512_dt*  arg1_6,
								uint512_dt*  arg0_7, uint512_dt*  arg1_7,
							   const int xdim0, const unsigned short offset_x, const unsigned short tile_x,
							   const unsigned short offset_y, unsigned short tile_y,
							   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){


    static hls::stream<uint256_dt> streamArrayrd[8];
    static hls::stream<uint256_dt> streamArraywr[8];
    static hls::stream<uint512_dt> rd_bufferArr[8];
    static hls::stream<uint512_dt> wr_bufferArr[8];
    static hls::stream<uint1024_dt> streamArray_1024[2];

    #pragma HLS STREAM variable = streamArrayrd depth = 2
	#pragma HLS STREAM variable = streamArraywr depth = 2
	#pragma HLS STREAM variable = streamArray_1024 depth = 2
	#pragma HLS STREAM variable = rd_bufferArr depth = 2  //max_depth_16
	#pragma HLS STREAM variable = wr_bufferArr depth = 2  // max_depth_16



    struct data_G data_g;
    data_g.sizex = size_x;
    data_g.sizey = size_y;
    data_g.sizez = size_z;
    data_g.xdim = xdim0;
	data_g.xblocks = (xdim0 >> (SHIFT_BITS+1));
	data_g.grid_sizey = size_y+2;
	data_g.grid_sizez = size_z+2;
	data_g.limit_z = size_z+3;

	data_g.offset_x = offset_x;
	data_g.tile_x = tile_x;
	data_g.offset_y = offset_y;
	data_g.tile_y = tile_y;

	data_g.plane_size = data_g.xblocks * data_g.grid_sizey;

	unsigned int tile_plane_size = (data_g.tile_x >> SHIFT_BITS) * data_g.tile_y;
	unsigned int totol_iter = tile_plane_size * data_g.grid_sizez;


	#pragma HLS dataflow

	read_to_fifo(arg0_0, rd_bufferArr[0], arg0_1, rd_bufferArr[1],
			arg0_2, rd_bufferArr[2], arg0_3, rd_bufferArr[3],
			arg0_4, rd_bufferArr[4], arg0_5, rd_bufferArr[5],
			arg0_6, rd_bufferArr[6], arg0_7, rd_bufferArr[7], data_g);



	stream_convert_512_256(rd_bufferArr[0], streamArrayrd[0], rd_bufferArr[1], streamArrayrd[1],
			rd_bufferArr[2], streamArrayrd[2], rd_bufferArr[3], streamArrayrd[3],
			rd_bufferArr[4], streamArrayrd[4], rd_bufferArr[5], streamArrayrd[5],
			rd_bufferArr[6], streamArrayrd[6], rd_bufferArr[7], streamArrayrd[7],data_g);


	stream_convert_256_2048(streamArrayrd[0], streamArrayrd[1], streamArrayrd[2], streamArrayrd[3],
			streamArrayrd[4], streamArrayrd[5], streamArrayrd[6], streamArrayrd[7],
			streamArray_1024[0], streamArray_1024[1], data_g);

	stream_convert_2048_256(streamArray_1024[0], streamArray_1024[1],
			streamArraywr[0], streamArraywr[1], streamArraywr[2], streamArraywr[3],
			streamArraywr[4], streamArraywr[5], streamArraywr[6], streamArraywr[7], data_g);

	stream_convert_256_512(streamArraywr[0], wr_bufferArr[0], streamArraywr[1], wr_bufferArr[1],
			streamArraywr[2], wr_bufferArr[2], streamArraywr[3], wr_bufferArr[3],
			streamArraywr[4], wr_bufferArr[4], streamArraywr[5], wr_bufferArr[5],
			streamArraywr[6], wr_bufferArr[6], streamArraywr[7], wr_bufferArr[7],
			data_g);


	write_from_fifo(arg1_0, wr_bufferArr[0], arg1_1, wr_bufferArr[1],
			arg1_2, wr_bufferArr[2], arg1_3, wr_bufferArr[3],
			arg1_4, wr_bufferArr[4], arg1_5, wr_bufferArr[5],
			arg1_6, wr_bufferArr[6], arg1_7, wr_bufferArr[7],data_g);

}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
										uint512_dt*  arg0_1, uint512_dt*  arg1_1,
										uint512_dt*  arg0_2, uint512_dt*  arg1_2,
										uint512_dt*  arg0_3, uint512_dt*  arg1_3,
										uint512_dt*  arg0_4, uint512_dt*  arg1_4,
										uint512_dt*  arg0_5, uint512_dt*  arg1_5,
										uint512_dt*  arg0_6, uint512_dt*  arg1_6,
										uint512_dt*  arg0_7, uint512_dt*  arg1_7,
									   const int xdim0, unsigned int tile_memx[], unsigned short tilex_count ,
									   unsigned int tile_memy[], unsigned short tiley_count,
									   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){

	unsigned int toltal_itr = tilex_count*tiley_count;
	unsigned short i = 0, j = 0;
	for(unsigned int itr = 0; itr < toltal_itr; itr++){
		if(j == tiley_count){
			j = 0;
			i++;
		}
		unsigned short offset_x = tile_memx[i] & 0xffff;
		unsigned short tile_x   = tile_memx[i] >> 16;
		unsigned short offset_y = tile_memy[j] & 0xffff;
		unsigned short tile_y   = tile_memy[j] >> 16;
		process_ReadWrite(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3,
				arg0_4, arg1_4, arg0_5, arg1_5, arg0_6, arg1_6, arg0_7, arg1_7,
				xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);
		j++;
	}
}

//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT


extern "C" {
void stencil_Read_Write(
		uint512_dt*  arg0_0,
		uint512_dt*  arg1_0,

		uint512_dt*  arg0_1,
		uint512_dt*  arg1_1,

		uint512_dt*  arg0_2,
		uint512_dt*  arg1_2,

		uint512_dt*  arg0_3,
		uint512_dt*  arg1_3,

		uint512_dt*  arg0_4,
		uint512_dt*  arg1_4,

		uint512_dt*  arg0_5,
		uint512_dt*  arg1_5,

		uint512_dt*  arg0_6,
		uint512_dt*  arg1_6,

		uint512_dt*  arg0_7,
		uint512_dt*  arg1_7,

		const unsigned int* tile,
		const int tilex_count,
		const int tiley_count,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_1 offset = slave bundle = gmem1 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_1 offset = slave bundle = gmem1 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_2 offset = slave bundle = gmem2 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_2 offset = slave bundle = gmem2 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_3 offset = slave bundle = gmem3 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_3 offset = slave bundle = gmem3 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_4 offset = slave bundle = gmem4 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_4 offset = slave bundle = gmem4 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_5 offset = slave bundle = gmem5 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_5 offset = slave bundle = gmem5 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_6 offset = slave bundle = gmem6 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_6 offset = slave bundle = gmem6 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_7 offset = slave bundle = gmem7 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_7 offset = slave bundle = gmem7 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=16 num_write_outstanding=16


	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem8

	#pragma HLS INTERFACE s_axilite port = arg0_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = tile bundle = control

	#pragma HLS INTERFACE s_axilite port = tilex_count bundle = control
	#pragma HLS INTERFACE s_axilite port = tiley_count bundle = control
	#pragma HLS INTERFACE s_axilite port = sizex bundle = control
	#pragma HLS INTERFACE s_axilite port = sizey bundle = control
	#pragma HLS INTERFACE s_axilite port = sizez bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0 bundle = control
	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control



	unsigned int tile_memx[256];
	unsigned int tile_memy[256];

	for(int j = 0; j < tilex_count; j++){
		#pragma HLS PIPELINE II=1
		t_pkt_32 tmp_s;
		unsigned int tmp;
		tmp = tile[j];
		tile_memx[j] = tmp;
		tmp_s.data = tmp;
	}

	for(int j = 0; j < tiley_count; j++){
			#pragma HLS PIPELINE II=1
			t_pkt_32 tmp_s;
			unsigned int tmp;
			tmp = tile[tilex_count+j];
			tile_memy[j] = tmp;
			tmp_s.data = tmp;
		}



	for(int i =  0; i < count; i++){
		process_ReadWrite_dataflow(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3, arg0_4, arg1_4, arg0_5, arg1_5, arg0_6, arg1_6, arg0_7, arg1_7,
				xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg0_0, arg1_1, arg0_1, arg1_2, arg0_2, arg1_3, arg0_3, arg1_4, arg0_4, arg1_5, arg0_5, arg1_6, arg0_6, arg1_7, arg0_7,
				xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}


