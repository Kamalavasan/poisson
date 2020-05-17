#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <stdio.h>


typedef ap_uint<512> uint512_dt;
typedef ap_uint<256> uint256_dt;
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

const int plane_buff_size = 80*80;
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
	unsigned int line_diff;
	unsigned short outer_loop_limit;
};


static void read_row(uint512_dt*  arg0, hls::stream<uint512_dt> &rd_buffer, const int gridsize_da){
	unsigned int itr_limit = (gridsize_da >> 1);
	for (int itr = 0; itr < itr_limit; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid_2 max=max_grid_2 avg=avg_grid_2
		rd_buffer << arg0[itr];
	}
}


static void stream_convert_512_256(hls::stream<uint512_dt> &in, hls::stream<uint256_dt> &out,  const int gridsize_da){
	unsigned int itr_limit = (gridsize_da >> 1);
	for (int itr = 0; itr < itr_limit; itr++){
		#pragma HLS PIPELINE II=2
		#pragma HLS loop_tripcount min=min_grid_2 max=max_grid_2 avg=avg_grid_2
		uint512_dt tmp = in.read();
		uint256_dt var_l = tmp.range(255,0);
		uint256_dt var_h = tmp.range(511,256);;
		out << var_l;
		out << var_h;
	}
}

static void stream_convert_256_512(hls::stream<uint256_dt> &in, hls::stream<uint512_dt> &out,const int gridsize_da){
	unsigned int itr_limit = (gridsize_da >> 1);
	for (int itr = 0; itr < itr_limit; itr++){
		#pragma HLS PIPELINE II=2
		#pragma HLS loop_tripcount min=min_grid_2 max=max_grid_2 avg=avg_grid_2
		uint512_dt tmp;
		tmp.range(255,0) = in.read();
		tmp.range(511,256) = in.read();
		out << tmp;
	}
}

static void process_a_grid( hls::stream<uint256_dt> &rd_buffer, hls::stream<uint256_dt> &wr_buffer, struct data_G data_g){
	unsigned short grid_sizex = data_g.grid_sizex;
	unsigned short sizex = data_g.sizex;
	unsigned short sizey = data_g.sizey;
	unsigned short sizez = data_g.sizez;
	unsigned short limit_z = data_g.limit_z;
	unsigned short grid_sizey = data_g.grid_sizey;
	unsigned short grid_sizez = data_g.grid_sizez;
	unsigned int line_diff = data_g.line_diff;
	unsigned int plane_diff = data_g.plane_diff;
	unsigned int plane_size = data_g.plane_size;
	unsigned int gridsize = data_g.gridsize_pr;

	// float s_4_4_8_arr[PORT_WIDTH], s_4_4_7_arr[PORT_WIDTH], s_4_4_6_arr[PORT_WIDTH], s_4_4_5_arr[PORT_WIDTH];
	// float s_4_8_4_arr[PORT_WIDTH], s_4_7_4_arr[PORT_WIDTH], s_4_6_4_arr[PORT_WIDTH], s_4_5_4_arr[PORT_WIDTH];
	// float s_8_4_4_arr[PORT_WIDTH], s_7_4_4_arr[PORT_WIDTH], s_6_4_4_arr[PORT_WIDTH], s_5_4_4_arr[PORT_WIDTH];
	// float s_4_4_4_arr[PORT_WIDTH];
	// float s_3_4_4_arr[PORT_WIDTH], s_2_4_4_arr[PORT_WIDTH], s_1_4_4_arr[PORT_WIDTH], s_0_4_4_arr[PORT_WIDTH];
	// float s_4_3_4_arr[PORT_WIDTH], s_4_2_4_arr[PORT_WIDTH], s_4_1_4_arr[PORT_WIDTH], s_4_0_4_arr[PORT_WIDTH];
	// float s_4_4_3_arr[PORT_WIDTH], s_4_4_2_arr[PORT_WIDTH], s_4_4_1_arr[PORT_WIDTH], s_4_4_0_arr[PORT_WIDTH];
	// float s_1_2_1_arr[PORT_WIDTH];
	// float s_1_1_1_arr[PORT_WIDTH];
	// float s_1_0_1_arr[PORT_WIDTH];
	// float s_1_1_0_arr[PORT_WIDTH];








	uint256_dt window_z_p_1[plane_buff_size];
	uint256_dt window_z_p_2[plane_buff_size];
	uint256_dt window_z_p_3[plane_buff_size];
	uint256_dt window_z_p_4[plane_buff_size];

	uint256_dt window_y_p_1[line_buff_size];
	uint256_dt window_y_p_2[line_buff_size];
	uint256_dt window_y_p_3[line_buff_size];
	uint256_dt window_y_p_4[line_buff_size];

	uint256_dt window_y_n_1[line_buff_size];
	uint256_dt window_y_n_2[line_buff_size];
	uint256_dt window_y_n_3[line_buff_size];
	uint256_dt window_y_n_4[line_buff_size];

	uint256_dt window_z_n_1[plane_buff_size];
	uint256_dt window_z_n_2[plane_buff_size];
	uint256_dt window_z_n_3[plane_buff_size];
	uint256_dt window_z_n_4[plane_buff_size];

	#pragma HLS RESOURCE variable=window_z_p_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_4 core=XPM_MEMORY uram latency=2

	#pragma HLS RESOURCE variable=window_y_p_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_p_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_p_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_p_4 core=XPM_MEMORY uram latency=2

	#pragma HLS RESOURCE variable=window_y_n_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_n_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_n_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_y_n_4 core=XPM_MEMORY uram latency=2

	#pragma HLS RESOURCE variable=window_z_n_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_4 core=XPM_MEMORY uram latency=2

	uint256_dt s_4_4_8, s_4_4_7, s_4_4_6, s_4_4_5;
	uint256_dt s_4_8_4, s_4_7_4, s_4_6_4, s_4_5_4;
	uint256_dt s_8_4_4, s_7_4_4, s_6_4_4, s_5_4_4;
	uint256_dt s_4_4_4;
	uint256_dt s_3_4_4, s_2_4_4, s_1_4_4, s_0_4_4;
	uint256_dt s_4_3_4, s_4_2_4, s_4_1_4, s_4_0_4;
	uint256_dt s_4_4_3, s_4_4_2, s_4_4_1, s_4_4_0;
	uint256_dt update_j;


	const float c[2*ORDER+1] = {0.0035714285714285713,-0.0380952380952381,0.2,-0.8,0.0,0.8,-0.2,0.0380952380952381,-0.0035714285714285713};
	const float invdx = 200; // 1/0.005
	const float invdy = 200; // 1/0.005
	const float invdz = 200; // 1/0.005
	const unsigned short half = 4;
	const unsigned short pml_width = 10;

	short xbeg=half;
	short xend=sizex-half;
	short ybeg=half;
	short yend=sizey-half;
	short zbeg=half;
	short zend=sizez-half;
	short xpmlbeg=xbeg+pml_width;
	short ypmlbeg=ybeg+pml_width;
	short zpmlbeg=zbeg+pml_width;
	short xpmlend=xend-pml_width;
	short ypmlend=yend-pml_width;
	short zpmlend=zend-pml_width;



	
	unsigned short i = 0, j = 0, k = 0;
	unsigned short j_p = 0, j_l = 0, j_p_diff = 0, j_l_diff = 0;
	for(unsigned int itr = 0; itr < gridsize; itr++) {
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=1

		if(k == grid_sizex){
			k = 0;
			j++;
		}

		if(j == grid_sizey){
			j = 0;
			i++;
		}

		// negetive z arm
		s_4_4_0 = window_z_n_4[j_p];

		s_4_4_1 = window_z_n_3[j_p];
		window_z_n_4[j_p] = s_4_4_1;

		s_4_4_2 = window_z_n_2[j_p];
		window_z_n_3[j_p] = s_4_4_2;

		s_4_4_3 = window_z_n_1[j_p_diff];
		window_z_n_2[j_p] = s_4_4_3;


		// positive y arm
		s_4_0_4 = window_y_n_4[j_l]; 
		window_z_n_1[j_p_diff] = s_4_0_4;

		s_4_1_4 = window_y_n_3[j_l];
		window_y_n_4[j_l] = s_4_1_4;

		s_4_2_4 = window_y_n_2[j_l];
		window_y_n_3[j_l] = s_4_2_4;

		s_4_3_4 = window_y_n_1[j_l_diff];
		window_y_n_2[j_l] = s_4_3_4;


		// negetive to positive x arm


		s_0_4_4 = s_1_4_4;
		window_y_n_1[j_l_diff] = s_0_4_4;

		s_1_4_4 = s_2_4_4;
		s_2_4_4 = s_3_4_4;
		s_3_4_4 = s_4_4_4;
		s_4_4_4 = s_5_4_4;
		s_5_4_4 = s_6_4_4;
		s_6_4_4 = s_7_4_4;
		s_7_4_4 = s_8_4_4;


		// positive Y arm 
		s_8_4_4 = window_y_p_1[j_l_diff];



		s_4_5_4 = window_y_p_2[j_l];
		window_y_p_1[j_l_diff] = s_4_5_4;

		s_4_6_4 = window_y_p_3[j_l];
		window_y_p_2[j_l] = s_4_6_4;

		s_4_7_4 = window_y_p_4[j_l];
		window_y_p_3[j_l] = s_4_7_4;



		// positive z arm
		s_4_8_4 = window_z_p_1[j_p_diff];
		window_y_p_4[j_l] = s_4_8_4;

		s_4_4_5 = window_z_p_2[j_p];
		window_z_p_1[j_p_diff] = s_4_4_5;

		s_4_4_6 = window_z_p_3[j_p];
		window_z_p_2[j_p] = s_4_4_6; 	

		s_4_4_7 = window_z_p_4[j_p];   //set
		window_z_p_3[j_p] = s_4_4_7;   //set	

		bool cond_tmp1 = (i < grid_sizez);
		if(cond_tmp1){
			s_4_4_8 = rd_buffer.read(); // set
		}
		window_z_p_4[j_p] = s_4_4_8; // set



		j_p++;
		if(j_p >= plane_size){
			j_p = 0;
		}

		j_l++;
		if(j_l >= grid_sizex){
			j_l = 0;
		}

		j_p_diff++;
		if(j_p_diff >= plane_diff){
			j_p_diff = 0;
		}

		j_l_diff++;
		if(j_l_diff >= line_diff){
			j_l_diff = 0;
		}



		// X ARM

		int i_X_ARM_0[2*ORDER+1] = {s_0_4_4.range(95,64), s_1_4_4.range(95,64), s_2_4_4.range(95,64), s_3_4_4.range(95,64), s_4_4_4.range(95,64), s_5_4_4.range(95,64), s_6_4_4.range(95,64), s_7_4_4.range(95,64), s_8_4_4.range(95,64)}; 

		int i_X_ARM_1[2*ORDER+1] = {s_0_4_4.range(127,96), s_1_4_4.range(127,96), s_2_4_4.range(127,96), s_3_4_4.range(127,96), s_4_4_4.range(127,96), s_5_4_4.range(127,96), s_6_4_4.range(127,96), s_7_4_4.range(127,96), s_8_4_4.range(127,96)}; 

		int i_X_ARM_2[2*ORDER+1] = {s_0_4_4.range(159,128), s_1_4_4.range(159,128), s_2_4_4.range(159,128), s_3_4_4.range(159,128), s_4_4_4.range(159,128), s_5_4_4.range(159,128), s_6_4_4.range(159,128), s_7_4_4.range(159,128), s_8_4_4.range(159,128)}; 

		int i_X_ARM_3[2*ORDER+1] = {s_0_4_4.range(191,160), s_1_4_4.range(191,160), s_2_4_4.range(191,160), s_3_4_4.range(191,160), s_4_4_4.range(191,160), s_5_4_4.range(191,160), s_6_4_4.range(191,160), s_7_4_4.range(191,160), s_8_4_4.range(191,160)}; 

		int i_X_ARM_4[2*ORDER+1] = {s_0_4_4.range(223,192), s_1_4_4.range(223,192), s_2_4_4.range(223,192), s_3_4_4.range(223,192), s_4_4_4.range(223,192), s_5_4_4.range(223,192), s_6_4_4.range(223,192), s_7_4_4.range(223,192), s_8_4_4.range(223,192)}; 

		int i_X_ARM_5[2*ORDER+1] = {s_0_4_4.range(255,224), s_1_4_4.range(255,224), s_2_4_4.range(255,224), s_3_4_4.range(255,224), s_4_4_4.range(255,224), s_5_4_4.range(255,224), s_6_4_4.range(255,224), s_7_4_4.range(255,224), s_8_4_4.range(255,224)};

		float X_ARM_0[2*ORDER+1], X_ARM_1[2*ORDER+1], X_ARM_2[2*ORDER+1], X_ARM_3[2*ORDER+1], X_ARM_4[2*ORDER+1], X_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_X_ARM_0[i];
			X_ARM_0[i] = tmp.f;

			tmp.i = i_X_ARM_1[i];
			X_ARM_1[i] = tmp.f;

			tmp.i = i_X_ARM_2[i];
			X_ARM_2[i] = tmp.f;

			tmp.i = i_X_ARM_3[i];
			X_ARM_3[i] = tmp.f;

			tmp.i = i_X_ARM_4[i];
			X_ARM_4[i] = tmp.f;

			tmp.i = i_X_ARM_5[i];
			X_ARM_5[i] = tmp.f;
		}

		// Y ARM

		int i_Y_ARM_0[2*ORDER+1] = {s_4_0_4.range(95,64), s_4_1_4.range(95,64), s_4_2_4.range(95,64), s_4_3_4.range(95,64), s_4_4_4.range(95,64), s_4_5_4.range(95,64), s_4_6_4.range(95,64), s_4_7_4.range(95,64), s_4_8_4.range(95,64)}; 

		int i_Y_ARM_1[2*ORDER+1] = {s_4_0_4.range(127,96), s_4_1_4.range(127,96), s_4_2_4.range(127,96), s_4_3_4.range(127,96), s_4_4_4.range(127,96), s_4_5_4.range(127,96), s_4_6_4.range(127,96), s_4_7_4.range(127,96), s_4_8_4.range(127,96)}; 

		int i_Y_ARM_2[2*ORDER+1] = {s_4_0_4.range(159,128), s_4_1_4.range(159,128), s_4_2_4.range(159,128), s_4_3_4.range(159,128), s_4_4_4.range(159,128), s_4_5_4.range(159,128), s_4_6_4.range(159,128), s_4_7_4.range(159,128), s_4_8_4.range(159,128)}; 

		int i_Y_ARM_3[2*ORDER+1] = {s_4_0_4.range(191,160), s_4_1_4.range(191,160), s_4_2_4.range(191,160), s_4_3_4.range(191,160), s_4_4_4.range(191,160), s_4_5_4.range(191,160), s_4_6_4.range(191,160), s_4_7_4.range(191,160), s_4_8_4.range(191,160)};

		int i_Y_ARM_4[2*ORDER+1] = {s_4_0_4.range(223,192), s_4_1_4.range(223,192), s_4_2_4.range(223,192), s_4_3_4.range(223,192), s_4_4_4.range(223,192), s_4_5_4.range(223,192), s_4_6_4.range(223,192), s_4_7_4.range(223,192), s_4_8_4.range(223,192)};

		int i_Y_ARM_5[2*ORDER+1] = {s_4_0_4.range(255,224), s_4_1_4.range(255,224), s_4_2_4.range(255,224), s_4_3_4.range(255,224), s_4_4_4.range(255,224), s_4_5_4.range(255,224), s_4_6_4.range(255,224), s_4_7_4.range(255,224), s_4_8_4.range(255,224)};

		float Y_ARM_0[2*ORDER+1], Y_ARM_1[2*ORDER+1], Y_ARM_2[2*ORDER+1], Y_ARM_3[2*ORDER+1], Y_ARM_4[2*ORDER+1], Y_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Y_ARM_0[i];
			Y_ARM_0[i] = tmp.f;

			tmp.i = i_Y_ARM_1[i];
			Y_ARM_1[i] = tmp.f;

			tmp.i = i_Y_ARM_2[i];
			Y_ARM_2[i] = tmp.f;

			tmp.i = i_Y_ARM_3[i];
			Y_ARM_3[i] = tmp.f;

			tmp.i = i_Y_ARM_4[i];
			Y_ARM_4[i] = tmp.f;

			tmp.i = i_Y_ARM_5[i];
			Y_ARM_5[i] = tmp.f;
		}

		// Z ARM

		int i_Z_ARM_0[2*ORDER+1] = {s_4_4_0.range(95,64), s_4_4_1.range(95,64), s_4_4_2.range(95,64), s_4_4_3.range(95,64), s_4_4_4.range(95,64), s_4_4_5.range(95,64), s_4_4_6.range(95,64), s_4_4_7.range(95,64), s_4_4_8.range(95,64)}; 

		int i_Z_ARM_1[2*ORDER+1] = {s_4_4_0.range(127,96), s_4_4_1.range(127,96), s_4_4_2.range(127,96), s_4_4_3.range(127,96), s_4_4_4.range(127,96), s_4_4_5.range(127,96), s_4_4_6.range(127,96), s_4_4_7.range(127,96), s_4_4_8.range(127,96)}; 

		int i_Z_ARM_2[2*ORDER+1] = {s_4_4_0.range(159,128), s_4_4_1.range(159,128), s_4_4_2.range(159,128), s_4_4_3.range(159,128), s_4_4_4.range(159,128), s_4_4_5.range(159,128), s_4_4_6.range(159,128), s_4_4_7.range(159,128), s_4_4_8.range(159,128)}; 

		int i_Z_ARM_3[2*ORDER+1] = {s_4_4_0.range(191,160), s_4_4_1.range(191,160), s_4_4_2.range(191,160), s_4_4_3.range(191,160), s_4_4_4.range(191,160), s_4_4_5.range(191,160), s_4_4_6.range(191,160), s_4_4_7.range(191,160), s_4_4_8.range(191,160)};

		int i_Z_ARM_4[2*ORDER+1] = {s_4_4_0.range(223,192), s_4_4_1.range(223,192), s_4_4_2.range(223,192), s_4_4_3.range(223,192), s_4_4_4.range(223,192), s_4_4_5.range(223,192), s_4_4_6.range(223,192), s_4_4_7.range(223,192), s_4_4_8.range(223,192)};

		int i_Z_ARM_5[2*ORDER+1] = {s_4_4_0.range(255,224), s_4_4_1.range(255,224), s_4_4_2.range(255,224), s_4_4_3.range(255,224), s_4_4_4.range(255,224), s_4_4_5.range(255,224), s_4_4_6.range(255,224), s_4_4_7.range(255,224), s_4_4_8.range(255,224)};

		float Z_ARM_0[2*ORDER+1], Z_ARM_1[2*ORDER+1], Z_ARM_2[2*ORDER+1], Z_ARM_3[2*ORDER+1], Z_ARM_4[2*ORDER+1], Z_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Z_ARM_0[i];
			Z_ARM_0[i] = tmp.f;

			tmp.i = i_Z_ARM_1[i];
			Z_ARM_1[i] = tmp.f;

			tmp.i = i_Z_ARM_2[i];
			Z_ARM_2[i] = tmp.f;

			tmp.i = i_Z_ARM_3[i];
			Z_ARM_3[i] = tmp.f;

			tmp.i = i_Z_ARM_4[i];
			Z_ARM_4[i] = tmp.f;

			tmp.i = i_Z_ARM_5[i];
			Z_ARM_5[i] = tmp.f;
		}

		float mem_wr[PORT_WIDTH];
		float s_4_4_4_arr[PORT_WIDTH];
		vec2s_4_4_4_arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.i = s_4_4_4.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_4_4_4_arr[k] = tmp.f;
		}





		#pragma HLS ARRAY_PARTITION variable=X_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Y_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Z_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=mem_wr complete dim=1
		#pragma HLS ARRAY_PARTITION variable=s_4_4_4_arr complete dim=1




	  	float sigma = 1 ; //mu[OPS_ACC5(0,0,0)]/rho[OPS_ACC4(0,0,0)];
	  	float sigmax=0.0;
	  	float sigmay=0.0;
	  	float sigmaz=0.0;

	  	short idx = k - ORDER;
	  	short idy = j - ORDER;
	  	short idz = i - ORDER;

	  	if(idx <= xbeg+pml_width){
	  	  sigmax = (xbeg+pml_width-idx ) * 0.1;//sigma/pml_width;
	  	}
	  	if(idx >=xend-pml_width){
	  	  sigmax=(idx -(xend-pml_width)) * 0.1; //sigma/pml_width;
	  	}

	  	if(idy <= ybeg+pml_width){
	  	  sigmay=(ybeg+pml_width-idy) * 0.1; //sigma/pml_width;
	  	}
	  	if(idy >= yend-pml_width){
	  	  sigmay=(idy-(yend-pml_width)) * 0.1; //sigma/pml_width;
	  	}

	  	if(idz <= zbeg+pml_width){
	  	  sigmaz=(zbeg+pml_width-idz) * 0.1; //sigma/pml_width;
	  	}
	  	if(idz >= zend-pml_width){
	  	  sigmaz=(idz -(zend-pml_width)) * 0.1; //sigma/pml_width;
	  	}


	  	float px = X_ARM_0[4];
	  	float py = X_ARM_1[4];
	  	float pz = X_ARM_2[4];

	  	float vx = X_ARM_3[4];
	  	float vy = X_ARM_4[4];
	  	float vz = X_ARM_5[4];


	  	float vxx=0.0;
	  	float vxy=0.0;
	  	float vxz=0.0;
	  	
	  	float vyx=0.0;
	  	float vyy=0.0;
	  	float vyz=0.0;
	
	  	float vzx=0.0;
	  	float vzy=0.0;
	  	float vzz=0.0;
	  	
	  	float pxx=0.0;
	  	float pxy=0.0;
	  	float pxz=0.0;
	  	
	  	float pyx=0.0;
	  	float pyy=0.0;
	  	float pyz=0.0;
	
	  	float pzx=0.0;
	  	float pzy=0.0;
	  	float pzz=0.0;

	  	for(int l=0;l <= ORDER*2; l++){
		    pxx += X_ARM_0[l] * c[l];
		    pyx += X_ARM_1[l] * c[l];
		    pzx += X_ARM_2[l] * c[l];
		    
		    vxx += X_ARM_3[l] * c[l];
		    vyx += X_ARM_4[l] * c[l];
		    vzx += X_ARM_5[l] * c[l];
		    
		    pxy += Y_ARM_0[l] * c[l];
		    pyy += Y_ARM_1[l] * c[l];
		    pzy += Y_ARM_2[l] * c[l];
		    
		    vxy += Y_ARM_0[l] * c[l];
		    vyy += Y_ARM_1[l] * c[l];
		    vzy += Y_ARM_2[l] * c[l];
		    
		    pxz += Z_ARM_0[l] * c[l];
		    pyz += Z_ARM_1[l] * c[l];
		    pzz += Z_ARM_2[l] * c[l];
		    
		    vxz += Z_ARM_0[l] * c[l];
		    vyz += Z_ARM_1[l] * c[l];
		    vzz += Z_ARM_2[l] * c[l];
		}

	  	pxx *= invdx;
	  	pyx *= invdx;
		pzx *= invdx;

		vxx *= invdx;
		vyx *= invdx;
		vzx *= invdx;

		pxy *= invdy;
		pyy *= invdy;
		pzy *= invdy;

		vxy *= invdy;
		vyy *= invdy;
		vzy *= invdy;

		pxz *= invdz;
		pyz *= invdz;
		pzz *= invdz;

		vxz *= invdz;
		vyz *= invdz;
		vzz *= invdz;

  		mem_wr[2]= vxx - sigmax*px;            //vxx/rho[OPS_ACC4(0,0,0)] - sigmax*px;
  		mem_wr[5]= (pxx+pyx+pxz) - sigmax*vx;  //(pxx+pyx+pxz)*mu[OPS_ACC5(0,0,0)] - sigmax*vx;
  		
  		mem_wr[3]= vyy - sigmay*py;  		  // vyy/rho[OPS_ACC4(0,0,0)] - sigmay*py;
  		mem_wr[6]= (pxy+pyy+pyz)- sigmay*vy;   //(pxy+pyy+pyz)*mu[OPS_ACC5(0,0,0)] - sigmay*vy;
  		
  		mem_wr[4]= vzz  - sigmaz*pz;  		  //vzz/rho[OPS_ACC4(0,0,0)] - sigmaz*pz;
  		mem_wr[7]= (pxz+pyz+pzz) - sigmaz*vz;  //(pxz+pyz+pzz)*mu[OPS_ACC5(0,0,0)] - sigmaz*vz;

  		mem_wr[0] = s_4_4_4_arr[6];
  		mem_wr[1] = s_4_4_4_arr[7];

		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = (idx < 0 || idx >= sizex || (idy < 0) || (idy >= sizey ) || (idz < ORDER) || (idz >= grid_sizez -ORDER));
//			tmp.f =  s_4_4_4_arr[k] ;
			tmp.f = change_cond ? s_4_4_4_arr[k] : mem_wr[k];
			update_j.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		bool cond_wr = (i >= ORDER) && ( i <= limit_z);
		if(cond_wr ) {
			wr_buffer <<  update_j;
		}

		// move the cell block
		k++;
	}
}

static void write_row( uint512_dt*  arg1, hls::stream<uint512_dt> &wr_buffer, const int gridsize_da){
	unsigned int itr_limit = (gridsize_da >> 1);
	for (int itr = 0; itr < itr_limit; itr++){
		#pragma HLS loop_tripcount min=min_grid_2 max=max_grid_2 avg=avg_grid_2
		#pragma HLS PIPELINE II=1
		arg1[itr] =  wr_buffer.read();
	}
}


void process_SLR0 (uint512_dt*  arg0, uint512_dt*  arg1,
		const int sizex, const int sizey, const int sizez, const int xdim_aigned){


    static hls::stream<uint256_dt> streamArray[40 + 1];
    static hls::stream<uint512_dt> rd_buffer;
    static hls::stream<uint512_dt> wr_buffer;

    #pragma HLS STREAM variable = streamArray depth = 2
	#pragma HLS STREAM variable = rd_buffer depth = max_depth_8
	#pragma HLS STREAM variable = wr_buffer depth = max_depth_8

    struct data_G data_g;
    data_g.sizex = sizex;
    data_g.sizey = sizey;
    data_g.sizez = sizez;
	data_g.grid_sizex = sizex+2*ORDER;
	data_g.grid_sizey = sizey+2*ORDER;
	data_g.grid_sizez = sizez+2*ORDER;
	data_g.limit_z = sizez+3*ORDER;


	unsigned short grid_sizey_4 = (data_g.grid_sizey - 4);
	data_g.plane_size = data_g.grid_sizex * data_g.grid_sizey;

	data_g.plane_diff = data_g.grid_sizex * grid_sizey_4;
	data_g.line_diff = data_g.grid_sizex - 4;
	data_g.gridsize_pr = data_g.plane_size * (data_g.limit_z);

	unsigned int gridsize_da = data_g.plane_size * (data_g.grid_sizez);


	#pragma HLS dataflow
	read_row(arg0, rd_buffer, gridsize_da);
	stream_convert_512_256(rd_buffer, streamArray[0], gridsize_da);

	process_a_grid( streamArray[0], streamArray[1], data_g);


	stream_convert_256_512(streamArray[1], wr_buffer, gridsize_da);
	write_row(arg1, wr_buffer, gridsize_da);

}


//-DHOST_CODE_OPT -DLOCAL_BUF_OPT -DDF_OPT -DFP_OPT
//--sc stencil_SLR0_1.out:stencil_SLR1_1.in
//--sc stencil_SLR1_1.out:stencil_SLR0_1.in

extern "C" {
void stencil_SLR0(
		uint512_dt*  arg0,
		uint512_dt*  arg1,
		const int sizex,
		const int sizey,
		const int sizez,
		const int xdim_aligned,
		const int count){

	#pragma HLS INTERFACE depth=4096 m_axi port = arg0 offset = slave bundle = gmem0 max_read_burst_length=256 max_write_burst_length=256
	#pragma HLS INTERFACE depth=4096 m_axi port = arg1 offset = slave bundle = gmem0
	#pragma HLS INTERFACE s_axilite port = arg0 bundle = control
	#pragma HLS INTERFACE s_axilite port = arg1 bundle = control
	#pragma HLS INTERFACE s_axilite port = sizex bundle = control
	#pragma HLS INTERFACE s_axilite port = sizey bundle = control
	#pragma HLS INTERFACE s_axilite port = sizez bundle = control
	#pragma HLS INTERFACE s_axilite port = xdim_aligned bundle = control
	#pragma HLS INTERFACE s_axilite port = count bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control


	for(int i =  0; i < count; i++){
	#pragma HLS loop_tripcount min=10 max=1000 avg=1000
		process_SLR0(arg0, arg1, sizex, sizey, sizez, xdim_aligned);
		// process_SLR0(arg1, arg0, sizex, sizey, sizez, xdim_aligned);
	}

}
}
