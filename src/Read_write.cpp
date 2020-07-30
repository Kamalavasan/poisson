#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>

static void read_tile(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer, struct data_G data_g){
	unsigned short tile_x = data_g.tile_x;
	unsigned short offset_x = data_g.offset_x;
	unsigned short tile_y = data_g.tile_y;
	unsigned short offset_y = data_g.offset_y;
	unsigned short end_z = data_g.grid_sizez;

	unsigned short xblocks = data_g.xblocks;
	unsigned int plane_size = data_g.plane_size;

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	for(unsigned short k = 0; k < end_z; k++){
		for(unsigned short i = 0; i < tile_y; i = i+1){
			unsigned int plane_offset = k* plane_size;
			unsigned int row_offset = xblocks * (offset_y + i);
			unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);
			unsigned int total_offset = plane_offset + row_offset + offset_x_b;
			unsigned int base_index = total_offset;
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS PIPELINE II=1
				rd_buffer << arg0[base_index + j];
			}
		}
	}
}

static void combine_tile(hls::stream<uint512_dt> &rd_buffer0, hls::stream<uint512_dt> &rd_buffer1, hls::stream<uint512_dt> &rd_buffer2, hls::stream<uint512_dt> &rd_buffer3,
		hls::stream<uint512_dt> &combined_buffer, const int xdim0, const int offset, int tile_x, int size_y){
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned short  end_row = size_y+2;
	for(unsigned short i = 0; i < end_row; i = i+2){
		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			combined_buffer << rd_buffer0.read();
		}

		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			combined_buffer << rd_buffer1.read();
		}

//		for (unsigned short j = 0; j < end_index; j++){
//			#pragma HLS PIPELINE II=1
//			combined_buffer << rd_buffer2.read();
//		}
//
//		for (unsigned short j = 0; j < end_index; j++){
//			#pragma HLS PIPELINE II=1
//			combined_buffer << rd_buffer3.read();
//		}
	}
}



static void stream_convert_512_256(hls::stream<uint512_dt> &in, hls::stream<uint256_dt> &out, struct data_G data_g){
	unsigned short offset_x = data_g.offset_x;
	unsigned short offset_y = data_g.offset_y;

	unsigned short tile_x = data_g.tile_x;
	unsigned short tile_y = data_g.tile_y;

	unsigned short end_z = data_g.grid_sizez;
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	for(unsigned short k = 0; k < end_z; k++){
		for(unsigned short i = 0; i < tile_y; i = i+1){
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS PIPELINE II=2
				uint512_dt tmp = in.read();
				uint256_dt var_l = tmp.range(255,0);
				uint256_dt var_h = tmp.range(511,256);;
				out << var_l;
				out << var_h;
			}
		}
	}
}



static void stream_convert_256_512(hls::stream<uint256_dt> &in, hls::stream<uint512_dt> &out, struct data_G data_g){
	unsigned short offset_x = data_g.offset_x;
	unsigned short offset_y = data_g.offset_y;

	unsigned short tile_x = data_g.tile_x;
	unsigned short tile_y = data_g.tile_y;

	unsigned short end_z = data_g.grid_sizez;
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	for(unsigned short k = 0; k < end_z; k++){
		for(unsigned short i = 0; i < tile_y; i = i+1){
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS PIPELINE II=1
				uint512_dt tmp;
				tmp.range(255,0) = in.read();
				tmp.range(511,256) = in.read();
				bool condx = j > 0 || offset_x == 0;
				bool condy = i >= 4 || offset_y == 0;
				bool cond_x_y = condx && condy;
				if(cond_x_y){
					out << tmp;
				}
			}
		}
	}
}

static void axis2_fifo256(hls::stream <t_pkt> &in, hls::stream<uint256_dt> &out, unsigned int tot_itr){

	for (int itr = 0; itr < tot_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp = in.read();
		out << tmp.data;
	}
}

static void fifo256_2axis(hls::stream <uint256_dt> &in, hls::stream<t_pkt> &out, unsigned int tot_itr){

	for (int itr = 0; itr < tot_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp;
		tmp.data = in.read();
		out.write(tmp);
	}
}



static void separate_tile(hls::stream<uint512_dt> &in_buffer, hls::stream<uint512_dt> &wr_buffer0, hls::stream<uint512_dt> &wr_buffer1, hls::stream<uint512_dt> &wr_buffer2,
		hls::stream<uint512_dt> &wr_buffer3, const int xdim0, const int offset, int tile_x, int size_y){
	unsigned short full_size = (tile_x >> (SHIFT_BITS+1));
	unsigned short adjust = offset == 0 ? 0 : 2;
	unsigned short end_index = full_size-adjust;
	unsigned short end_row = size_y + 2;
	for(unsigned short i = 0; i < end_row; i = i+2){
		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			wr_buffer0 << in_buffer.read();
		}

		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			wr_buffer1 << in_buffer.read();
		}
//
//		for (unsigned short j = 0; j < end_index; j++){
//			#pragma HLS PIPELINE II=1
//			wr_buffer2 << in_buffer.read();
//		}
//
//		for (unsigned short j = 0; j < end_index; j++){
//			#pragma HLS PIPELINE II=1
//			wr_buffer3 << in_buffer.read();
//		}
	}
}



static void write_tile(uint512_dt*  arg1, hls::stream<uint512_dt> &wr_buffer, struct data_G data_g){
	unsigned short offset_x = data_g.offset_x;
	unsigned short offset_y = data_g.offset_y;

	unsigned short adjust_x = (offset_x == 0?  0 : 8);
	unsigned short adjust_y = (offset_y == 0?  0 : 8);

	unsigned short tile_x = data_g.tile_x - adjust_x;
	unsigned short tile_y = data_g.tile_y - adjust_y;

	unsigned short end_z = data_g.grid_sizez;

	unsigned short xblocks = data_g.xblocks;
	unsigned int plane_size = data_g.plane_size;

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	for(unsigned short k = 0; k < end_z; k++){
		for(unsigned short i = 0; i < tile_y; i = i+1){
			unsigned int plane_offset = k* plane_size;
			unsigned int row_offset = xblocks * (offset_y + i);
			unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);
			unsigned int total_offset = plane_offset + row_offset + offset_x_b;
			unsigned int base_index = total_offset;
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS PIPELINE II=1
				arg1[base_index + j] = wr_buffer.read();
			}
		}
	}
}

static void process_ReadWrite (uint512_dt*  arg0_0, uint512_dt*  arg0_1,
				   uint512_dt*  arg1_0,  uint512_dt*  arg1_1,
				   hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
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


//	read_tile(arg0_0, rd_bufferArr[0], xdim0, offset, tile_x, size_y, 0);
//	read_tile(arg0_1, rd_bufferArr[1], xdim0, offset, tile_x, size_y, 1);
//
//
//	combine_tile(rd_bufferArr[0], rd_bufferArr[1], rd_bufferArr[2], rd_bufferArr[3], streamArray_512[0], xdim0, offset, tile_x, size_y);
//	stream_convert_512_256(streamArray_512[0], streamArray[0], tile_x, size_y);
	read_tile(arg0_0, rd_bufferArr[0], data_g);
	stream_convert_512_256(rd_bufferArr[0], streamArray[0], data_g);

	fifo256_2axis(streamArray[0], out, totol_iter);
	axis2_fifo256(in, streamArray[31], totol_iter);

	stream_convert_256_512(streamArray[31],wr_bufferArr[0], data_g);
	write_tile(arg1_0, wr_bufferArr[0], data_g);



//	stream_convert_256_512(streamArray[31], streamArray_512[1], tile_x, size_y, offset);
//
//	separate_tile(streamArray_512[1], wr_bufferArr[0], wr_bufferArr[1], wr_bufferArr[2], wr_bufferArr[3], xdim0, offset, tile_x, size_y);
//	write_tile(arg1_0, wr_bufferArr[0], xdim0, offset, tile_x, size_y, 0);
//	write_tile(arg1_1, wr_bufferArr[1], xdim0, offset, tile_x, size_y, 1);


}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg0_1,
				   uint512_dt*  arg1_0,  uint512_dt*  arg1_1,
				   hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
				   const int xdim0, unsigned int tile_memx[], unsigned short tilex_count ,
				   unsigned int tile_memy[], unsigned short tiley_count,
				   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){

	for(int i = 0; i < tilex_count; i++){
			#pragma HLS dataflow
			unsigned short offset_x = tile_memx[i] & 0xffff;
			unsigned short tile_x   = tile_memx[i] >> 16;
			for(int j = 0; j < tiley_count; j++){
				unsigned short offset_y = tile_memy[j] & 0xffff;
				unsigned short tile_y   = tile_memy[j] >> 16;
				printf("xdim0: %d, tile_x: %d offset_x:%d tile_y:%d offset_y:%d\n",xdim0, tile_x, offset_x,  tile_y, offset_y);
				process_ReadWrite(arg0_0, arg0_1, arg1_0, arg1_1, in, out, xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);
			}
		}

}

//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT


extern "C" {
void stencil_Read_Write(
		uint512_dt*  arg0_0,
		uint512_dt*  arg0_1,

		uint512_dt*  arg1_0,
		uint512_dt*  arg1_1,

		const unsigned int* tile,
		const int tilex_count,
		const int tiley_count,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count,

		hls::stream <t_pkt_32> &tile_s_out,
		hls::stream <t_pkt> &in,
		hls::stream <t_pkt> &out){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 num_read_outstanding=2 num_write_outstanding=2
	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_1 offset = slave bundle = gmem1 max_read_burst_length=64 max_write_burst_length=64 num_read_outstanding=2 num_write_outstanding=2


	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_1 offset = slave bundle = gmem1

	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem4

	#pragma HLS INTERFACE s_axilite port = arg0_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg0_1 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg1_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_1 bundle = control


	#pragma HLS INTERFACE s_axilite port = tile bundle = control
	#pragma HLS INTERFACE axis port = tile_s_out  register
	#pragma HLS INTERFACE axis port = in  register
	#pragma HLS INTERFACE axis port = out register

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
		tile_s_out.write(tmp_s);
	}

	for(int j = 0; j < tiley_count; j++){
			#pragma HLS PIPELINE II=1
			t_pkt_32 tmp_s;
			unsigned int tmp;
			tmp = tile[tilex_count+j];
			tile_memy[j] = tmp;
			tmp_s.data = tmp;
			tile_s_out.write(tmp_s);
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

		process_ReadWrite_dataflow(arg0_0, arg0_1, arg1_0, arg1_1, in, out, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg1_1, arg0_0, arg0_1, in, out, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}
