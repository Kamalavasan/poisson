#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include "../src/stencil.h"
#include <stdio.h>
#include "stencil.cpp"


#define TILE_X (256/16)
#define TILE_Y 256





static void read_to_fifo(uint512_dt*  arg0, hls::stream<uint256_dt> &rd_buffer0_0, hls::stream<uint256_dt> &rd_buffer0_1,
		uint512_dt*  arg1, hls::stream<uint256_dt> &rd_buffer1_0, hls::stream<uint256_dt> &rd_buffer1_1,
		unsigned char offset_a,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short tile_y = (data_g.tile_y);

	unsigned short xblocks = (data_g.xblocks5);
	unsigned short tile_x = (data_g.tile_x5);
	unsigned short offset_x = ((data_g.offset_x5));
	unsigned int plane_size = (data_g.plane_size5);
	unsigned short offset_80 = (data_g.offset_x80);
	unsigned int y_offset_addr = data_g.y_offset_addr;
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1

	switch(offset_80){
		case 0: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 16: {adjust[0] = 1; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;} // only first two are new values
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 48: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 0; adjust[4] = 0;  break;}
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 0;  break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
	}

	unsigned char end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = data_g.total_itr_R;


	unsigned short i = 0, k = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		if(i == tile_y){
			i = 0;
			k++;
		}

		unsigned short k1 = register_it(k);
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = row_offset0 + offset_x_b;
		unsigned int base_index = base_index0 + plane_offset;

		i++;

		for(unsigned char j = 0; j < end_index; j++){
			#pragma HLS loop_tripcount min=1 max=8 avg=8
			#pragma HLS PIPELINE II=1
			uint512_dt tmp0 =  arg0[base_index + j +adjust[offset_a+0]];
			uint512_dt tmp1 =  arg1[base_index + j +adjust[offset_a+1]];

			rd_buffer0_0 << tmp0.range(255,0);
			rd_buffer0_1 << tmp0.range(511,256);

			rd_buffer1_0 << tmp1.range(255,0);
			rd_buffer1_1 << tmp1.range(511,256);
		}

	}
}

static void read_to_fifo1(uint512_dt*  arg0, hls::stream<uint256_dt> &rd_buffer0_0, hls::stream<uint256_dt> &rd_buffer0_1,
		unsigned char offset_a,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short tile_y = (data_g.tile_y);

	unsigned short xblocks = (data_g.xblocks5);
	unsigned short tile_x = (data_g.tile_x5);
	unsigned short offset_x = ((data_g.offset_x5));
	unsigned int plane_size = (data_g.plane_size5);
	unsigned short offset_80 = (data_g.offset_x80);
	unsigned int y_offset_addr = data_g.y_offset_addr;
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1

	switch(offset_80){
		case 0: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 16: {adjust[0] = 1; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;} // only first two are new values
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 48: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 0; adjust[4] = 0;  break;}
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 0;  break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
	}

	unsigned char end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = data_g.total_itr_R;


	unsigned short i = 0, k = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		if(i == tile_y){
			i = 0;
			k++;
		}

		unsigned short k1 = register_it(k);
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = row_offset0 + offset_x_b;
		unsigned int base_index = base_index0 + plane_offset;

		i++;

		for(unsigned char j = 0; j < end_index; j++){
			#pragma HLS loop_tripcount min=1 max=8 avg=8
			#pragma HLS PIPELINE II=1
			uint512_dt tmp0 =  arg0[base_index + j +adjust[offset_a+0]];

			rd_buffer0_0 << tmp0.range(255,0);
			rd_buffer0_1 << tmp0.range(511,256);

		}

	}
}







static void write_from_fifo(uint512_dt*  arg0, hls::stream<uint256_dt> &rd_buffer0_0, hls::stream<uint256_dt> &rd_buffer0_1,
		uint512_dt*  arg1, hls::stream<uint256_dt> &rd_buffer1_0, hls::stream<uint256_dt> &rd_buffer1_1,
		unsigned char offset_a,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short adjust_y = (data_g.offset_y != 0 ? 3 : 0);
	unsigned short tile_y = (data_g.tile_y - adjust_y);

	unsigned char xblocks = (data_g.xblocks5);
	unsigned short tile_x = (data_g.tile_x5);
	unsigned short offset_x = (data_g.offset_x5);
	unsigned int plane_size = (data_g.plane_size5);
	unsigned short xblocksP = 1;
	unsigned int y_offset_addr = data_g.y_offset_addr;

	unsigned char end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = data_g.total_itr_W; //register_it <unsigned int>(end_z * tile_y);

	unsigned short offset_80 = data_g.offset_x80;
	unsigned char fset = (offset_x != 0) ? 1 : 0;
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1
	switch(offset_80){
		case 0: {adjust[0] = fset; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; break;}
		case 16: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 0; adjust[4] = 0;  break;}
		case 48: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 0; break;}
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 1;  break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
	}

	unsigned short i_dum = 0, k_dum = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200

		unsigned short i = i_dum;
		unsigned short k = k_dum;

		if(i_dum == tile_y-1){
			i_dum = 0;
			k_dum++;
		} else {
			i_dum++;
		}
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i+adjust_y);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;


		for(unsigned char j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=8 max=32 avg=16
			uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

			tmp0.range(255,0) = rd_buffer0_0.read();
			tmp0.range(511,256) = rd_buffer0_1.read();

			tmp1.range(255,0) = rd_buffer1_0.read();
			tmp1.range(511,256) = rd_buffer1_1.read();

			arg0[base_index0 + j + adjust[offset_a+0]] = tmp0;
			arg1[base_index0 + j + adjust[offset_a+1]] = tmp1;

		}

	}
}


static void write_from_fifo1(uint512_dt*  arg0, hls::stream<uint256_dt> &rd_buffer0_0, hls::stream<uint256_dt> &rd_buffer0_1,
		unsigned char offset_a,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short offset_y = (data_g.offset_y);
	unsigned short adjust_y = (data_g.offset_y != 0 ? 3 : 0);
	unsigned short tile_y = (data_g.tile_y - adjust_y);

	unsigned char xblocks = (data_g.xblocks5);
	unsigned short tile_x = (data_g.tile_x5);
	unsigned short offset_x = (data_g.offset_x5);
	unsigned int plane_size = (data_g.plane_size5);
	unsigned short xblocksP = 1;
	unsigned int y_offset_addr = data_g.y_offset_addr;

	unsigned char end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned int total_itr = data_g.total_itr_W; //register_it <unsigned int>(end_z * tile_y);

	unsigned short offset_80 = data_g.offset_x80;
	unsigned char fset = (offset_x != 0) ? 1 : 0;
	unsigned char adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1
	switch(offset_80){
		case 0: {adjust[0] = fset; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; break;}
		case 16: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 32: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 0; adjust[4] = 0;  break;}
		case 48: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 0; break;}
		case 64: {adjust[0] = 1; adjust[1] = 1; adjust[2] = 1; adjust[3] = 1; adjust[4] = 1;  break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
	}

	unsigned short i_dum = 0, k_dum = 0;
	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200

		unsigned short i = i_dum;
		unsigned short k = k_dum;

		if(i_dum == tile_y-1){
			i_dum = 0;
			k_dum++;
		} else {
			i_dum++;
		}
		unsigned int plane_offset = k* plane_size;
		unsigned int offset_x_b = offset_x >> (SHIFT_BITS+1);

		unsigned short tot_y_offset0 = (offset_y + i+adjust_y);
		unsigned int row_offset0 = xblocks * tot_y_offset0;
		unsigned int base_index0 = plane_offset + row_offset0 + offset_x_b;


		for(unsigned char j = 0; j < end_index; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS loop_tripcount min=8 max=32 avg=16
			uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

			tmp0.range(255,0) = rd_buffer0_0.read();
			tmp0.range(511,256) = rd_buffer0_1.read();

			arg0[base_index0 + j + adjust[offset_a+0]] = tmp0;


		}

	}
}





static void skipAndInsert(hls::stream<uint256_dt> &in0_0, hls::stream<uint256_dt> &in0_1, hls::stream<uint256_dt> &out0_0, hls::stream<uint256_dt> &out0_1,
		hls::stream<uint256_dt> &in1_0, hls::stream<uint256_dt> &in1_1, hls::stream<uint256_dt> &out1_0, hls::stream<uint256_dt> &out1_1,
		hls::stream<uint256_dt> &in2_0, hls::stream<uint256_dt> &in2_1, hls::stream<uint256_dt> &out2_0, hls::stream<uint256_dt> &out2_1,
		hls::stream<uint256_dt> &in3_0, hls::stream<uint256_dt> &in3_1, hls::stream<uint256_dt> &out3_0, hls::stream<uint256_dt> &out3_1,
		hls::stream<uint256_dt> &in4_0, hls::stream<uint256_dt> &in4_1, hls::stream<uint256_dt> &out4_0, hls::stream<uint256_dt> &out4_1,
		struct data_G data_g){

	unsigned short end_z = data_g.grid_sizez;
	unsigned short tile_y = (data_g.tile_y);
	unsigned short tile_x = (data_g.tile_x5);
	unsigned short end_index = (tile_x >> (SHIFT_BITS+1));
	unsigned short end_indexp1 = end_index + 1;
	unsigned int total_itr0 = end_z * tile_y;
	unsigned int total_itr = register_it <unsigned int>(total_itr0 * (end_indexp1));

	bool nz_offsetx = (data_g.offset_x != 0) ? 1 : 0;
	char fset  = (data_g.offset_x != 0) ? -1 : 0;
	bool adjust_y = (data_g.offset_y != 0 ? 1 : 0);

	unsigned short offset_80 = data_g.offset_x80;


	short adjust[8];
	#pragma HLS ARRAY_PARTITION variable=adjust complete dim=1
	switch(offset_80){
		case 0: {adjust[0] = fset; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; break;}
		case 16: {adjust[0] = 0; adjust[1] = -1; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0;  break;}
		case 32: {adjust[0] = 0; adjust[1] = 0; adjust[2] = -1; adjust[3] = 0; adjust[4] = 0;  break;}
		case 48: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = -1; adjust[4] = 0;  break;}
		case 64: {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = -1;  break;}
		default : {adjust[0] = 0; adjust[1] = 0; adjust[2] = 0; adjust[3] = 0; adjust[4] = 0; break;}
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
			Tmp0.range(255,0) = in0_0.read();
			Tmp1.range(255,0) = in1_0.read();
			Tmp2.range(255,0) = in2_0.read();
			Tmp3.range(255,0) = in3_0.read();
			Tmp4.range(255,0) = in4_0.read();

			Tmp0.range(511,256) = in0_1.read();
			Tmp1.range(511,256) = in1_1.read();
			Tmp2.range(511,256) = in2_1.read();
			Tmp3.range(511,256) = in3_1.read();
			Tmp4.range(511,256) = in4_1.read();
		}

		tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; tmp5=Tmp5; tmp6=Tmp6; tmp7=Tmp7;

		bool cond0 = (cond_y && ((i+adjust[0]) >= 0)  &&  ((i+adjust[0]) < end_index) );
		bool cond1 = (cond_y && ((i+adjust[1]) >= 0)  &&  ((i+adjust[1]) < end_index) );
		bool cond2 = (cond_y && ((i+adjust[2]) >= 0)  &&  ((i+adjust[2]) < end_index) );
		bool cond3 = (cond_y && ((i+adjust[3]) >= 0)  &&  ((i+adjust[3]) < end_index) );
		bool cond4 = (cond_y && ((i+adjust[4]) >= 0)  &&  ((i+adjust[4]) < end_index) );

		if(cond0){	out0_0 << tmp0.range(255,0); out0_1 << tmp0.range(511,256);}
		if(cond1){	out1_0 << tmp1.range(255,0); out1_1 << tmp1.range(511,256);}
		if(cond2){	out2_0 << tmp2.range(255,0); out2_1 << tmp2.range(511,256);}
		if(cond3){	out3_0 << tmp3.range(255,0); out3_1 << tmp3.range(511,256);}
		if(cond4){	out4_0 << tmp4.range(255,0); out4_1 << tmp4.range(511,256);}

		i++;

	}

}


static void stream_convert_512_1280(hls::stream<uint256_dt> &in0_0, hls::stream<uint256_dt> &in0_1,
		hls::stream<uint256_dt> &in1_0, hls::stream<uint256_dt> &in1_1,
		hls::stream<uint256_dt> &in2_0, hls::stream<uint256_dt> &in2_1,
		hls::stream<uint256_dt> &in3_0, hls::stream<uint256_dt> &in3_1,
		hls::stream<uint256_dt> &in4_0, hls::stream<uint256_dt> &in4_1,


		hls::stream<uint256_dt> &out0, hls::stream<uint256_dt> &out1,
		hls::stream<uint256_dt> &out2, hls::stream<uint256_dt> &out3,
		hls::stream<uint256_dt> &out4,
		struct data_G data_g){

	unsigned int total_itr = (data_g.totol_iter_64 >> 1); //total_itr0 * end_index;

	unsigned short offset_80 = (data_g.offset_x80);

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2


		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4;
		uint512_dt Tmp0, Tmp1, Tmp2, Tmp3, Tmp4;

		Tmp0.range(255,0) =  in0_0.read();
		Tmp0.range(511,256) = in0_1.read();

		Tmp1.range(255,0) = in1_0.read();
		Tmp1.range(511,256) =  in1_1.read();

		Tmp2.range(255,0) = in2_0.read();
		Tmp2.range(511,256) = in2_1.read();

		Tmp3.range(255,0) = in3_0.read();
		Tmp3.range(511,256) = in3_1.read();

		Tmp4.range(255,0) = in4_0.read();
		Tmp4.range(511,256) = in4_1.read();

		switch(offset_80){
			case 0:  { tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; break;}
			case 16: { tmp0 = Tmp1; tmp1 = Tmp2; tmp2=Tmp3; tmp3=Tmp4; tmp4=Tmp0; break;}
			case 32: { tmp0 = Tmp2; tmp1 = Tmp3; tmp2=Tmp4; tmp3=Tmp0; tmp4=Tmp1; break;}
			case 48: { tmp0 = Tmp3; tmp1 = Tmp4; tmp2=Tmp0; tmp3=Tmp1; tmp4=Tmp2; break;}
			case 64: { tmp0 = Tmp4; tmp1 = Tmp0; tmp2=Tmp1; tmp3=Tmp2; tmp4=Tmp3; break;}
			default :{ tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; break;}
		}

		out0 << tmp0.range(255,0);
		out1 << tmp0.range(511,256);

		out2 << tmp1.range(255,0);
		out3 << tmp1.range(511,256);

		out4 << tmp2.range(255,0);
		out0 << tmp2.range(511,256);

		out1 << tmp3.range(255,0);
		out2 << tmp3.range(511,256);

		out3 << tmp4.range(255,0);
		out4 << tmp4.range(511,256);
	}
}

static void stream_convert_1280_512(hls::stream<uint256_dt> &in0, hls::stream<uint256_dt> &in1,
		hls::stream<uint256_dt> &in2, hls::stream<uint256_dt> &in3,
		hls::stream<uint256_dt> &in4,

		hls::stream<uint256_dt> &out0_0, hls::stream<uint256_dt> &out0_1,
		hls::stream<uint256_dt> &out1_0, hls::stream<uint256_dt> &out1_1,
		hls::stream<uint256_dt> &out2_0, hls::stream<uint256_dt> &out2_1,
		hls::stream<uint256_dt> &out3_0, hls::stream<uint256_dt> &out3_1,
		hls::stream<uint256_dt> &out4_0, hls::stream<uint256_dt> &out4_1,
		struct data_G data_g){

	unsigned int total_itr = (data_g.totol_iter_64 >> 1); //register_it <unsigned int>(total_itr0 * end_index);

	unsigned short offset_80 = (data_g.offset_x80);

	for(unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS loop_tripcount min=1000 max=1500 avg=1200
		#pragma HLS PIPELINE II=2

		uint512_dt tmp0, tmp1, tmp2, tmp3, tmp4;
		uint512_dt Tmp0, Tmp1, Tmp2, Tmp3, Tmp4;

		Tmp0.range(255,0) = in0.read();
		Tmp0.range(511,256) = in1.read();

		Tmp1.range(255,0) = in2.read();
		Tmp1.range(511,256) = in3.read();

		Tmp2.range(255,0) = in4.read();
		Tmp2.range(511,256) = in0.read();

		Tmp3.range(255,0) = in1.read();
		Tmp3.range(511,256) = in2.read();

		Tmp4.range(255,0) = in3.read();
		Tmp4.range(511,256) = in4.read();


		switch(offset_80){
			case 0:  { tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; break;}
			case 16: { tmp0 = Tmp4; tmp1 = Tmp0; tmp2=Tmp1; tmp3=Tmp2; tmp4=Tmp3;  break;}
			case 32: { tmp0 = Tmp3; tmp1 = Tmp4; tmp2=Tmp0; tmp3=Tmp1; tmp4=Tmp2;  break;}
			case 48: { tmp0 = Tmp2; tmp1 = Tmp3; tmp2=Tmp4; tmp3=Tmp0; tmp4=Tmp1; break;}
			case 64: { tmp0 = Tmp1; tmp1 = Tmp2; tmp2=Tmp3; tmp3=Tmp4; tmp4=Tmp0; break;}
			default :{ tmp0 = Tmp0; tmp1 = Tmp1; tmp2=Tmp2; tmp3=Tmp3; tmp4=Tmp4; break;}
		}
		out0_0 << tmp0.range(255,0);
		out0_1 << tmp0.range(511,256);
		out1_0 << tmp1.range(255,0);
		out1_1 << tmp1.range(511,256);

		out2_0 << tmp2.range(255,0);
		out2_1 << tmp2.range(511,256);
		out3_0 << tmp3.range(255,0);
		out3_1 << tmp3.range(511,256);

		out4_0 << tmp4.range(255,0);
		out4_1 << tmp4.range(511,256);
	}
}


static void process_ReadWrite (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
								uint512_dt*  arg0_1, uint512_dt*  arg1_1,
								uint512_dt*  arg0_2, uint512_dt*  arg1_2,
								uint512_dt*  arg0_3, uint512_dt*  arg1_3,
								uint512_dt*  arg0_4, uint512_dt*  arg1_4,
								hls::stream <t_pkt_1024> &inl, hls::stream <t_pkt> &inu,
								hls::stream <t_pkt_1024> &outl, hls::stream <t_pkt> &outu,
							   const int xdim0, const unsigned short offset_x, const unsigned short tile_x,
							   const unsigned short offset_y, unsigned short tile_y,
							   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){


    static hls::stream<uint256_dt> streamArrayrd[8];
    static hls::stream<uint256_dt> streamArraywr[8];

    static hls::stream<unsigned int> rd_index[8];
    static hls::stream<unsigned int> wr_index[8];

    static hls::stream<uint256_dt> rd_bufferArr0[8];
    static hls::stream<uint256_dt> rd_bufferArr1[8];
    static hls::stream<uint256_dt> wr_bufferArr0[8];
    static hls::stream<uint256_dt> wr_bufferArr1[8];

    static hls::stream<uint256_dt> skip_bufferArrL[8];
    static hls::stream<uint256_dt> skip_bufferArrU[8];

    static hls::stream<uint256_dt> streamC_4_256_0[8];
    static hls::stream<uint256_dt> streamC_4_256_1[8];
    static hls::stream<uint256_dt> streamC_4_256_2[8];
    static hls::stream<uint256_dt> streamC_4_256_3[8];

    static hls::stream<uint1024_dt> streamArray_1024[4];

    #pragma HLS STREAM variable = streamArrayrd depth = 2
	#pragma HLS STREAM variable = streamArraywr depth = 2

	#pragma HLS STREAM variable = rd_index depth = 2
	#pragma HLS STREAM variable = wr_index depth = 2

	#pragma HLS STREAM variable = streamArray_1024 depth = 64

	#pragma HLS STREAM variable = rd_bufferArr0 depth = 2
	#pragma HLS STREAM variable = wr_bufferArr0 depth = 2
	#pragma HLS STREAM variable = rd_bufferArr1 depth = 2
	#pragma HLS STREAM variable = wr_bufferArr1 depth = 2


	#pragma HLS STREAM variable = skip_bufferArrL depth = 2
	#pragma HLS STREAM variable = skip_bufferArrU depth = 2

	#pragma HLS STREAM variable = streamC_4_256_0 depth = 2
	#pragma HLS STREAM variable = streamC_4_256_1 depth = 2
	#pragma HLS STREAM variable = streamC_4_256_2 depth = 2
	#pragma HLS STREAM variable = streamC_4_256_3 depth = 2




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

	data_g.offset_x5 = offset_x/5;
	data_g.tile_x5 = tile_x/5;

	unsigned char adjust_y = (data_g.offset_y != 0 ? 3 : 0);

	data_g.xblocks5 = data_g.xblocks / 5;
	data_g.y_offset_addr = (offset_y+adjust_y) * (data_g.xblocks5);
	data_g.offset_x80 = offset_x % 80;


	data_g.plane_size = data_g.xblocks * (data_g.grid_sizey);
	data_g.plane_size5 = data_g.xblocks5 * (data_g.grid_sizey);
	data_g.total_itr_R = data_g.grid_sizez * data_g.tile_y;
	data_g.total_itr_W = data_g.grid_sizez * (data_g.tile_y - adjust_y);

	unsigned int tile_plane_size = (data_g.tile_x5 >> (SHIFT_BITS)) * data_g.tile_y;
	unsigned int totol_iter = tile_plane_size * data_g.grid_sizez;
	data_g.totol_iter_64 = totol_iter;


	#pragma HLS dataflow


//
	read_to_fifo(arg0_0, rd_bufferArr0[0], rd_bufferArr1[0],
			arg0_1, rd_bufferArr0[1], rd_bufferArr1[1],
			0, data_g);

	read_to_fifo(arg0_2, rd_bufferArr0[2], rd_bufferArr1[2],
			arg0_3, rd_bufferArr0[3], rd_bufferArr1[3],
			2, data_g);

	read_to_fifo1(arg0_4, rd_bufferArr0[4], rd_bufferArr1[4],
			4, data_g);



	stream_convert_512_1280(rd_bufferArr0[0], rd_bufferArr1[0], rd_bufferArr0[1], rd_bufferArr1[1],
			rd_bufferArr0[2], rd_bufferArr1[2], rd_bufferArr0[3], rd_bufferArr1[3],
			rd_bufferArr0[4], rd_bufferArr1[4],
			streamC_4_256_0[0], streamC_4_256_0[1], streamC_4_256_0[2], streamC_4_256_0[3], streamC_4_256_0[4],
			data_g);

//	// going out
	fifo256_5_2axis(streamC_4_256_0[0], streamC_4_256_0[1], streamC_4_256_0[2], streamC_4_256_0[3], streamC_4_256_0[4],
			outl, outu, totol_iter);

	axis2_fifo256_5(inl, inu,
			streamC_4_256_1[0], streamC_4_256_1[1], streamC_4_256_1[2], streamC_4_256_1[3], streamC_4_256_1[4],
			totol_iter);

	// coming in

	stream_convert_1280_512(streamC_4_256_1[0], streamC_4_256_1[1], streamC_4_256_1[2], streamC_4_256_1[3], streamC_4_256_1[4],
			skip_bufferArrL[0], skip_bufferArrU[0], skip_bufferArrL[1], skip_bufferArrU[1],
			skip_bufferArrL[2], skip_bufferArrU[2], skip_bufferArrL[3], skip_bufferArrU[3],
			skip_bufferArrL[4], skip_bufferArrU[4],  data_g);

	skipAndInsert( skip_bufferArrL[0], skip_bufferArrU[0], wr_bufferArr0[0], wr_bufferArr1[0], skip_bufferArrL[1], skip_bufferArrU[1], wr_bufferArr0[1], wr_bufferArr1[1],
			 skip_bufferArrL[2], skip_bufferArrU[2], wr_bufferArr0[2],   wr_bufferArr1[2],  skip_bufferArrL[3], skip_bufferArrU[3], wr_bufferArr0[3], wr_bufferArr1[3],
			 skip_bufferArrL[4], skip_bufferArrU[4], wr_bufferArr0[4], wr_bufferArr1[4],
			data_g);



	write_from_fifo(arg1_0, wr_bufferArr0[0], wr_bufferArr1[0],
			arg1_1, wr_bufferArr0[1], wr_bufferArr1[1],
			0, data_g);

	write_from_fifo(arg1_2, wr_bufferArr0[2], wr_bufferArr1[2],
			arg1_3, wr_bufferArr0[3], wr_bufferArr1[3],
			2, data_g);

	write_from_fifo1(arg1_4, wr_bufferArr0[4], wr_bufferArr1[4],
			4, data_g);


}




static void process_ReadWrite_dataflow (uint512_dt*  arg0_0, uint512_dt*  arg1_0,
										uint512_dt*  arg0_1, uint512_dt*  arg1_1,
										uint512_dt*  arg0_2, uint512_dt*  arg1_2,
										uint512_dt*  arg0_3, uint512_dt*  arg1_3,
										uint512_dt*  arg0_4, uint512_dt*  arg1_4,
										hls::stream <t_pkt_1024> &inl, hls::stream <t_pkt> &inu,
										hls::stream <t_pkt_1024> &outl, hls::stream <t_pkt> &outu,
									   const int xdim0, unsigned int tile_memx[], unsigned char tilex_count ,
									   unsigned int tile_memy[], unsigned char tiley_count,
									   const unsigned short size_x, const unsigned short size_y, const unsigned short size_z){

	unsigned char toltal_itr = tilex_count*tiley_count;
	unsigned char i = 0, j = 0;
	for(unsigned char itr = 0; itr < toltal_itr; itr++){
		if(j == tilex_count){
			j = 0;
			i++;
		}

		unsigned short offset_x = tile_memx[j] & 0xffff;
		unsigned short tile_x   = tile_memx[j] >> 16;
		unsigned short offset_y = tile_memy[i] & 0xffff;
		unsigned short tile_y   = tile_memy[i] >> 16;

		j++;
		process_ReadWrite(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3, arg0_4, arg1_4,
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
		hls::stream <t_pkt> &inu,
		hls::stream <t_pkt_1024> &outl,
		hls::stream <t_pkt> &outu){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_0 offset = slave bundle = gmem0 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_0 offset = slave bundle = gmem0 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_1 offset = slave bundle = gmem1 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_1 offset = slave bundle = gmem1 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_2 offset = slave bundle = gmem2 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_2 offset = slave bundle = gmem2 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_3 offset = slave bundle = gmem3 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_3 offset = slave bundle = gmem3 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0_4 offset = slave bundle = gmem4 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1_4 offset = slave bundle = gmem4 max_read_burst_length=8 max_write_burst_length=8 latency=40 num_read_outstanding=32 num_write_outstanding=32

	#pragma HLS INTERFACE depth=4096 m_axi port = tile offset = slave bundle = gmem16

	#pragma HLS INTERFACE axis port = tile_s_out  register
	#pragma HLS INTERFACE axis port = inl  register
	#pragma HLS INTERFACE axis port = inu register
	#pragma HLS INTERFACE axis port = outl  register
	#pragma HLS INTERFACE axis port = outu register

	#pragma HLS INTERFACE s_axilite port = arg0_0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_0 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg0_1 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_1 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg0_2 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_2 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg0_3 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_3 bundle = control

	#pragma HLS INTERFACE s_axilite port = arg0_4 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1_4 bundle = control

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


	unsigned short count_s = count;
	for(unsigned short i =  0; i < count_s; i++){
		process_ReadWrite_dataflow(arg0_0, arg1_0, arg0_1, arg1_1, arg0_2, arg1_2, arg0_3, arg1_3, arg0_4, arg1_4,
				inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
		process_ReadWrite_dataflow(arg1_0, arg0_0, arg1_1, arg0_1, arg1_2, arg0_2, arg1_3, arg0_3, arg1_4, arg0_4,
				inl, inu, outl, outu, xdim0, tile_memx, tilex_count, tile_memy, tiley_count, sizex, sizey, sizez);
	}
}
}


