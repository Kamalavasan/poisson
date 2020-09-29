#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <stencil.h>
#include <stencil.cpp>



//--------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------- SLR crossing SLR1 -----------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------




void process_SLR (hls::stream <t_pkt> &in, hls::stream <t_pkt> &out,
		const int xdim0, const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){


    static hls::stream<uint256_dt> streamArray[40 + 1];
    #pragma HLS STREAM variable = streamArray depth = 2

    struct data_G data_g;
    data_g.sizex = size_x;
    data_g.sizey = size_y;
    data_g.sizez = size_z;

    data_g.offset_x = 0;
    data_g.tile_x = size_x+2;
    data_g.offset_y = 0;
    data_g.tile_y = size_y+2;


	data_g.xblocks = (data_g.tile_x >> SHIFT_BITS);
	data_g.grid_sizey = size_y + 2;
	data_g.grid_sizez = size_z+2;
	data_g.limit_z = size_z+3;

	unsigned short tile_y_1 = (data_g.tile_y - 1);
	unsigned int plane_size = data_g.xblocks * data_g.tile_y;

	data_g.plane_diff = data_g.xblocks * tile_y_1;
	data_g.line_diff = data_g.xblocks - 1;
	data_g.gridsize_pr = plane_size * (data_g.limit_z);

	unsigned int gridsize_da = plane_size * (data_g.grid_sizez);


	#pragma HLS dataflow
    axis2_fifo256(in, streamArray[0], gridsize_da);

//    process_tile( streamArray[0], streamArray[1], data_g);
//    process_tile( streamArray[1], streamArray[2], size_x, size_y, offset, data_g);
//    process_tile( streamArray[2], streamArray[3], size_x, size_y, offset, data_g);
//    process_tile( streamArray[3], streamArray[4], size_x, size_y, offset, data_g);
//    process_tile( streamArray[4], streamArray[5], size_x, size_y, offset, data_g);
//
//	process_tile( streamArray[5], streamArray[6], size_x, size_y, offset, data_g);
//	process_tile( streamArray[6], streamArray[7], size_x, size_y, offset, data_g);
//	process_tile( streamArray[7], streamArray[8], size_x, size_y, offset, data_g);
//	process_tile( streamArray[8], streamArray[9], size_x, size_y, offset, data_g);
//	process_tile( streamArray[9], streamArray[10], size_x, size_y, offset, data_g);
//
//    process_tile( streamArray[10], streamArray[11], size_x, size_y, offset, data_g);
//    process_tile( streamArray[11], streamArray[12], size_x, size_y, offset, data_g);
//    process_tile( streamArray[12], streamArray[13], size_x, size_y, offset, data_g);
//    process_tile( streamArray[13], streamArray[14], size_x, size_y, offset, data_g);
//    process_tile( streamArray[14], streamArray[15], size_x, size_y, offset, data_g);
//
//	process_tile( streamArray[15], streamArray[16], size_x, size_y, offset, data_g);
//	process_tile( streamArray[16], streamArray[17], size_x, size_y, offset, data_g);
//	process_tile( streamArray[17], streamArray[18], size_x, size_y, offset, data_g);
//	process_tile( streamArray[18], streamArray[19], size_x, size_y, offset, data_g);
//	process_tile( streamArray[19], streamArray[20], size_x, size_y, offset, data_g);

	fifo256_2axis(streamArray[0], out, gridsize_da);


}

extern "C" {
void stencil_SLR2(
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim0,
		const int count,
		hls::stream <t_pkt> &in,
		hls::stream <t_pkt> &out
		){

	#pragma HLS INTERFACE axis port = in register
	#pragma HLS INTERFACE axis port = out register

	#pragma HLS INTERFACE s_axilite port = sizex bundle = control
	#pragma HLS INTERFACE s_axilite port = sizey bundle = control
	#pragma HLS INTERFACE s_axilite port = sizez bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim0 bundle = control
	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control





	for(unsigned short itr =  0; itr < 2*count ; itr++){
		process_SLR( in, out, xdim0, sizex, sizey, sizez);

	}

}
}
