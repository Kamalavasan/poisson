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
		struct data_G data_g, unsigned short start){
	unsigned short tile_x = data_g.tile_x;
	unsigned short offset_x = data_g.offset_x;
	unsigned short tile_y = data_g.tile_y;
	unsigned short offset_y = data_g.offset_y;
	unsigned short end_z = data_g.grid_sizez;

	unsigned short xblocks = data_g.xblocks;
	unsigned int plane_size = data_g.plane_size;

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned short tiley_4 = tile_y;
	unsigned int total_out_itr = end_z * tiley_4;

	uint512_dt buffer[TILE_X*TILE_Y];
	#pragma HLS RESOURCE variable=buffer core=XPM_MEMORY uram

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
		}

	}
}


static void write_from_fifo(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer0,
		struct data_G data_g, unsigned short start){

	unsigned short tile_x = data_g.tile_x;
	unsigned short offset_x = data_g.offset_x;
	unsigned short tile_y = data_g.tile_y;
	unsigned short offset_y = data_g.offset_y;
	unsigned short end_z = data_g.grid_sizez;

	unsigned short xblocks = data_g.xblocks;
	unsigned int plane_size = data_g.plane_size;

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned short tiley_4 = tile_y ;
	unsigned int total_out_itr = end_z * tiley_4;


	uint512_dt buffer[TILE_X*TILE_Y];
	#pragma HLS RESOURCE variable=buffer core=XPM_MEMORY uram

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
		}

	}
}




















static void process_ReadWrite (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
				   const int xdim0, const unsigned short offset_x, const unsigned short tile_x,
				   const unsigned short offset_y, unsigned short tile_y,
				   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){


    static hls::stream<uint256_dt> streamArray[40 + 1];
    static hls::stream<uint512_dt> rd_bufferArr[4];
    static hls::stream<uint512_dt> wr_bufferArr[4];
    static hls::stream<uint512_dt> streamArray_512[4];

    #pragma HLS STREAM variable = streamArray depth = 2
	#pragma HLS STREAM variable = streamArray_512 depth = 2
	#pragma HLS STREAM variable = rd_bufferArr depth = max_depth_16
	#pragma HLS STREAM variable = wr_bufferArr depth = max_depth_16



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

	read_to_fifo(arg0_0, rd_bufferArr[0], data_g, 0);
	write_from_fifo(arg1_0, rd_bufferArr[0], data_g, 0);


}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
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
		process_ReadWrite(arg0_0, arg1_0, xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);
		j++;
	}
}

//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT


extern "C" {
void stencil_Read_Write(
		uint512_dt*  arg0_0,
		uint512_dt*  arg1_0,

		const unsigned int* tile,
		const int tilex_count,
		const int tiley_count,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem4

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
		process_ReadWrite_dataflow(arg0_0, arg1_0, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg0_0, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}


