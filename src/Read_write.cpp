#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>
#include "stencil.cpp"


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
	unsigned short offset_x = ((data_g.offset_x) >> 3);
	unsigned int plane_size = (data_g.plane_size >> 3);
	unsigned short offset_128 = data_g.offset_x & 127;
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1

	switch(offset_128){
		case 0: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;} // only first two are new values
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 96: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 1; adjust[5] = 1; adjust[6] = 0; adjust[7] = 0; break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
	}

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
			rd_buffer0 << arg0[base_index + j + adjust[0]];
			rd_buffer1 << arg1[base_index + j + adjust[1]];
			rd_buffer2 << arg2[base_index + j + adjust[2]];
			rd_buffer3 << arg3[base_index + j + adjust[3]];

			rd_buffer4 << arg4[base_index + j + adjust[4]];
			rd_buffer5 << arg5[base_index + j + adjust[5]];
			rd_buffer6 << arg6[base_index + j + adjust[6]];
			rd_buffer7 << arg7[base_index + j + adjust[7]];

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
	unsigned short adjust_y = (data_g.offset_y != 0 ? 3 : 0);
	unsigned short tile_y = (data_g.tile_y - adjust_y);

	unsigned short xblocks = (data_g.xblocks >> 3);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short offset_x = ((data_g.offset_x) >> 3);
	unsigned int plane_size = (data_g.plane_size >> 3);
	unsigned short xblocksP = 1;

	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = end_z * tile_y;

	unsigned short offset_128 = data_g.offset_x & 127;
	unsigned char fset = (offset_x != 0) ? 1 : 0;
	printf("fset is %d\n", fset);
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1
	switch(offset_128){
		case 0: {adjust[0] = fset; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 1; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 96: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 1; adjust[5] = 1; adjust[6] = 1; adjust[7] = 0; break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
	}

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		unsigned short k = itr / tile_y;
		unsigned short i = itr % tile_y;
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i+adjust_y);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;

//		#pragma HLS DATAFLOW
		for(unsigned short j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=8 max=32 avg=16
			arg0[base_index0 + j +adjust[0]] = rd_buffer0.read();
			arg1[base_index0 + j +adjust[1]] = rd_buffer1.read();
			arg2[base_index0 + j +adjust[2]] = rd_buffer2.read();
			arg3[base_index0 + j +adjust[3]] = rd_buffer3.read();
			arg4[base_index0 + j +adjust[4]] = rd_buffer4.read();
			arg5[base_index0 + j +adjust[5]] = rd_buffer5.read();
			arg6[base_index0 + j +adjust[6]] = rd_buffer6.read();
			arg7[base_index0 + j +adjust[7]] = rd_buffer7.read();
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
	bool adjust_x = (data_g.offset_x != 0) ? 1 : 0;
	unsigned short offset_128 = data_g.offset_x & 127;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2
		uint512_dt Tmp0 = in0.read();
		uint512_dt Tmp1 = in1.read();
		uint512_dt Tmp2 = in2.read();
		uint512_dt Tmp3 = in3.read();
		uint512_dt Tmp4 = in4.read();
		uint512_dt Tmp5 = in5.read();
		uint512_dt Tmp6 = in6.read();
		uint512_dt Tmp7 = in7.read();

		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

		switch(offset_128){
			case 0:  { tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
			case 32: { tmp0 = Tmp2; tmp1 = Tmp3; tmp2=Tmp4; tmp3=Tmp5; tmp4=Tmp6; tmp5=Tmp7; tmp6=Tmp0; tmp7=Tmp1; break;}
			case 64: { tmp0 = Tmp4; tmp1 = Tmp5; tmp2=Tmp6; tmp3=Tmp7; tmp4=Tmp0; tmp5=Tmp1; tmp6=Tmp2; tmp7=Tmp3; break;}
			case 96: { tmp0 = Tmp6; tmp1 = Tmp7; tmp2=Tmp0; tmp3=Tmp1; tmp4=Tmp2; tmp5=Tmp3; tmp6=Tmp4; tmp7=Tmp5; break;}
			default :{ tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
		}

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

	bool adjust_x = (data_g.offset_x != 0) ? 1 : 0;
	bool adjust_y = (data_g.offset_y != 0 ? 1 : 0);

	unsigned short offset_128 = data_g.offset_x & 127;

	unsigned short i = 0, j = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2

		if(i >= end_index){
			i = 0;
			j++;
		}

		if(j >= tile_y){
			j = 0;
		}

		bool cond_y = (!adjust_y || j >= 3);
//		bool cond_x = (!adjust_x || i >= 2);

		i++;

		uint512_dt Tmp0, Tmp1, Tmp2, Tmp3, Tmp4, Tmp5, Tmp6, Tmp7;
		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

		Tmp0.range(255,0) = in0.read();
		Tmp0.range(511,256) = in0.read();

		Tmp1.range(255,0) = in1.read();
		Tmp1.range(511,256) = in1.read();

		Tmp2.range(255,0) = in2.read();
		Tmp2.range(511,256) = in2.read();

		Tmp3.range(255,0) = in3.read();
		Tmp3.range(511,256) = in3.read();

		Tmp4.range(255,0) = in4.read();
		Tmp4.range(511,256) = in4.read();

		Tmp5.range(255,0) = in5.read();
		Tmp5.range(511,256) = in5.read();

		Tmp6.range(255,0) = in6.read();
		Tmp6.range(511,256) = in6.read();

		Tmp7.range(255,0) = in7.read();
		Tmp7.range(511,256) = in7.read();


		switch(offset_128){
			case 0:  { tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
			case 32: { tmp0 = Tmp6; tmp1 = Tmp7; tmp2=Tmp0; tmp3=Tmp1; tmp4=Tmp2; tmp5=Tmp3; tmp6=Tmp4; tmp7=Tmp5; break;}
			case 64: { tmp0 = Tmp4; tmp1 = Tmp5; tmp2=Tmp6; tmp3=Tmp7; tmp4=Tmp0; tmp5=Tmp1; tmp6=Tmp2; tmp7=Tmp3; break;}
			case 96: { tmp0 = Tmp2; tmp1 = Tmp3; tmp2=Tmp4; tmp3=Tmp5; tmp4=Tmp6; tmp5=Tmp7; tmp6=Tmp0; tmp7=Tmp1; break;}
			default :{ tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
		}

//		if(cond_y){
			out0 << tmp0;
			out1 << tmp1;
			out2 << tmp2;
			out3 << tmp3;
			out4 << tmp4;
			out5 << tmp5;
			out6 << tmp6;
			out7 << tmp7;
//		}

	}
}

static void skipAndInsert(hls::stream<uint512_dt> &in0, hls::stream<uint512_dt> &out0,
		hls::stream<uint512_dt> &in1, hls::stream<uint512_dt> &out1,
		hls::stream<uint512_dt> &in2, hls::stream<uint512_dt> &out2,
		hls::stream<uint512_dt> &in3, hls::stream<uint512_dt> &out3,
		hls::stream<uint512_dt> &in4, hls::stream<uint512_dt> &out4,
		hls::stream<uint512_dt> &in5, hls::stream<uint512_dt> &out5,
		hls::stream<uint512_dt> &in6, hls::stream<uint512_dt> &out6,
		hls::stream<uint512_dt> &in7, hls::stream<uint512_dt> &out7,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 3);
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned short end_indexp1 = end_index + 1;
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * (end_indexp1);

	bool nz_offsetx = (data_g.offset_x != 0) ? 1 : 0;
	char fset  = (data_g.offset_x != 0) ? -1 : 0;
	bool adjust_y = (data_g.offset_y != 0 ? 1 : 0);

	unsigned short offset_128 = data_g.offset_x & 127;


	short adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1
	switch(offset_128){
		case 0: {adjust[0] = fset; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 32: {adjust[0] = 0; adjust[1] = 0; adjust[2] = -1; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 64: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = -1; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
		case 96: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = -1; adjust[7] = 0; break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; adjust[5] = 0; adjust[6] = 0; adjust[7] = 0; break;}
	}

	unsigned short i = 0, j = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=1

		if(i >= end_indexp1){
			i = 0;
			j++;
		}

		if(j >= tile_y){
			j = 0;
		}

		bool cond_y = (!adjust_y || j >= 3);


		uint512_dt Tmp0, Tmp1, Tmp2, Tmp3, Tmp4, Tmp5, Tmp6, Tmp7;
		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

		if(i < end_index){
			Tmp0 = in0.read();
			Tmp1 = in1.read();
			Tmp2 = in2.read();
			Tmp3 = in3.read();
			Tmp4 = in4.read();
			Tmp5 = in5.read();
			Tmp6 = in6.read();
			Tmp7 = in7.read();
		}

		tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7;
//		switch(offset_128){
//			case 0:  { tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
//			case 32: { tmp0 = Tmp6; tmp1 = Tmp7; tmp2=Tmp0; tmp3=Tmp1; tmp4=Tmp2; tmp5=Tmp3; tmp6=Tmp4; tmp7=Tmp5; break;}
//			case 64: { tmp0 = Tmp4; tmp1 = Tmp5; tmp2=Tmp6; tmp3=Tmp7; tmp4=Tmp0; tmp5=Tmp1; tmp6=Tmp2; tmp7=Tmp3; break;}
//			case 96: { tmp0 = Tmp2; tmp1 = Tmp3; tmp2=Tmp4; tmp3=Tmp5; tmp4=Tmp6; tmp5=Tmp7; tmp6=Tmp0; tmp7=Tmp1; break;}
//			default :{ tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7; break;}
//		}

		bool cond0 = (cond_y && ((i+adjust[0]) >= 0)  &&  ((i+adjust[0]) < end_index) );
		bool cond1 = (cond_y && ((i+adjust[1]) >= 0)  &&  ((i+adjust[1]) < end_index) );
		bool cond2 = (cond_y && ((i+adjust[2]) >= 0)  &&  ((i+adjust[2]) < end_index) );
		bool cond3 = (cond_y && ((i+adjust[3]) >= 0)  &&  ((i+adjust[3]) < end_index) );
		bool cond4 = (cond_y && ((i+adjust[4]) >= 0)  &&  ((i+adjust[4]) < end_index) );
		bool cond5 = (cond_y && ((i+adjust[5]) >= 0)  &&  ((i+adjust[5]) < end_index) );
		bool cond6 = (cond_y && ((i+adjust[6]) >= 0)  &&  ((i+adjust[6]) < end_index) );
		bool cond7 = (cond_y && ((i+adjust[7]) >= 0)  &&  ((i+adjust[7]) < end_index) );

		if(cond0){	out0 << tmp0;}
		if(cond1){	out1 << tmp1;}
		if(cond2){	out2 << tmp2;}
		if(cond3){	out3 << tmp3;}
		if(cond4){	out4 << tmp4;}
		if(cond5){	out5 << tmp5;}
		if(cond6){	out6 << tmp6;}
		if(cond7){	out7 << tmp7;}
		i++;

	}

}


static void stream_convert_256_2048(hls::stream<uint256_dt> &in0, hls::stream<uint256_dt> &in1,
		hls::stream<uint256_dt> &in2, hls::stream<uint256_dt> &in3,
		hls::stream<uint256_dt> &in4, hls::stream<uint256_dt> &in5,
		hls::stream<uint256_dt> &in6, hls::stream<uint256_dt> &in7,
		hls::stream<uint1024_dt> &out0, hls::stream<uint1024_dt> &out1, struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 4); // 16*4 = 128bytes
	unsigned short end_index = (tile_x >> (SHIFT_BITS));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2
		uint1024_dt tmp0, tmp1, tmp2, tmp3;
		tmp0.range(255,0)    = in0.read();
		tmp0.range(511,256)  = in0.read();
		tmp0.range(767,512)  = in1.read();
		tmp0.range(1023,768) = in1.read();

		tmp1.range(255,0)    = in2.read();
		tmp1.range(511,256)  = in2.read();
		tmp1.range(767,512)  = in3.read();
		tmp1.range(1023,768) = in3.read();

		tmp2.range(255,0)    = in4.read();
		tmp2.range(511,256)  = in4.read();
		tmp2.range(767,512)  = in5.read();
		tmp2.range(1023,768) = in5.read();

		tmp3.range(255,0)    = in6.read();
		tmp3.range(511,256)  = in6.read();
		tmp3.range(767,512)  = in7.read();
		tmp3.range(1023,768) = in7.read();

		out0 << tmp0;
		out1 << tmp1;
		out0 << tmp2;
		out1 << tmp3;
	}
}

static void stream_convert_2048_256(hls::stream<uint1024_dt> &in0, hls::stream<uint1024_dt> &in1,
		hls::stream<uint256_dt> &out0, hls::stream<uint256_dt> &out1,
		hls::stream<uint256_dt> &out2, hls::stream<uint256_dt> &out3,
		hls::stream<uint256_dt> &out4, hls::stream<uint256_dt> &out5,
		hls::stream<uint256_dt> &out6, hls::stream<uint256_dt> &out7,  struct data_G data_g){
	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x >> 4);  // 16*4 =128 bytes
	unsigned short end_index = (tile_x >> (SHIFT_BITS));
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = total_itr0 * end_index;

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2
		uint1024_dt tmp0, tmp1, tmp2, tmp3;
		tmp0 = in0.read();
		tmp1 = in1.read();
		tmp2 = in0.read();
		tmp3 = in1.read();

		out0 << tmp0.range(255,0);
		out0 << tmp0.range(511,256);
		out1 << tmp0.range(767,512);
		out1 << tmp0.range(1023,768);

		out2 << tmp1.range(255,0);
		out2 << tmp1.range(511,256);
		out3 << tmp1.range(767,512);
		out3 << tmp1.range(1023,768);

		out4 << tmp2.range(255,0);
		out4 << tmp2.range(511,256);
		out5 << tmp2.range(767,512);
		out5 << tmp2.range(1023,768);

		out6 << tmp3.range(255,0);
		out6 << tmp3.range(511,256);
		out7 << tmp3.range(767,512);
		out7 << tmp3.range(1023,768);

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
								hls::stream <t_pkt_1024> &inl, hls::stream <t_pkt_1024> &inu,
								hls::stream <t_pkt_1024> &outl, hls::stream <t_pkt_1024> &outu,
							   const int xdim0, const unsigned short offset_x, const unsigned short tile_x,
							   const unsigned short offset_y, unsigned short tile_y,
							   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){


    static hls::stream<uint256_dt> streamArrayrd[8];
    static hls::stream<uint256_dt> streamArraywr[8];
    static hls::stream<uint512_dt> rd_bufferArr[8];
    static hls::stream<uint512_dt> wr_bufferArr[8];
    static hls::stream<uint512_dt> skip_bufferArr[8];
    static hls::stream<uint1024_dt> streamArray_1024[4];

    #pragma HLS STREAM variable = streamArrayrd depth = 2
	#pragma HLS STREAM variable = streamArraywr depth = 2
	#pragma HLS STREAM variable = streamArray_1024 depth = 64
	#pragma HLS STREAM variable = rd_bufferArr depth = 64  //max_depth_16
	#pragma HLS STREAM variable = wr_bufferArr depth = 64  // max_depth_16
	#pragma HLS STREAM variable = skip_bufferArr depth = 64


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

	data_g.plane_size = data_g.xblocks * (data_g.grid_sizey);

	unsigned int tile_plane_size = (data_g.tile_x >> (SHIFT_BITS+3)) * data_g.tile_y;
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

	// going out
	fifo2048_2axis(streamArray_1024[0], streamArray_1024[1], outl, outu, totol_iter);
	axis2_fifo2048(inl, inu, streamArray_1024[2], streamArray_1024[3], totol_iter);

	// coming in

	stream_convert_2048_256(streamArray_1024[2], streamArray_1024[3],
			streamArraywr[0], streamArraywr[1], streamArraywr[2], streamArraywr[3],
			streamArraywr[4], streamArraywr[5], streamArraywr[6], streamArraywr[7], data_g);

	stream_convert_256_512(streamArraywr[0], skip_bufferArr[0], streamArraywr[1], skip_bufferArr[1],
			streamArraywr[2], skip_bufferArr[2], streamArraywr[3], skip_bufferArr[3],
			streamArraywr[4], skip_bufferArr[4], streamArraywr[5], skip_bufferArr[5],
			streamArraywr[6], skip_bufferArr[6], streamArraywr[7], skip_bufferArr[7],
			data_g);

	skipAndInsert(skip_bufferArr[0], wr_bufferArr[0], skip_bufferArr[1], wr_bufferArr[1],
			skip_bufferArr[2], wr_bufferArr[2], skip_bufferArr[3], wr_bufferArr[3],
			skip_bufferArr[4], wr_bufferArr[4], skip_bufferArr[5], wr_bufferArr[5],
			skip_bufferArr[6], wr_bufferArr[6], skip_bufferArr[7], wr_bufferArr[7],
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
										hls::stream <t_pkt_1024> &inl, hls::stream <t_pkt_1024> &inu,
										hls::stream <t_pkt_1024> &outl, hls::stream <t_pkt_1024> &outu,
									   const int xdim0, unsigned int tile_memx[], unsigned short tilex_count ,
									   unsigned int tile_memy[], unsigned short tiley_count,
									   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){

	unsigned int toltal_itr = tilex_count*tiley_count;
	unsigned short i = 0, j = 0;
	for(unsigned int itr = 0; itr < toltal_itr; itr++){
		if(j == tilex_count){
			j = 0;
			i++;
		}
		unsigned short offset_x = tile_memx[j] & 0xffff;
		unsigned short tile_x   = tile_memx[j] >> 16;
		unsigned short offset_y = tile_memy[i] & 0xffff;
		unsigned short tile_y   = tile_memy[i] >> 16;
		j++;


		printf("Read Write: Itr:%d offset_x:%d offset_y:%d\n", itr, offset_x, offset_y);
		process_ReadWrite(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3,
				arg0_4, arg1_4, arg0_5, arg1_5, arg0_6, arg1_6, arg0_7, arg1_7,
				inl, inu, outl, outu,
				xdim0, offset_x, tile_x, offset_y, tile_y, size_x, size_y, size_z);


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
		const int count,
		hls::stream <t_pkt_32> &tile_s_out,
		hls::stream <t_pkt_1024> &inl,
		hls::stream <t_pkt_1024> &inu,
		hls::stream <t_pkt_1024> &outl,
		hls::stream <t_pkt_1024> &outu){

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

	#pragma HLS INTERFACE axis port = tile_s_out  register
	#pragma HLS INTERFACE axis port = inl  register
	#pragma HLS INTERFACE axis port = inu register
	#pragma HLS INTERFACE axis port = outl  register
	#pragma HLS INTERFACE axis port = outu register

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



	for(int i =  0; i < count; i++){
		process_ReadWrite_dataflow(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3, arg0_4, arg1_4, arg0_5, arg1_5, arg0_6, arg1_6, arg0_7, arg1_7,
				inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg0_0, arg1_1, arg0_1, arg1_2, arg0_2, arg1_3, arg0_3, arg1_4, arg0_4, arg1_5, arg0_5, arg1_6, arg0_6, arg1_7, arg0_7,
				inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}


