#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <stencil.h>
#include <stencil.cpp>


//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT
//--sc ops_poisson_kernel_stencil_SLR0.out:ops_poisson_kernel_stencil_SLR1.in
//--sc ops_poisson_kernel_stencil_SLR0.in:ops_poisson_kernel_stencil_SLR1.out


//--------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------- SLR crossing SLR1 -----------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------




void process_SLR (hls::stream <t_pkt> &in1, hls::stream <t_pkt> &out1, hls::stream <t_pkt> &in2, hls::stream <t_pkt> &out2,
		const int xdim0, const unsigned short offset_x, const unsigned short tile_x,
		unsigned short offset_y, unsigned short tile_y,
		const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){

    static hls::stream<uint256_dt> streamArray[40 + 1];
    #pragma HLS STREAM variable = streamArray depth = 2

    struct data_G data_g;
    data_g.sizex = size_x;
    data_g.sizey = size_y;
    data_g.sizez = size_z;

    data_g.offset_x = offset_x;
    data_g.tile_x = tile_x;
    data_g.offset_y = offset_y;
    data_g.tile_y = tile_y;


	data_g.xblocks = (tile_x >> SHIFT_BITS);
	data_g.grid_sizey = size_y + 2;
	data_g.grid_sizez = size_z+2;
	data_g.limit_z = size_z+3;

	unsigned short tiley_1 = (data_g.tile_y - 1);
	unsigned int plane_size = data_g.xblocks * data_g.tile_y;

	data_g.plane_diff = data_g.xblocks * tiley_1;
	data_g.line_diff = data_g.xblocks - 1;
	data_g.gridsize_pr = plane_size * (data_g.limit_z);

	unsigned int gridsize_da = plane_size * (data_g.grid_sizez);



	#pragma HLS dataflow
    axis2_fifo256(in1, streamArray[0], gridsize_da);

    process_tile( streamArray[0], streamArray[1], data_g);
    process_tile( streamArray[1], streamArray[2], data_g);
    process_tile( streamArray[2], streamArray[3], data_g);
    process_tile( streamArray[3], streamArray[4], data_g);

	fifo256_2axis(streamArray[4], out1, gridsize_da);
	axis2_fifo256(in2, streamArray[40], gridsize_da);

	process_tile( streamArray[40], streamArray[5], data_g);
	process_tile( streamArray[5], streamArray[6], data_g);
	process_tile( streamArray[6], streamArray[7], data_g);
	process_tile( streamArray[7], streamArray[8], data_g);

	fifo256_2axis(streamArray[8], out2, gridsize_da);


}

extern "C" {
void stencil_SLR1(
		const int tilex_count,
		const int tiley_count,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count,

		hls::stream <t_pkt_32> &tile_s_in,
		hls::stream <t_pkt_32> &tile_s_out,

		hls::stream <t_pkt> &in1,
		hls::stream <t_pkt> &out1,
		hls::stream <t_pkt> &in2,
		hls::stream <t_pkt> &out2
		){

	#pragma HLS INTERFACE axis port = in1 register
	#pragma HLS INTERFACE axis port = out1 register
	#pragma HLS INTERFACE axis port = in2 register
	#pragma HLS INTERFACE axis port = out2 register
	#pragma HLS INTERFACE axis port = tile_s_in register
	#pragma HLS INTERFACE axis port = tile_s_out register

	#pragma HLS INTERFACE s_axilite port = tilex_count bundle = control
	#pragma HLS INTERFACE s_axilite port = tiley_count bundle = control
	#pragma HLS INTERFACE s_axilite port = sizey bundle = control
	#pragma HLS INTERFACE s_axilite port = sizex bundle = control
	#pragma HLS INTERFACE s_axilite port = sizez bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0 bundle = control
	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control

	unsigned int tile_memx[256];
	unsigned int tile_memy[256];

	for(int j = 0; j < tilex_count; j++){
		t_pkt_32 tmp_s;
		tmp_s = tile_s_in.read();
		tile_s_out.write(tmp_s);
		tile_memx[j] = tmp_s.data;
	}

	for(int j = 0; j < tiley_count; j++){
		t_pkt_32 tmp_s;
		tmp_s = tile_s_in.read();
		tile_s_out.write(tmp_s);
		tile_memy[j] = tmp_s.data;
	}

	unsigned int total_count = (count << 1) * tilex_count*tiley_count;
	unsigned short i = 0, j = 0, k = 0;
	unsigned short i_dum = 0, j_dum = 0, k_dum = 0;
	for(unsigned int itr= 0;  itr < total_count; itr++){
		bool cond_k = (k == tiley_count - 1);
		bool cond_j = (j == tilex_count -1);
		if(cond_k){
			k_dum = 0;
		}

		if(cond_j && cond_k){
			i_dum = i + 1;
			j_dum = 0;
		} else if(cond_k){
			j_dum = j + 1;
		}
		i = i_dum;
		j = j_dum;
		k = k_dum;
		k_dum++;

		unsigned short offset_x = tile_memx[j] & 0xffff;
		unsigned short tile_x   = tile_memx[j] >> 16;
		unsigned short offset_y = tile_memy[k] & 0xffff;
		unsigned short tile_y   = tile_memy[k] >> 16;

		process_SLR( in1, out1, in2, out2, xdim0, offset_x, tile_x, offset_y, tile_y, sizex, sizey, sizez);
	}

}
}
