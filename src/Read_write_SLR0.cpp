#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>

static void read_tile(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer0, struct data_G data_g, unsigned short start){

	unsigned short tile_x = data_g.tile_x;
	unsigned short offset_x = data_g.offset_x;
	unsigned short end_z = data_g.grid_sizez;
	unsigned short xblocks = data_g.xblocks;

	unsigned short offset_y = (data_g.offset_y >> 1);
	unsigned short tile_y = (data_g.tile_y >> 1);
	unsigned int plane_size = (data_g.plane_size >> 1);

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_out_itr = end_z * tile_y;
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



static void combine_tile(hls::stream<uint512_dt> &rd_buffer0, hls::stream<uint512_dt> &rd_buffer1,
		hls::stream<uint512_dt> &combined_buffer, struct data_G data_g){

	unsigned short tile_x = data_g.tile_x;
	unsigned short tile_y = data_g.tile_y;
	unsigned short end_z = data_g.grid_sizez;
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	unsigned int total_itr0 = end_z * ((tile_y) >> 1);
	unsigned int total_itr = (total_itr0 * end_index) << 1;
	unsigned short i = 0;
	bool flag = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=1
		if(i >= (end_index << 1)){
			i = 0;
		}

		if(i < end_index){
			flag = 0;
		} else {
			flag = 1;
		}

		if(flag){
			combined_buffer << rd_buffer1.read();
		} else {
			combined_buffer << rd_buffer0.read();
		}

		i++;
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
		#pragma HLS loop_tripcount min=1000 max=2000 avg=1500
		for(unsigned short i = 0; i < tile_y; i = i+1){
			#pragma HLS loop_tripcount min=10 max=20 avg=15
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS loop_tripcount min=10 max=20 avg=15
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
		#pragma HLS loop_tripcount min=1000 max=2000 avg=1500
		for(unsigned short i = 0; i < tile_y; i = i+1){
			#pragma HLS loop_tripcount min=10 max=20 avg=15
			for (unsigned short j = 0; j < end_index; j++){
				#pragma HLS loop_tripcount min=10 max=20 avg=15
				#pragma HLS PIPELINE II=2
				uint512_dt tmp;
				tmp.range(255,0) = in.read();
				tmp.range(511,256) = in.read();
				bool condx = j >= 2 || offset_x == 0;
				bool condy = i >= 24 || offset_y == 0;
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

static void axis2_fifo512(hls::stream <t_pkt> &inl, hls::stream <t_pkt> &inu, hls::stream<uint512_dt> &out, unsigned int tot_itr){
	for (int itr = 0; itr < tot_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp0 = inl.read();
		t_pkt tmp1 = inu.read();
		uint512_dt tmp_d;
		tmp_d.range(255,0) = tmp0.data;
		tmp_d.range(511,256) = tmp1.data;
		out << tmp_d;
	}
}

static void fifo512_2axis(hls::stream <uint512_dt> &in, hls::stream<t_pkt> &outl,  hls::stream<t_pkt> &outu, unsigned int tot_itr){
	for (int itr = 0; itr < tot_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp0, tmp1;
		uint512_dt tmp_d = in.read();
		tmp0.data = tmp_d.range(255,0);
		tmp1.data = tmp_d.range(511,256);
		outl.write(tmp0);
		outu.write(tmp1);
	}
}



static void separate_tile(hls::stream<uint512_dt> &in_buffer, hls::stream<uint512_dt> &wr_buffer0, hls::stream<uint512_dt> &wr_buffer1, struct data_G data_g){

	unsigned short offset_x = data_g.offset_x;
	unsigned short offset_y = data_g.offset_y;

	unsigned short adjust_x = (offset_x == 0?  0 : 32);
	unsigned short adjust_x_b = (offset_x == 0?  0 : 2);
	unsigned short adjust_y = (offset_y == 0?  0 : 24);

	unsigned short tile_x = data_g.tile_x - adjust_x;
	unsigned short tile_y = data_g.tile_y - adjust_y;

	unsigned short end_z = data_g.grid_sizez;
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	unsigned int total_itr0 = end_z * ((tile_y) >> 1);
	unsigned int total_itr = (total_itr0 * end_index) << 1;
	unsigned short i = 0;
	bool flag = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=1
		if(i >= (end_index << 1)){
			i = 0;
		}

		if(i < end_index){
			flag = 0;
		} else {
			flag = 1;
		}

		if(flag){
			wr_buffer1 << in_buffer.read();
		} else {
			wr_buffer0 << in_buffer.read();
		}

		i++;
	}
}



static void write_tile(uint512_dt*  arg0, hls::stream<uint512_dt> &wr_buffer0, struct data_G data_g, unsigned short start){
	unsigned short offset_x = data_g.offset_x;


	unsigned short adjust_x = (offset_x == 0?  0 : 32);
	unsigned short adjust_x_b = (offset_x == 0?  0 : 2);
	unsigned short adjust_y = (data_g.offset_y == 0?  0 : 24);

	unsigned short tile_x = data_g.tile_x - adjust_x;
	unsigned short end_z = data_g.grid_sizez;
	unsigned short xblocks = data_g.xblocks;

	unsigned short offset_y = (data_g.offset_y >> 1);
	unsigned short tile_y = ((data_g.tile_y - adjust_y) >> 1);
	unsigned int plane_size = (data_g.plane_size >> 1);

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));

	unsigned int total_out_itr = end_z * tile_y;
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
		unsigned short tot_y_offset0 = (offset_y + i + (adjust_y >> 1));
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;
		i = i + 1;
		for (unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=10 max=20 avg=15
			arg0[base_index0 + adjust_x_b + j] = wr_buffer0.read();
		}
	}
}

static void process_ReadWrite (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
				   hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
				   hls::stream <t_pkt> &inl, hls::stream <t_pkt> &inu,
				   hls::stream <t_pkt> &outl, hls::stream <t_pkt> &outu,
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

	unsigned short adjust_y = (data_g.offset_y == 0?  0 : 24);
	unsigned short adjust_x = (offset_x == 0?  0 : 32);
	unsigned int tile_plane_size = (data_g.tile_x >> SHIFT_BITS) * (data_g.tile_y);
	unsigned int tile_plane_size_W = ((data_g.tile_x-adjust_x) >> SHIFT_BITS) * (data_g.tile_y-adjust_y);
	unsigned int totol_iter = tile_plane_size * data_g.grid_sizez;
	unsigned int totol_iter_R = totol_iter >> 2;
	unsigned int totol_iter_W = (tile_plane_size_W * data_g.grid_sizez) >> 2;


	#pragma HLS dataflow
	read_tile(arg0_0, rd_bufferArr[0], data_g, 0);
	axis2_fifo512(inl, inu, rd_bufferArr[1], totol_iter_R);

	combine_tile(rd_bufferArr[0], rd_bufferArr[1], streamArray_512[0], data_g);
	stream_convert_512_256(streamArray_512[0], streamArray[0], data_g);

	fifo256_2axis(streamArray[0], out, totol_iter);
	axis2_fifo256(in, streamArray[31], totol_iter);

	stream_convert_256_512(streamArray[31],streamArray_512[1], data_g);
	separate_tile(streamArray_512[1], wr_bufferArr[0], wr_bufferArr[1], data_g);

	write_tile(arg1_0, wr_bufferArr[0], data_g, 0);
	fifo512_2axis(wr_bufferArr[1], outl, outu, totol_iter_W);





}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
				   hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
				   hls::stream <t_pkt> &inl, hls::stream <t_pkt> &inu,
				   hls::stream <t_pkt> &outl, hls::stream <t_pkt> &outu,
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
		process_ReadWrite(arg0_0,  arg1_0,  in, out, inl, inu, outl, outu, xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);
		j++;
	}
}

//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT


extern "C" {
void stencil_Read_Write_SLR0(
		uint512_dt*  arg0_0,
		uint512_dt*  arg1_0,

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
		hls::stream <t_pkt> &out,
		hls::stream <t_pkt> &inl,
		hls::stream <t_pkt> &inu,
		hls::stream <t_pkt> &outl,
		hls::stream <t_pkt> &outu){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 num_read_outstanding=16 num_write_outstanding=16 latency=30
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64 num_read_outstanding=16 num_write_outstanding=16 latency=30

	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem3

	#pragma HLS INTERFACE s_axilite port = arg0_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_0 bundle = control


	#pragma HLS INTERFACE s_axilite port = tile bundle = control
	#pragma HLS INTERFACE axis port = tile_s_out  register
	#pragma HLS INTERFACE axis port = in  register
	#pragma HLS INTERFACE axis port = out register
	#pragma HLS INTERFACE axis port = inl  register
	#pragma HLS INTERFACE axis port = inu register
	#pragma HLS INTERFACE axis port = outl  register
	#pragma HLS INTERFACE axis port = outu register

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
		process_ReadWrite_dataflow(arg0_0, arg1_0, in, out, inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg0_0, in, out, inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}
