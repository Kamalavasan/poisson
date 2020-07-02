static void derives_calc_ytep_k1( hls::stream<uint256_dt> &rd_buffer0, hls::stream<uint256_dt> &wr_buffer0,
								  hls::stream<uint256_dt> &rd_buffer1, hls::stream<uint256_dt> &wr_buffer1,
								  hls::stream<uint256_dt> &rd_buffer2, hls::stream<uint256_dt> &wr_buffer2,
								  hls::stream<uint256_dt> &yy, hls::stream<uint256_dt> &yy_final, struct data_G data_g){
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








	uint768_dt window_z_p_1[plane_buff_size];
	uint768_dt window_z_p_2[plane_buff_size];
	uint768_dt window_z_p_3[plane_buff_size];
	uint768_dt window_z_p_4[plane_buff_size];

	uint768_dt window_y_p_1[line_buff_size];
	uint768_dt window_y_p_2[line_buff_size];
	uint768_dt window_y_p_3[line_buff_size];
	uint768_dt window_y_p_4[line_buff_size];

	uint576_dt window_y_n_1[line_buff_size];
	uint576_dt window_y_n_2[line_buff_size];
	uint576_dt window_y_n_3[line_buff_size];
	uint576_dt window_y_n_4[line_buff_size];

	uint576_dt window_z_n_1[plane_buff_size];
	uint576_dt window_z_n_2[plane_buff_size];
	uint576_dt window_z_n_3[plane_buff_size];
	uint576_dt window_z_n_4[plane_buff_size];

	#pragma HLS RESOURCE variable=window_z_p_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_p_4 core=XPM_MEMORY uram latency=2

	#pragma HLS RESOURCE variable=window_y_p_1 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_p_2 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_p_3 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_p_4 core=RAM_1P_BRAM  latency=2

	#pragma HLS RESOURCE variable=window_y_n_1 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_n_2 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_n_3 core=RAM_1P_BRAM  latency=2
	#pragma HLS RESOURCE variable=window_y_n_4 core=RAM_1P_BRAM  latency=2

	#pragma HLS RESOURCE variable=window_z_n_1 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_2 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_3 core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_z_n_4 core=XPM_MEMORY uram latency=2

	uint768_dt s_4_4_8, s_4_4_7, s_4_4_6, s_4_4_5;
	uint768_dt s_4_8_4, s_4_7_4, s_4_6_4, s_4_5_4;
	uint768_dt s_4_4_4_f2, s_4_4_4_f1, s_4_4_4, s_4_4_4_b1, s_4_4_4_b2;
	uint576_dt s_4_3_4, s_4_2_4, s_4_1_4, s_4_0_4;
	uint576_dt s_4_4_3, s_4_4_2, s_4_4_1, s_4_4_0;


	uint256_dt update0,update1,update2;

	uint768_dt yy_final_vec;


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
	unsigned short i_dum = 0, j_dum = 0, k_dum = 0;
	unsigned short j_p_dum = 0, j_l_dum = 0, j_p_diff_dum = 0, j_l_diff_dum = 0;
	unsigned short j_p, j_l, j_p_diff, j_l_diff;
	for(unsigned int itr = 0; itr < gridsize; itr++) {
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=1

		j_p = j_p_dum;
		j_l = j_l_dum;
		j_p_diff = j_p_diff_dum;
		j_l_diff = j_l_diff_dum;

		bool cmp1 = (k == grid_sizex -1);
		bool cmp2 = cmp1 && (j == grid_sizey-1);
		bool cmp3 = cmp2 && (i == limit_z-1);

		if(cmp1){
			k_dum = 0;
		}

		if(cmp2){
			j_dum = 0;
		} else if(cmp1){
			j_dum = j + 1;
		}

		if(cmp3){
			i_dum = 0;
		} else if(cmp2){
			i_dum = i + 1;
		}
//		if(k_dum == grid_sizex){
//			k_dum = 0;
//			j_dum = j + 1;
//		}
//
//		if(j_dum == grid_sizey){
//			j_dum = 0;
//			i_dum = i + 1;
//		}
//
//		if(i_dum == limit_z){
//			k_dum = 0;
//			j_dum = 0;
//			i_dum = 0;
//
//		}

		i = i_dum;
		j = j_dum;
		k = k_dum;

		// negetive z arm
		s_4_4_0 = window_z_n_4[j_p];

		s_4_4_1 = window_z_n_3[j_p];
		window_z_n_4[j_p] = s_4_4_1;

		s_4_4_2 = window_z_n_2[j_p];
		window_z_n_3[j_p] = s_4_4_2;

		s_4_4_3 = window_z_n_1[j_p_diff];
		window_z_n_2[j_p] = s_4_4_3;


		// Negetive y arm
		s_4_0_4 = window_y_n_4[j_l];
		window_z_n_1[j_p_diff] = s_4_0_4;

		s_4_1_4 = window_y_n_3[j_l];
		window_y_n_4[j_l] = s_4_1_4;

		s_4_2_4 = window_y_n_2[j_l];
		window_y_n_3[j_l] = s_4_2_4;

		s_4_3_4 = window_y_n_1[j_l_diff];
		window_y_n_2[j_l] = s_4_3_4;


		// negetive to positive x arm


		s_4_4_4_b2 = s_4_4_4_b1;
		window_y_n_1[j_l_diff] = {s_4_4_4_b2.range(255,64)  | s_4_4_4_b2.range(511,320)<< 256 | s_4_4_4_b2.range(767,576) << 512};


		s_4_4_4_b1 = s_4_4_4;
		s_4_4_4 = s_4_4_4_f1;
		s_4_4_4_f1 = s_4_4_4_f2;


		// positive Y arm 
		s_4_4_4_f2 = window_y_p_1[j_l_diff];



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
			s_4_4_8.range(255,0)   = rd_buffer0.read(); // set
			s_4_4_8.range(511,256) = rd_buffer1.read();
			s_4_4_8.range(767,512) = rd_buffer2.read();
		}
		window_z_p_4[j_p] = s_4_4_8; // set



		j_p_dum++;
		if(j_p_dum >= plane_size){
			j_p_dum = 0;
		}

		j_l_dum++;
		if(j_l_dum >= grid_sizex){
			j_l_dum = 0;
		}

		j_p_diff_dum++;
		if(j_p_diff_dum >= plane_diff){
			j_p_diff_dum = 0;
		}

		j_l_diff_dum++;
		if(j_l_diff_dum >= line_diff){
			j_l_diff_dum = 0;
		}


		#define X_0(a,c) (a+32*(c+1) -1, a+32*c)
		// X0 ARM
		int i_X0_ARM_0[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,0)), s_4_4_4_b1.range(X_0(64,0)), s_4_4_4_b1.range(X_0(320,0)), s_4_4_4_b1.range(X_0(576,0)),s_4_4_4.range(X_0(64,0)), s_4_4_4.range(X_0(320,0)), s_4_4_4.range(X_0(576,0)), s_4_4_4_f1.range(X_0(64,0)), s_4_4_4_f1.range(X_0(320,0))};

		int i_X0_ARM_1[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,1)), s_4_4_4_b1.range(X_0(64,1)), s_4_4_4_b1.range(X_0(320,1)), s_4_4_4_b1.range(X_0(576,1)),s_4_4_4.range(X_0(64,1)), s_4_4_4.range(X_0(320,1)), s_4_4_4.range(X_0(576,1)), s_4_4_4_f1.range(X_0(64,1)), s_4_4_4_f1.range(X_0(320,1))};

		int i_X0_ARM_2[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,2)), s_4_4_4_b1.range(X_0(64,2)), s_4_4_4_b1.range(X_0(320,2)), s_4_4_4_b1.range(X_0(576,2)),s_4_4_4.range(X_0(64,2)), s_4_4_4.range(X_0(320,2)), s_4_4_4.range(X_0(576,2)), s_4_4_4_f1.range(X_0(64,2)), s_4_4_4_f1.range(X_0(320,2))};

		int i_X0_ARM_3[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,3)), s_4_4_4_b1.range(X_0(64,3)), s_4_4_4_b1.range(X_0(320,3)), s_4_4_4_b1.range(X_0(576,3)),s_4_4_4.range(X_0(64,3)), s_4_4_4.range(X_0(320,3)), s_4_4_4.range(X_0(576,3)), s_4_4_4_f1.range(X_0(64,3)), s_4_4_4_f1.range(X_0(320,3))};

		int i_X0_ARM_4[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,4)), s_4_4_4_b1.range(X_0(64,4)), s_4_4_4_b1.range(X_0(320,4)), s_4_4_4_b1.range(X_0(576,4)),s_4_4_4.range(X_0(64,4)), s_4_4_4.range(X_0(320,4)), s_4_4_4.range(X_0(576,4)), s_4_4_4_f1.range(X_0(64,4)), s_4_4_4_f1.range(X_0(320,4))};

		int i_X0_ARM_5[(2*ORDER+1)] = {s_4_4_4_b2.range(X_0(576,5)), s_4_4_4_b1.range(X_0(64,5)), s_4_4_4_b1.range(X_0(320,5)), s_4_4_4_b1.range(X_0(576,5)),s_4_4_4.range(X_0(64,5)), s_4_4_4.range(X_0(320,5)), s_4_4_4.range(X_0(576,5)), s_4_4_4_f1.range(X_0(64,5)), s_4_4_4_f1.range(X_0(320,5))};

		float X0_ARM_0[2*ORDER+1], X0_ARM_1[2*ORDER+1], X0_ARM_2[2*ORDER+1], X0_ARM_3[2*ORDER+1], X0_ARM_4[2*ORDER+1], X0_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_X0_ARM_0[i];
			X0_ARM_0[i] = tmp.f;

			tmp.i = i_X0_ARM_1[i];
			X0_ARM_1[i] = tmp.f;

			tmp.i = i_X0_ARM_2[i];
			X0_ARM_2[i] = tmp.f;

			tmp.i = i_X0_ARM_3[i];
			X0_ARM_3[i] = tmp.f;

			tmp.i = i_X0_ARM_4[i];
			X0_ARM_4[i] = tmp.f;

			tmp.i = i_X0_ARM_5[i];
			X0_ARM_5[i] = tmp.f;
		}

		// X1_ARM
		int i_X1_ARM_0[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,0)), s_4_4_4_b1.range(X_0(320,0)), s_4_4_4_b1.range(X_0(576,0)),s_4_4_4.range(X_0(64,0)), s_4_4_4.range(X_0(320,0)), s_4_4_4.range(X_0(576,0)), s_4_4_4_f1.range(X_0(64,0)), s_4_4_4_f1.range(X_0(320,0)),s_4_4_4_f1.range(X_0(576,0))};

		int i_X1_ARM_1[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,1)), s_4_4_4_b1.range(X_0(320,1)), s_4_4_4_b1.range(X_0(576,1)),s_4_4_4.range(X_0(64,1)), s_4_4_4.range(X_0(320,1)), s_4_4_4.range(X_0(576,1)), s_4_4_4_f1.range(X_0(64,1)), s_4_4_4_f1.range(X_0(320,1)),s_4_4_4_f1.range(X_0(576,1))};

		int i_X1_ARM_2[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,2)), s_4_4_4_b1.range(X_0(320,2)), s_4_4_4_b1.range(X_0(576,2)),s_4_4_4.range(X_0(64,2)), s_4_4_4.range(X_0(320,2)), s_4_4_4.range(X_0(576,2)), s_4_4_4_f1.range(X_0(64,2)), s_4_4_4_f1.range(X_0(320,2)),s_4_4_4_f1.range(X_0(576,2))};

		int i_X1_ARM_3[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,3)), s_4_4_4_b1.range(X_0(320,3)), s_4_4_4_b1.range(X_0(576,3)),s_4_4_4.range(X_0(64,3)), s_4_4_4.range(X_0(320,3)), s_4_4_4.range(X_0(576,3)), s_4_4_4_f1.range(X_0(64,3)), s_4_4_4_f1.range(X_0(320,3)),s_4_4_4_f1.range(X_0(576,3))};

		int i_X1_ARM_4[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,4)), s_4_4_4_b1.range(X_0(320,4)), s_4_4_4_b1.range(X_0(576,4)),s_4_4_4.range(X_0(64,4)), s_4_4_4.range(X_0(320,4)), s_4_4_4.range(X_0(576,4)), s_4_4_4_f1.range(X_0(64,4)), s_4_4_4_f1.range(X_0(320,4)),s_4_4_4_f1.range(X_0(576,4))};

		int i_X1_ARM_5[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(64,5)), s_4_4_4_b1.range(X_0(320,5)), s_4_4_4_b1.range(X_0(576,5)),s_4_4_4.range(X_0(64,5)), s_4_4_4.range(X_0(320,5)), s_4_4_4.range(X_0(576,5)), s_4_4_4_f1.range(X_0(64,5)), s_4_4_4_f1.range(X_0(320,5)),s_4_4_4_f1.range(X_0(576,5))};

		float X1_ARM_0[2*ORDER+1], X1_ARM_1[2*ORDER+1], X1_ARM_2[2*ORDER+1], X1_ARM_3[2*ORDER+1], X1_ARM_4[2*ORDER+1], X1_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_X1_ARM_0[i];
			X1_ARM_0[i] = tmp.f;

			tmp.i = i_X1_ARM_1[i];
			X1_ARM_1[i] = tmp.f;

			tmp.i = i_X1_ARM_2[i];
			X1_ARM_2[i] = tmp.f;

			tmp.i = i_X1_ARM_3[i];
			X1_ARM_3[i] = tmp.f;

			tmp.i = i_X1_ARM_4[i];
			X1_ARM_4[i] = tmp.f;

			tmp.i = i_X1_ARM_5[i];
			X1_ARM_5[i] = tmp.f;
		}

		// X2_ARM
		int i_X2_ARM_0[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,0)), s_4_4_4_b1.range(X_0(576,0)),s_4_4_4.range(X_0(64,0)), s_4_4_4.range(X_0(320,0)), s_4_4_4.range(X_0(576,0)), s_4_4_4_f1.range(X_0(64,0)), s_4_4_4_f1.range(X_0(320,0)),s_4_4_4_f1.range(X_0(576,0)), s_4_4_4_f2.range(X_0(64,0))};

		int i_X2_ARM_1[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,1)), s_4_4_4_b1.range(X_0(576,1)),s_4_4_4.range(X_0(64,1)), s_4_4_4.range(X_0(320,1)), s_4_4_4.range(X_0(576,1)), s_4_4_4_f1.range(X_0(64,1)), s_4_4_4_f1.range(X_0(320,1)),s_4_4_4_f1.range(X_0(576,1)), s_4_4_4_f2.range(X_0(64,0))};

		int i_X2_ARM_2[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,2)), s_4_4_4_b1.range(X_0(576,2)),s_4_4_4.range(X_0(64,2)), s_4_4_4.range(X_0(320,2)), s_4_4_4.range(X_0(576,2)), s_4_4_4_f1.range(X_0(64,2)), s_4_4_4_f1.range(X_0(320,2)),s_4_4_4_f1.range(X_0(576,2)), s_4_4_4_f2.range(X_0(64,0))};

		int i_X2_ARM_3[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,3)), s_4_4_4_b1.range(X_0(576,3)),s_4_4_4.range(X_0(64,3)), s_4_4_4.range(X_0(320,3)), s_4_4_4.range(X_0(576,3)), s_4_4_4_f1.range(X_0(64,3)), s_4_4_4_f1.range(X_0(320,3)),s_4_4_4_f1.range(X_0(576,3)), s_4_4_4_f2.range(X_0(64,0))};

		int i_X2_ARM_4[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,4)), s_4_4_4_b1.range(X_0(576,4)),s_4_4_4.range(X_0(64,4)), s_4_4_4.range(X_0(320,4)), s_4_4_4.range(X_0(576,4)), s_4_4_4_f1.range(X_0(64,4)), s_4_4_4_f1.range(X_0(320,4)),s_4_4_4_f1.range(X_0(576,4)), s_4_4_4_f2.range(X_0(64,0))};

		int i_X2_ARM_5[(2*ORDER+1)] = {s_4_4_4_b1.range(X_0(320,5)), s_4_4_4_b1.range(X_0(576,5)),s_4_4_4.range(X_0(64,5)), s_4_4_4.range(X_0(320,5)), s_4_4_4.range(X_0(576,5)), s_4_4_4_f1.range(X_0(64,5)), s_4_4_4_f1.range(X_0(320,5)),s_4_4_4_f1.range(X_0(576,5)), s_4_4_4_f2.range(X_0(64,0))};

		float X2_ARM_0[2*ORDER+1], X2_ARM_1[2*ORDER+1], X2_ARM_2[2*ORDER+1], X2_ARM_3[2*ORDER+1], X2_ARM_4[2*ORDER+1], X2_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_X2_ARM_0[i];
			X2_ARM_0[i] = tmp.f;

			tmp.i = i_X2_ARM_1[i];
			X2_ARM_1[i] = tmp.f;

			tmp.i = i_X2_ARM_2[i];
			X2_ARM_2[i] = tmp.f;

			tmp.i = i_X2_ARM_3[i];
			X2_ARM_3[i] = tmp.f;

			tmp.i = i_X2_ARM_4[i];
			X2_ARM_4[i] = tmp.f;

			tmp.i = i_X2_ARM_5[i];
			X2_ARM_5[i] = tmp.f;
		}
		#undef X_0

		
		#define Y_0(a,c) (a+32*(c+1) -1, a+32*c)
		// Y0 ARM
		int i_Y0_ARM_0[2*ORDER+1] = {s_4_0_4.range(Y_0(0,0)), s_4_1_4.range(Y_0(0,0)), s_4_2_4.range(Y_0(0,0)), s_4_3_4.range(Y_0(0,0)), s_4_4_4.range(Y_0(64,0)), s_4_5_4.range(Y_0(64,0)), s_4_6_4.range(Y_0(64,0)), s_4_7_4.range(Y_0(64,0)), s_4_8_4.range(Y_0(64,0))}; 

		int i_Y0_ARM_1[2*ORDER+1] = {s_4_0_4.range(Y_0(0,1)), s_4_1_4.range(Y_0(0,1)), s_4_2_4.range(Y_0(0,1)), s_4_3_4.range(Y_0(0,1)), s_4_4_4.range(Y_0(64,1)), s_4_5_4.range(Y_0(64,1)), s_4_6_4.range(Y_0(64,1)), s_4_7_4.range(Y_0(64,1)), s_4_8_4.range(Y_0(64,1))}; 

		int i_Y0_ARM_2[2*ORDER+1] = {s_4_0_4.range(Y_0(0,2)), s_4_1_4.range(Y_0(0,2)), s_4_2_4.range(Y_0(0,2)), s_4_3_4.range(Y_0(0,2)), s_4_4_4.range(Y_0(64,2)), s_4_5_4.range(Y_0(64,2)), s_4_6_4.range(Y_0(64,2)), s_4_7_4.range(Y_0(64,2)), s_4_8_4.range(Y_0(64,2))}; 

		int i_Y0_ARM_3[2*ORDER+1] = {s_4_0_4.range(Y_0(0,3)), s_4_1_4.range(Y_0(0,3)), s_4_2_4.range(Y_0(0,3)), s_4_3_4.range(Y_0(0,3)), s_4_4_4.range(Y_0(64,3)), s_4_5_4.range(Y_0(64,3)), s_4_6_4.range(Y_0(64,3)), s_4_7_4.range(Y_0(64,3)), s_4_8_4.range(Y_0(64,3))};

		int i_Y0_ARM_4[2*ORDER+1] = {s_4_0_4.range(Y_0(0,4)), s_4_1_4.range(Y_0(0,4)), s_4_2_4.range(Y_0(0,4)), s_4_3_4.range(Y_0(0,4)), s_4_4_4.range(Y_0(64,4)), s_4_5_4.range(Y_0(64,4)), s_4_6_4.range(Y_0(64,4)), s_4_7_4.range(Y_0(64,4)), s_4_8_4.range(Y_0(64,4))};

		int i_Y0_ARM_5[2*ORDER+1] = {s_4_0_4.range(Y_0(0,5)), s_4_1_4.range(Y_0(0,5)), s_4_2_4.range(Y_0(0,5)), s_4_3_4.range(Y_0(0,5)), s_4_4_4.range(Y_0(64,5)), s_4_5_4.range(Y_0(64,5)), s_4_6_4.range(Y_0(64,5)), s_4_7_4.range(Y_0(64,5)), s_4_8_4.range(Y_0(64,5))};

		float Y0_ARM_0[2*ORDER+1], Y0_ARM_1[2*ORDER+1], Y0_ARM_2[2*ORDER+1], Y0_ARM_3[2*ORDER+1], Y0_ARM_4[2*ORDER+1], Y0_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Y0_ARM_0[i];
			Y0_ARM_0[i] = tmp.f;

			tmp.i = i_Y0_ARM_1[i];
			Y0_ARM_1[i] = tmp.f;

			tmp.i = i_Y0_ARM_2[i];
			Y0_ARM_2[i] = tmp.f;

			tmp.i = i_Y0_ARM_3[i];
			Y0_ARM_3[i] = tmp.f;

			tmp.i = i_Y0_ARM_4[i];
			Y0_ARM_4[i] = tmp.f;

			tmp.i = i_Y0_ARM_5[i];
			Y0_ARM_5[i] = tmp.f;
		}

		// Y1 ARM
		int i_Y1_ARM_0[2*ORDER+1] = {s_4_0_4.range(Y_0(192,0)), s_4_1_4.range(Y_0(192,0)), s_4_2_4.range(Y_0(192,0)), s_4_3_4.range(Y_0(192,0)), s_4_4_4.range(Y_0(320,0)), s_4_5_4.range(Y_0(320,0)), s_4_6_4.range(Y_0(320,0)), s_4_7_4.range(Y_0(320,0)), s_4_8_4.range(Y_0(320,0))}; 

		int i_Y1_ARM_1[2*ORDER+1] = {s_4_0_4.range(Y_0(192,1)), s_4_1_4.range(Y_0(192,1)), s_4_2_4.range(Y_0(192,1)), s_4_3_4.range(Y_0(192,1)), s_4_4_4.range(Y_0(320,1)), s_4_5_4.range(Y_0(320,1)), s_4_6_4.range(Y_0(320,1)), s_4_7_4.range(Y_0(320,1)), s_4_8_4.range(Y_0(320,1))}; 

		int i_Y1_ARM_2[2*ORDER+1] = {s_4_0_4.range(Y_0(192,2)), s_4_1_4.range(Y_0(192,2)), s_4_2_4.range(Y_0(192,2)), s_4_3_4.range(Y_0(192,2)), s_4_4_4.range(Y_0(320,2)), s_4_5_4.range(Y_0(320,2)), s_4_6_4.range(Y_0(320,2)), s_4_7_4.range(Y_0(320,2)), s_4_8_4.range(Y_0(320,2))}; 

		int i_Y1_ARM_3[2*ORDER+1] = {s_4_0_4.range(Y_0(192,3)), s_4_1_4.range(Y_0(192,3)), s_4_2_4.range(Y_0(192,3)), s_4_3_4.range(Y_0(192,3)), s_4_4_4.range(Y_0(320,3)), s_4_5_4.range(Y_0(320,3)), s_4_6_4.range(Y_0(320,3)), s_4_7_4.range(Y_0(320,3)), s_4_8_4.range(Y_0(320,3))};

		int i_Y1_ARM_4[2*ORDER+1] = {s_4_0_4.range(Y_0(192,4)), s_4_1_4.range(Y_0(192,4)), s_4_2_4.range(Y_0(192,4)), s_4_3_4.range(Y_0(192,4)), s_4_4_4.range(Y_0(320,4)), s_4_5_4.range(Y_0(320,4)), s_4_6_4.range(Y_0(320,4)), s_4_7_4.range(Y_0(320,4)), s_4_8_4.range(Y_0(320,4))};

		int i_Y1_ARM_5[2*ORDER+1] = {s_4_0_4.range(Y_0(192,5)), s_4_1_4.range(Y_0(192,5)), s_4_2_4.range(Y_0(192,5)), s_4_3_4.range(Y_0(192,5)), s_4_4_4.range(Y_0(320,5)), s_4_5_4.range(Y_0(320,5)), s_4_6_4.range(Y_0(320,5)), s_4_7_4.range(Y_0(320,5)), s_4_8_4.range(Y_0(320,5))};

		float Y1_ARM_0[2*ORDER+1], Y1_ARM_1[2*ORDER+1], Y1_ARM_2[2*ORDER+1], Y1_ARM_3[2*ORDER+1], Y1_ARM_4[2*ORDER+1], Y1_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Y1_ARM_0[i];
			Y1_ARM_0[i] = tmp.f;

			tmp.i = i_Y1_ARM_1[i];
			Y1_ARM_1[i] = tmp.f;

			tmp.i = i_Y1_ARM_2[i];
			Y1_ARM_2[i] = tmp.f;

			tmp.i = i_Y1_ARM_3[i];
			Y1_ARM_3[i] = tmp.f;

			tmp.i = i_Y1_ARM_4[i];
			Y1_ARM_4[i] = tmp.f;

			tmp.i = i_Y1_ARM_5[i];
			Y1_ARM_5[i] = tmp.f;
		}

		// Y2 ARM
		int i_Y2_ARM_0[2*ORDER+1] = {s_4_0_4.range(Y_0(384,0)), s_4_1_4.range(Y_0(384,0)), s_4_2_4.range(Y_0(384,0)), s_4_3_4.range(Y_0(384,0)), s_4_4_4.range(Y_0(576,0)), s_4_5_4.range(Y_0(576,0)), s_4_6_4.range(Y_0(576,0)), s_4_7_4.range(Y_0(576,0)), s_4_8_4.range(Y_0(576,0))}; 

		int i_Y2_ARM_1[2*ORDER+1] = {s_4_0_4.range(Y_0(384,1)), s_4_1_4.range(Y_0(384,1)), s_4_2_4.range(Y_0(384,1)), s_4_3_4.range(Y_0(384,1)), s_4_4_4.range(Y_0(576,1)), s_4_5_4.range(Y_0(576,1)), s_4_6_4.range(Y_0(576,1)), s_4_7_4.range(Y_0(576,1)), s_4_8_4.range(Y_0(576,1))}; 

		int i_Y2_ARM_2[2*ORDER+1] = {s_4_0_4.range(Y_0(384,2)), s_4_1_4.range(Y_0(384,2)), s_4_2_4.range(Y_0(384,2)), s_4_3_4.range(Y_0(384,2)), s_4_4_4.range(Y_0(576,2)), s_4_5_4.range(Y_0(576,2)), s_4_6_4.range(Y_0(576,2)), s_4_7_4.range(Y_0(576,2)), s_4_8_4.range(Y_0(576,2))}; 

		int i_Y2_ARM_3[2*ORDER+1] = {s_4_0_4.range(Y_0(384,3)), s_4_1_4.range(Y_0(384,3)), s_4_2_4.range(Y_0(384,3)), s_4_3_4.range(Y_0(384,3)), s_4_4_4.range(Y_0(576,3)), s_4_5_4.range(Y_0(576,3)), s_4_6_4.range(Y_0(576,3)), s_4_7_4.range(Y_0(576,3)), s_4_8_4.range(Y_0(576,3))};

		int i_Y2_ARM_4[2*ORDER+1] = {s_4_0_4.range(Y_0(384,4)), s_4_1_4.range(Y_0(384,4)), s_4_2_4.range(Y_0(384,4)), s_4_3_4.range(Y_0(384,4)), s_4_4_4.range(Y_0(576,4)), s_4_5_4.range(Y_0(576,4)), s_4_6_4.range(Y_0(576,4)), s_4_7_4.range(Y_0(576,4)), s_4_8_4.range(Y_0(576,4))};

		int i_Y2_ARM_5[2*ORDER+1] = {s_4_0_4.range(Y_0(384,5)), s_4_1_4.range(Y_0(384,5)), s_4_2_4.range(Y_0(384,5)), s_4_3_4.range(Y_0(384,5)), s_4_4_4.range(Y_0(576,5)), s_4_5_4.range(Y_0(576,5)), s_4_6_4.range(Y_0(576,5)), s_4_7_4.range(Y_0(576,5)), s_4_8_4.range(Y_0(576,5))};

		float Y2_ARM_0[2*ORDER+1], Y2_ARM_1[2*ORDER+1], Y2_ARM_2[2*ORDER+1], Y2_ARM_3[2*ORDER+1], Y2_ARM_4[2*ORDER+1], Y2_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Y2_ARM_0[i];
			Y2_ARM_0[i] = tmp.f;

			tmp.i = i_Y2_ARM_1[i];
			Y2_ARM_1[i] = tmp.f;

			tmp.i = i_Y2_ARM_2[i];
			Y2_ARM_2[i] = tmp.f;

			tmp.i = i_Y2_ARM_3[i];
			Y2_ARM_3[i] = tmp.f;

			tmp.i = i_Y2_ARM_4[i];
			Y2_ARM_4[i] = tmp.f;

			tmp.i = i_Y2_ARM_5[i];
			Y2_ARM_5[i] = tmp.f;
		}
		#undef Y_0

		#define Z_0(a,c) (a+32*(c+1) -1, a+32*c)
		// Z0 ARM
		int i_Z0_ARM_0[2*ORDER+1] = {s_4_4_0.range(Z_0(0,0)), s_4_4_1.range(Z_0(0,0)), s_4_4_2.range(Z_0(0,0)), s_4_4_3.range(Z_0(0,0)), s_4_4_4.range(Z_0(64,0)), s_4_4_5.range(Z_0(64,0)), s_4_4_6.range(Z_0(64,0)), s_4_4_7.range(Z_0(64,0)), s_4_4_8.range(Z_0(64,0))}; 

		int i_Z0_ARM_1[2*ORDER+1] = {s_4_4_0.range(Z_0(0,1)), s_4_4_1.range(Z_0(0,1)), s_4_4_2.range(Z_0(0,1)), s_4_4_3.range(Z_0(0,1)), s_4_4_4.range(Z_0(64,1)), s_4_4_5.range(Z_0(64,1)), s_4_4_6.range(Z_0(64,1)), s_4_4_7.range(Z_0(64,1)), s_4_4_8.range(Z_0(64,1))}; 

		int i_Z0_ARM_2[2*ORDER+1] = {s_4_4_0.range(Z_0(0,2)), s_4_4_1.range(Z_0(0,2)), s_4_4_2.range(Z_0(0,2)), s_4_4_3.range(Z_0(0,2)), s_4_4_4.range(Z_0(64,2)), s_4_4_5.range(Z_0(64,2)), s_4_4_6.range(Z_0(64,2)), s_4_4_7.range(Z_0(64,2)), s_4_4_8.range(Z_0(64,2))}; 

		int i_Z0_ARM_3[2*ORDER+1] = {s_4_4_0.range(Z_0(0,3)), s_4_4_1.range(Z_0(0,3)), s_4_4_2.range(Z_0(0,3)), s_4_4_3.range(Z_0(0,3)), s_4_4_4.range(Z_0(64,3)), s_4_4_5.range(Z_0(64,3)), s_4_4_6.range(Z_0(64,3)), s_4_4_7.range(Z_0(64,3)), s_4_4_8.range(Z_0(64,3))};

		int i_Z0_ARM_4[2*ORDER+1] = {s_4_4_0.range(Z_0(0,4)), s_4_4_1.range(Z_0(0,4)), s_4_4_2.range(Z_0(0,4)), s_4_4_3.range(Z_0(0,4)), s_4_4_4.range(Z_0(64,4)), s_4_4_5.range(Z_0(64,4)), s_4_4_6.range(Z_0(64,4)), s_4_4_7.range(Z_0(64,4)), s_4_4_8.range(Z_0(64,4))};

		int i_Z0_ARM_5[2*ORDER+1] = {s_4_4_0.range(Z_0(0,5)), s_4_4_1.range(Z_0(0,5)), s_4_4_2.range(Z_0(0,5)), s_4_4_3.range(Z_0(0,5)), s_4_4_4.range(Z_0(64,5)), s_4_4_5.range(Z_0(64,5)), s_4_4_6.range(Z_0(64,5)), s_4_4_7.range(Z_0(64,5)), s_4_4_8.range(Z_0(64,5))};

		float Z0_ARM_0[2*ORDER+1], Z0_ARM_1[2*ORDER+1], Z0_ARM_2[2*ORDER+1], Z0_ARM_3[2*ORDER+1], Z0_ARM_4[2*ORDER+1], Z0_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Z0_ARM_0[i];
			Z0_ARM_0[i] = tmp.f;

			tmp.i = i_Z0_ARM_1[i];
			Z0_ARM_1[i] = tmp.f;

			tmp.i = i_Z0_ARM_2[i];
			Z0_ARM_2[i] = tmp.f;

			tmp.i = i_Z0_ARM_3[i];
			Z0_ARM_3[i] = tmp.f;

			tmp.i = i_Z0_ARM_4[i];
			Z0_ARM_4[i] = tmp.f;

			tmp.i = i_Z0_ARM_5[i];
			Z0_ARM_5[i] = tmp.f;
		}

		// Z1 ARM
		int i_Z1_ARM_0[2*ORDER+1] = {s_4_4_0.range(Z_0(192,0)), s_4_4_1.range(Z_0(192,0)), s_4_4_2.range(Z_0(192,0)), s_4_4_3.range(Z_0(192,0)), s_4_4_4.range(Z_0(320,0)), s_4_4_5.range(Z_0(320,0)), s_4_4_6.range(Z_0(320,0)), s_4_4_7.range(Z_0(320,0)), s_4_4_8.range(Z_0(320,0))}; 

		int i_Z1_ARM_1[2*ORDER+1] = {s_4_4_0.range(Z_0(192,1)), s_4_4_1.range(Z_0(192,1)), s_4_4_2.range(Z_0(192,1)), s_4_4_3.range(Z_0(192,1)), s_4_4_4.range(Z_0(320,1)), s_4_4_5.range(Z_0(320,1)), s_4_4_6.range(Z_0(320,1)), s_4_4_7.range(Z_0(320,1)), s_4_4_8.range(Z_0(320,1))}; 

		int i_Z1_ARM_2[2*ORDER+1] = {s_4_4_0.range(Z_0(192,2)), s_4_4_1.range(Z_0(192,2)), s_4_4_2.range(Z_0(192,2)), s_4_4_3.range(Z_0(192,2)), s_4_4_4.range(Z_0(320,2)), s_4_4_5.range(Z_0(320,2)), s_4_4_6.range(Z_0(320,2)), s_4_4_7.range(Z_0(320,2)), s_4_4_8.range(Z_0(320,2))}; 

		int i_Z1_ARM_3[2*ORDER+1] = {s_4_4_0.range(Z_0(192,3)), s_4_4_1.range(Z_0(192,3)), s_4_4_2.range(Z_0(192,3)), s_4_4_3.range(Z_0(192,3)), s_4_4_4.range(Z_0(320,3)), s_4_4_5.range(Z_0(320,3)), s_4_4_6.range(Z_0(320,3)), s_4_4_7.range(Z_0(320,3)), s_4_4_8.range(Z_0(320,3))};

		int i_Z1_ARM_4[2*ORDER+1] = {s_4_4_0.range(Z_0(192,4)), s_4_4_1.range(Z_0(192,4)), s_4_4_2.range(Z_0(192,4)), s_4_4_3.range(Z_0(192,4)), s_4_4_4.range(Z_0(320,4)), s_4_4_5.range(Z_0(320,4)), s_4_4_6.range(Z_0(320,4)), s_4_4_7.range(Z_0(320,4)), s_4_4_8.range(Z_0(320,4))};

		int i_Z1_ARM_5[2*ORDER+1] = {s_4_4_0.range(Z_0(192,5)), s_4_4_1.range(Z_0(192,5)), s_4_4_2.range(Z_0(192,5)), s_4_4_3.range(Z_0(192,5)), s_4_4_4.range(Z_0(320,5)), s_4_4_5.range(Z_0(320,5)), s_4_4_6.range(Z_0(320,5)), s_4_4_7.range(Z_0(320,5)), s_4_4_8.range(Z_0(320,5))};

		float Z1_ARM_0[2*ORDER+1], Z1_ARM_1[2*ORDER+1], Z1_ARM_2[2*ORDER+1], Z1_ARM_3[2*ORDER+1], Z1_ARM_4[2*ORDER+1], Z1_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Z1_ARM_0[i];
			Z1_ARM_0[i] = tmp.f;

			tmp.i = i_Z1_ARM_1[i];
			Z1_ARM_1[i] = tmp.f;

			tmp.i = i_Z1_ARM_2[i];
			Z1_ARM_2[i] = tmp.f;

			tmp.i = i_Z1_ARM_3[i];
			Z1_ARM_3[i] = tmp.f;

			tmp.i = i_Z1_ARM_4[i];
			Z1_ARM_4[i] = tmp.f;

			tmp.i = i_Z1_ARM_5[i];
			Z1_ARM_5[i] = tmp.f;
		}
		// Z2 ARM
		int i_Z2_ARM_0[2*ORDER+1] = {s_4_4_0.range(Z_0(384,0)), s_4_4_1.range(Z_0(384,0)), s_4_4_2.range(Z_0(384,0)), s_4_4_3.range(Z_0(384,0)), s_4_4_4.range(Z_0(576,0)), s_4_4_5.range(Z_0(576,0)), s_4_4_6.range(Z_0(576,0)), s_4_4_7.range(Z_0(576,0)), s_4_4_8.range(Z_0(576,0))}; 

		int i_Z2_ARM_1[2*ORDER+1] = {s_4_4_0.range(Z_0(384,1)), s_4_4_1.range(Z_0(384,1)), s_4_4_2.range(Z_0(384,1)), s_4_4_3.range(Z_0(384,1)), s_4_4_4.range(Z_0(576,1)), s_4_4_5.range(Z_0(576,1)), s_4_4_6.range(Z_0(576,1)), s_4_4_7.range(Z_0(576,1)), s_4_4_8.range(Z_0(576,1))}; 

		int i_Z2_ARM_2[2*ORDER+1] = {s_4_4_0.range(Z_0(384,2)), s_4_4_1.range(Z_0(384,2)), s_4_4_2.range(Z_0(384,2)), s_4_4_3.range(Z_0(384,2)), s_4_4_4.range(Z_0(576,2)), s_4_4_5.range(Z_0(576,2)), s_4_4_6.range(Z_0(576,2)), s_4_4_7.range(Z_0(576,2)), s_4_4_8.range(Z_0(576,2))}; 

		int i_Z2_ARM_3[2*ORDER+1] = {s_4_4_0.range(Z_0(384,3)), s_4_4_1.range(Z_0(384,3)), s_4_4_2.range(Z_0(384,3)), s_4_4_3.range(Z_0(384,3)), s_4_4_4.range(Z_0(576,3)), s_4_4_5.range(Z_0(576,3)), s_4_4_6.range(Z_0(576,3)), s_4_4_7.range(Z_0(576,3)), s_4_4_8.range(Z_0(576,3))};

		int i_Z2_ARM_4[2*ORDER+1] = {s_4_4_0.range(Z_0(384,4)), s_4_4_1.range(Z_0(384,4)), s_4_4_2.range(Z_0(384,4)), s_4_4_3.range(Z_0(384,4)), s_4_4_4.range(Z_0(576,4)), s_4_4_5.range(Z_0(576,4)), s_4_4_6.range(Z_0(576,4)), s_4_4_7.range(Z_0(576,4)), s_4_4_8.range(Z_0(576,4))};

		int i_Z2_ARM_5[2*ORDER+1] = {s_4_4_0.range(Z_0(384,5)), s_4_4_1.range(Z_0(384,5)), s_4_4_2.range(Z_0(384,5)), s_4_4_3.range(Z_0(384,5)), s_4_4_4.range(Z_0(576,5)), s_4_4_5.range(Z_0(576,5)), s_4_4_6.range(Z_0(576,5)), s_4_4_7.range(Z_0(576,5)), s_4_4_8.range(Z_0(576,5))};

		float Z2_ARM_0[2*ORDER+1], Z2_ARM_1[2*ORDER+1], Z2_ARM_2[2*ORDER+1], Z2_ARM_3[2*ORDER+1], Z2_ARM_4[2*ORDER+1], Z2_ARM_5[2*ORDER+1];
		for(int i = 0; i < 2*ORDER+1; i++){
			data_conv tmp;
			tmp.i = i_Z2_ARM_0[i];
			Z2_ARM_0[i] = tmp.f;

			tmp.i = i_Z2_ARM_1[i];
			Z2_ARM_1[i] = tmp.f;

			tmp.i = i_Z2_ARM_2[i];
			Z2_ARM_2[i] = tmp.f;

			tmp.i = i_Z2_ARM_3[i];
			Z2_ARM_3[i] = tmp.f;

			tmp.i = i_Z2_ARM_4[i];
			Z2_ARM_4[i] = tmp.f;

			tmp.i = i_Z2_ARM_5[i];
			Z2_ARM_5[i] = tmp.f;
		}

		float mem_wr_k0[PORT_WIDTH];
		float mem_wr_k1[PORT_WIDTH];
		float mem_wr_k2[PORT_WIDTH];

		float mem_wr_k_dt0[PORT_WIDTH];
		float mem_wr_k_dt1[PORT_WIDTH];
		float mem_wr_k_dt2[PORT_WIDTH];

		float mem_wr_y_tmp0[PORT_WIDTH];
		float mem_wr_y_tmp1[PORT_WIDTH];
		float mem_wr_y_tmp2[PORT_WIDTH];

		float yy_final_arr0[PORT_WIDTH];
		float yy_final_arr1[PORT_WIDTH];
		float yy_final_arr2[PORT_WIDTH];


		float s_4_4_4_arr0[PORT_WIDTH];
		float s_4_4_4_arr1[PORT_WIDTH];
		float s_4_4_4_arr2[PORT_WIDTH];

		#define S_0(a,c) (a+32*(c+1) -1, a+32*c)
		vec2s_4_4_4_arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.i = s_4_4_4.range(S_0(0,k));
			s_4_4_4_arr0[k] = tmp.f;

			tmp.i = s_4_4_4.range(S_0(256,k));
			s_4_4_4_arr1[k] = tmp.f;

			tmp.i = s_4_4_4.range(S_0(512,k));
			s_4_4_4_arr2[k] = tmp.f;
		}

		// X-ARM
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X0_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=X1_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X1_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X1_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X1_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X1_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X1_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=X2_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X2_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X2_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X2_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X2_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=X2_ARM_5 complete dim=1


		// Y-ARM
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y0_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y1_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Y2_ARM_5 complete dim=1


		// Z-ARM
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z0_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z1_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_3 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_4 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=Z2_ARM_5 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=mem_wr_k0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=mem_wr_k1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=mem_wr_k2 complete dim=1

		#pragma HLS ARRAY_PARTITION variable=s_4_4_4_arr0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=s_4_4_4_arr1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=s_4_4_4_arr2 complete dim=1




	  	float sigma = s_4_4_4_arr[1]/s_4_4_4_arr[0];  //mu[OPS_ACC5(0,0,0)]/rho[OPS_ACC4(0,0,0)];
	  	float sigmax=0.0;
	  	float sigmay=0.0;
	  	float sigmaz=0.0;

	  	short idx = k - ORDER;
	  	short idy = j - ORDER;
	  	short idz = i - 2*ORDER;


	  	float sigmax1, sigmax2, sigmay1, sigmay2, sigmaz1, sigmaz2;


	  	bool idx_cond1 = idx <= xbeg+pml_width;
	  	bool idx_cond2 = idx >=xend-pml_width;
	  	sigmax1 = (xbeg+pml_width-idx ) * sigma * 0.1f;//sigma/pml_width;
	  	sigmax2 =(idx -(xend-pml_width)) * sigma * 0.1f; //sigma/pml_width;
	  	sigmax = idx_cond2 ? sigmax2 : (idx_cond1 ? sigmax1 : 0.0f);

	  	bool idy_cond1 = idy <= ybeg+pml_width;
	  	bool idy_cond2 = idy >= yend-pml_width;
	  	sigmay1=(ybeg+pml_width-idy) * sigma * 0.1f; //sigma/pml_width;
	  	sigmay2=(idy-(yend-pml_width)) * sigma * 0.1f; //sigma/pml_width;
	  	sigmay = idy_cond2 ? sigmay2 : (idy_cond1 ? sigmay1 : 0.0f);

	  	bool idz_cond1 = idz <= zbeg+pml_width;
	  	bool idz_cond2 = idz >= zend-pml_width;
	  	sigmaz1=(zbeg+pml_width-idz) * sigma * 0.1f; //sigma/pml_width;
	  	sigmaz2=(idz -(zend-pml_width)) * sigma * 0.1f; //sigma/pml_width;
	  	sigmaz = idz_cond2 ? sigmaz2 : (idz_cond1 ? sigmaz1 : 0.0f);


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
		    
		    vxy += Y_ARM_3[l] * c[l];
		    vyy += Y_ARM_4[l] * c[l];
		    vzy += Y_ARM_5[l] * c[l];
		    
		    pxz += Z_ARM_0[l] * c[l];
		    pyz += Z_ARM_1[l] * c[l];
		    pzz += Z_ARM_2[l] * c[l];
		    
		    vxz += Z_ARM_3[l] * c[l];
		    vyz += Z_ARM_4[l] * c[l];
		    vzz += Z_ARM_5[l] * c[l];

//			#pragma HLS RESOURCE variable=pxx core=FAddSub_nodsp
//			#pragma HLS RESOURCE variable=pyx core=FAddSub_nodsp
//			#pragma HLS RESOURCE variable=pzx core=FAddSub_nodsp
//			#pragma HLS RESOURCE variable=vxx core=FAddSub_nodsp
//			#pragma HLS RESOURCE variable=vyx core=FAddSub_nodsp
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


		float k_2_1 = vxx/s_4_4_4_arr[0];
		float k_2_2 =  sigmax*px;
		mem_wr_k[2]= k_2_1 - k_2_2;            //vxx/rho[OPS_ACC4(0,0,0)] - sigmax*px;

		float k_5_1 = (pxx+pyx+pxz);
		float k_5_2 = k_5_1 *s_4_4_4_arr[1];
		float k_5_3 = sigmax*vx;
		mem_wr_k[5]= k_5_2 - k_5_3;  //(pxx+pyx+pxz)*mu[OPS_ACC5(0,0,0)] - sigmax*vx;

		float k_3_1 = vyy/s_4_4_4_arr[0];
		float k_3_2 = sigmay*py;
		mem_wr_k[3]= k_3_1 - k_3_2;  		  // vyy/rho[OPS_ACC4(0,0,0)] - sigmay*py;

		float k_6_1 = (pxy+pyy+pyz);
		float k_6_2 = k_6_1 *s_4_4_4_arr[1];
		float k_6_3 = sigmay*vy;
		mem_wr_k[6]= k_6_2 - k_6_3;   //(pxy+pyy+pyz)*mu[OPS_ACC5(0,0,0)] - sigmay*vy;

		float k_4_1 = vzz/s_4_4_4_arr[0];
		float k_4_2 = sigmaz*pz;
		mem_wr_k[4]= k_4_1  - k_4_2;  		  //vzz/rho[OPS_ACC4(0,0,0)] - sigmaz*pz;

		float k_7_1 = (pxz+pyz+pzz);
		float k_7_2 = k_7_1*s_4_4_4_arr[1];
		float k_7_3 = sigmaz*vz;
		mem_wr_k[7]= k_7_2 - k_7_3;  //(pxz+pyz+pzz)*mu[OPS_ACC5(0,0,0)] - sigmaz*vz;


  		mem_wr_k[0] = s_4_4_4_arr[0];
  		mem_wr_k[1] = s_4_4_4_arr[1];


  		// calc K dt
  		mem_wr_k_dt[2] = mem_wr_k[2] * 0.1f;
  		mem_wr_k_dt[5] = mem_wr_k[5] * 0.1f;
  		
  		mem_wr_k_dt[3] = mem_wr_k[3] * 0.1f;
  		mem_wr_k_dt[6] = mem_wr_k[6] * 0.1f;
  		
  		mem_wr_k_dt[4] = mem_wr_k[4] * 0.1f;
  		mem_wr_k_dt[7] = mem_wr_k[7] * 0.1f;

  		mem_wr_k_dt[0] = s_4_4_4_arr[0];
  		mem_wr_k_dt[1] = s_4_4_4_arr[1];

  		// calc Y temp
  		mem_wr_y_tmp[2] = s_4_4_4_arr[2] + mem_wr_k_dt[2]*0.5f;
  		mem_wr_y_tmp[5] = s_4_4_4_arr[5] + mem_wr_k_dt[5]*0.5f;
  	
  		mem_wr_y_tmp[3] = s_4_4_4_arr[3] + mem_wr_k_dt[3]*0.5f;
  		mem_wr_y_tmp[6] = s_4_4_4_arr[6] + mem_wr_k_dt[6]*0.5f;
  	
  		mem_wr_y_tmp[4] = s_4_4_4_arr[4] + mem_wr_k_dt[4]*0.5f;
  		mem_wr_y_tmp[7] = s_4_4_4_arr[7] + mem_wr_k_dt[7]*0.5f;

  		mem_wr_y_tmp[0] = s_4_4_4_arr[0];
  		mem_wr_y_tmp[1] = s_4_4_4_arr[1];


  		// calc Y final
  		yy_final_arr[2] = s_4_4_4_arr[2] + mem_wr_k_dt[2] *  0.1666666667f;
  		yy_final_arr[5] = s_4_4_4_arr[5] + mem_wr_k_dt[5] *  0.1666666667f;

  		yy_final_arr[3] = s_4_4_4_arr[3] + mem_wr_k_dt[3] *  0.1666666667f;
  		yy_final_arr[6] = s_4_4_4_arr[6] + mem_wr_k_dt[6] *  0.1666666667f;

  		yy_final_arr[4] = s_4_4_4_arr[4] + mem_wr_k_dt[4] *  0.1666666667f;
  		yy_final_arr[7] = s_4_4_4_arr[7] + mem_wr_k_dt[7] *  0.1666666667f;

  		yy_final_arr[0] = s_4_4_4_arr[0];
  		yy_final_arr[1] = s_4_4_4_arr[1];

  		bool change_cond1 = (idx < 0) || (idx >= sizex) || (idy < 0) ;
  		bool change_cond2 = (idy >= sizey ) || (idz < 0) || (idz >= sizez);
		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = 1;//change_cond1 || change_cond2;
			tmp.f = change_cond ? s_4_4_4_arr0[k] : mem_wr_y_tmp[k];
			update0.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = 1;//change_cond1 || change_cond2;
			tmp.f = change_cond ? s_4_4_4_arr1[k] : mem_wr_y_tmp[k];
			update1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = 1;//change_cond1 || change_cond2;
			tmp.f = change_cond ? s_4_4_4_arr2[k] : mem_wr_y_tmp[k];
			update2.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}


		yy_final2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = change_cond1 || change_cond2;
			tmp.f = change_cond ? s_4_4_4_arr[k] : yy_final_arr[k];
			yy_final_vec.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		bool cond_wr = (i >= ORDER) && ( i < grid_sizez + ORDER);
		if(cond_wr ) {
			wr_buffer0 <<  update0;
			wr_buffer1 <<  update1;
			wr_buffer2 <<  update2;
//			yy <<  s_4_4_4;
//			yy_final << yy_final_vec;
		}

		// move the cell block
		k_dum = k + 1;
	}
}
