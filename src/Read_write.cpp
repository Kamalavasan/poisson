#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>



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
	unsigned short tiley_4 = tile_y; //((tile_y+3-start) >> 0);
	unsigned int total_out_itr = end_z * tiley_4;
	unsigned int k = 0,  i = start;
	for(unsigned int itr = 0; itr < total_out_itr; itr++){
	#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		bool cond_i = i >= tile_y;
		if(cond_i){
			k++;
			i = start;
		}

		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;

		i = i + 1;
		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS loop_tripcount min=10 max=20 avg=15
			#pragma HLS PIPELINE II=1

			rd_buffer0 << arg0[base_index0 + j];
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
	unsigned short tiley_4 = tile_y;//((tile_y+3-start) >> 0);
	unsigned int total_out_itr = end_z * tiley_4;
	unsigned int k = 0,  i = start;
	for(unsigned int itr = 0; itr < total_out_itr; itr++){
	#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		bool cond_i = i >= tile_y;
		if(cond_i){
			k++;
			i = start;
		}

		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;

		i = i + 1;
		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS loop_tripcount min=10 max=20 avg=15
			#pragma HLS PIPELINE II=1

			arg0[base_index0 + j] = rd_buffer0.read();
		}
	}
}




















static void process_ReadWrite (uint512_dt*  arg0_0,  uint512_dt*  arg0_1, uint512_dt*  arg0_2, uint512_dt*  arg0_3,
				   uint512_dt*  arg1_0,  uint512_dt*  arg1_1, uint512_dt*  arg1_2,  uint512_dt*  arg1_3,
				   //hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
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

//	read_to_fifo(arg0_1, rd_bufferArr[1], data_g, 1);
//	write_from_fifo(arg1_1, rd_bufferArr[1], data_g, 1);
//
//	read_to_fifo(arg0_2, rd_bufferArr[2], data_g, 2);
//	write_from_fifo(arg1_2, rd_bufferArr[2], data_g, 2);
//
//	read_to_fifo(arg0_3, rd_bufferArr[3], data_g, 3);
//	write_from_fifo(arg1_3, rd_bufferArr[3], data_g, 3);



//	read_tile(arg0_0, arg0_1, arg0_2, arg0_3, rd_bufferArr[0], rd_bufferArr[1], rd_bufferArr[2], rd_bufferArr[3], data_g, 0);
//
//
//	combine_tile(rd_bufferArr[0], rd_bufferArr[1], rd_bufferArr[2], rd_bufferArr[3], streamArray_512[0], data_g);
////	stream_convert_512_256(streamArray_512[0], streamArray[0], data_g);
////
////
////	stream_convert_256_512(streamArray[0],streamArray_512[1], data_g);
//	separate_tile(streamArray_512[0], wr_bufferArr[0], wr_bufferArr[1], wr_bufferArr[2], wr_bufferArr[3], data_g);
//	write_tile(arg1_0, arg1_1, arg1_2, arg1_3, wr_bufferArr[0], wr_bufferArr[1], wr_bufferArr[2], wr_bufferArr[3], data_g, 0);

}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg0_1, uint512_dt*  arg0_2, uint512_dt*  arg0_3,
				   uint512_dt*  arg1_0,   uint512_dt*  arg1_1, uint512_dt*  arg1_2,  uint512_dt*  arg1_3,
				   //hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
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
//		printf("Itr:%d\n", itr);
		process_ReadWrite(arg0_0, arg0_1, arg0_2, arg0_3, arg1_0, arg1_1, arg1_2, arg1_3/*, in, out*/, xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);
		j++;
	}
}

//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT


extern "C" {
void stencil_Read_Write(
		uint512_dt*  arg0_0,
		uint512_dt*  arg0_1,
		uint512_dt*  arg0_2,
		uint512_dt*  arg0_3,

		uint512_dt*  arg1_0,
		uint512_dt*  arg1_1,
		uint512_dt*  arg1_2,
		uint512_dt*  arg1_3,


		const unsigned int* tile,
		const int tilex_count,
		const int tiley_count,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count/*,

		hls::stream <t_pkt_32> &tile_s_out,
		hls::stream <t_pkt> &in,
		hls::stream <t_pkt> &out*/){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 //num_read_outstanding=4 num_write_outstanding=4
	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_1 offset = slave bundle = gmem1 max_read_burst_length=64 max_write_burst_length=64 //num_read_outstanding=2 num_write_outstanding=2
	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_2 offset = slave bundle = gmem2 max_read_burst_length=64 max_write_burst_length=64
	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_3 offset = slave bundle = gmem3 max_read_burst_length=64 max_write_burst_length=64



	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_3 offset = slave bundle = gmem3


	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem4

	#pragma HLS INTERFACE s_axilite port = arg0_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg0_1 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg0_2 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg0_3 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg1_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_1 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_2 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_3 bundle = control

	#pragma HLS INTERFACE s_axilite port = tile bundle = control
//	#pragma HLS INTERFACE axis port = tile_s_out  register
//	#pragma HLS INTERFACE axis port = in  register
//	#pragma HLS INTERFACE axis port = out register

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
//		tile_s_out.write(tmp_s);
	}

	for(int j = 0; j < tiley_count; j++){
			#pragma HLS PIPELINE II=1
			t_pkt_32 tmp_s;
			unsigned int tmp;
			tmp = tile[tilex_count+j];
			tile_memy[j] = tmp;
			tmp_s.data = tmp;
//			tile_s_out.write(tmp_s);
		}



	bool read_write_offset_dn = 0;
	for(int i =  0; i < count; i++){
//		for(int j = 0; j < tile_count; j++){
//			//#pragma HLS dataflow
//			unsigned short offset = tile_mem[j] & 0xffff;
//			unsigned short tile_x = tile_mem[j] >> 16;
//			process_ReadWrite(arg0_0, arg0_1, arg1_0, arg1_1, in, out, xdim0, offset, tile_x, sizex, sizey);
//		}
//
//		for(int j = 0; j < tile_count; j++){
//			//#pragma HLS dataflow
//			unsigned short offset = tile_mem[j] & 0xffff;
//			unsigned short tile_x = tile_mem[j] >> 16;
//			process_ReadWrite(arg1_0, arg1_1, arg0_0, arg0_1, in, out, xdim0, offset, tile_x, sizex, sizey);
//		}

		process_ReadWrite_dataflow(arg0_0, arg0_1, arg0_2, arg0_3, arg1_0, arg1_1, arg1_2, arg1_3/*, in, out*/, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg1_1, arg1_2, arg1_3, arg0_0, arg0_1, arg0_2, arg0_3/*, in, out*/, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}
