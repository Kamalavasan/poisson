static void derives_calc_ytep_k3( hls::stream<uint256_dt> &rd_buffer, hls::stream<uint256_dt> &yy_read, hls::stream<uint256_dt> &yy_final_read, hls::stream<uint256_dt> &wr_buffer, hls::stream<uint256_dt> &yy_write, hls::stream<uint256_dt> &yy_final_write,struct data_G data_g){
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

	uint256_dt window_yy[4*plane_buff_size];
	uint256_dt window_yy_final[4*plane_buff_size];

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

	#pragma HLS RESOURCE variable=window_yy core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_yy_final core=XPM_MEMORY uram latency=2

	uint256_dt s_4_4_8, s_4_4_7, s_4_4_6, s_4_4_5;
	uint256_dt s_4_8_4, s_4_7_4, s_4_6_4, s_4_5_4;
	uint256_dt s_8_4_4, s_7_4_4, s_6_4_4, s_5_4_4;
	uint256_dt s_4_4_4;
	uint256_dt s_3_4_4, s_2_4_4, s_1_4_4, s_0_4_4;
	uint256_dt s_4_3_4, s_4_2_4, s_4_1_4, s_4_0_4;
	uint256_dt s_4_4_3, s_4_4_2, s_4_4_1, s_4_4_0;
	uint256_dt yy_vec, yy_vec_tmp;
	uint256_dt yy_final_vec, yy_final_vec_tmp, yy_final_vec_write;
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
	unsigned short j_p = 0, j_l = 0, j_p_diff = 0, j_l_diff = 0, j_p_4 = 0;;
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



		yy_vec = window_yy[j_p_4];
		bool cond_tmp2 = (i < grid_sizez);
		if(cond_tmp1){
			yy_vec_tmp = yy_read.read(); // set
		}
		window_yy[j_p_4] = yy_vec_tmp;


		yy_final_vec = window_yy_final[j_p_4];
		bool cond_tmp2 = (i < grid_sizez);
		if(cond_tmp1){
			yy_final_vec_tmp = yy_final_read.read(); // set
		}
		window_yy_final[j_p_4] = yy_final_vec_tmp;



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

		j_p_4++;
		if(j_p_4 >= 4*plane_size){
			j_p_4 = 0;
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

		float mem_wr_k[PORT_WIDTH];
		float mem_wr_k_dt[PORT_WIDTH];
		float mem_wr_y_tmp[PORT_WIDTH];
		float yy_final_arr[PORT_WIDTH];
		float s_4_4_4_arr[PORT_WIDTH];
		float yy_vec_arr[PORT_WIDTH];
		vec2s_4_4_4_arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.i = yy_vec.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			yy_vec_arr[k] = tmp.f;
		}

		vecs2_4_4_4_arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.i = s_4_4_4.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_4_4_4_arr[k] = tmp.f;
		}

		vecs2_yy_final_vec_write: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.i = yy_final_vec.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			yy_final_arr[k] = tmp.f;
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
	  	short idz = i - 2*ORDER;

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
		    
		    vxy += Y_ARM_3[l] * c[l];
		    vyy += Y_ARM_4[l] * c[l];
		    vzy += Y_ARM_5[l] * c[l];
		    
		    pxz += Z_ARM_0[l] * c[l];
		    pyz += Z_ARM_1[l] * c[l];
		    pzz += Z_ARM_2[l] * c[l];
		    
		    vxz += Z_ARM_3[l] * c[l];
		    vyz += Z_ARM_4[l] * c[l];
		    vzz += Z_ARM_5[l] * c[l];
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


  		mem_wr_k[2]= vxx - sigmax*px;            //vxx/rho[OPS_ACC4(0,0,0)] - sigmax*px;
  		mem_wr_k[5]= (pxx+pyx+pxz) - sigmax*vx;  //(pxx+pyx+pxz)*mu[OPS_ACC5(0,0,0)] - sigmax*vx;
  		
  		mem_wr_k[3]= vyy - sigmay*py;  		  // vyy/rho[OPS_ACC4(0,0,0)] - sigmay*py;
  		mem_wr_k[6]= (pxy+pyy+pyz)- sigmay*vy;   //(pxy+pyy+pyz)*mu[OPS_ACC5(0,0,0)] - sigmay*vy;
  		
  		mem_wr_k[4]= vzz  - sigmaz*pz;  		  //vzz/rho[OPS_ACC4(0,0,0)] - sigmaz*pz;
  		mem_wr_k[7]= (pxz+pyz+pzz) - sigmaz*vz;  //(pxz+pyz+pzz)*mu[OPS_ACC5(0,0,0)] - sigmaz*vz;

  		mem_wr_k[0] = s_4_4_4_arr[0];
  		mem_wr_k[1] = s_4_4_4_arr[1];


  		// calc K dt
  		mem_wr_k_dt[2] = mem_wr_k[2] * 0.1;
  		mem_wr_k_dt[5] = mem_wr_k[5] * 0.1;
  		
  		mem_wr_k_dt[3] = mem_wr_k[3] * 0.1;
  		mem_wr_k_dt[6] = mem_wr_k[6] * 0.1;
  		
  		mem_wr_k_dt[4] = mem_wr_k[4] * 0.1;
  		mem_wr_k_dt[7] = mem_wr_k[7] * 0.1;

  		mem_wr_k_dt[0] = s_4_4_4_arr[0];
  		mem_wr_k_dt[1] = s_4_4_4_arr[1];

  		// YY final 
  		yy_final_arr[2] += mem_wr_k_dt[2] * 0.33333333;
  		yy_final_arr[5] += mem_wr_k_dt[5] * 0.33333333;

  		yy_final_arr[3] += mem_wr_k_dt[3] * 0.33333333;
  		yy_final_arr[6] += mem_wr_k_dt[6] * 0.33333333;

  		yy_final_arr[4] += mem_wr_k_dt[4] * 0.33333333;
  		yy_final_arr[7] += mem_wr_k_dt[7] * 0.33333333;

  		yy_final_arr[0] += mem_wr_k_dt[0] * 0.33333333;
  		yy_final_arr[1] += mem_wr_k_dt[1] * 0.33333333;

  		// calc Y temp
  		mem_wr_y_tmp[2] = yy_vec_arr[2] + mem_wr_k_dt[2];
  		mem_wr_y_tmp[5] = yy_vec_arr[5] + mem_wr_k_dt[5];
  	
  		mem_wr_y_tmp[3] = yy_vec_arr[3] + mem_wr_k_dt[3];
  		mem_wr_y_tmp[6] = yy_vec_arr[6] + mem_wr_k_dt[6];
  	
  		mem_wr_y_tmp[4] = yy_vec_arr[4] + mem_wr_k_dt[4];
  		mem_wr_y_tmp[7] = yy_vec_arr[7] + mem_wr_k_dt[7];

  		mem_wr_y_tmp[0] = s_4_4_4_arr[0];
  		mem_wr_y_tmp[1] = s_4_4_4_arr[1];


		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			bool change_cond = (idx < 0 || idx >= sizex || (idy < 0) || (idy >= sizey ) || (idz < 0) || (idz >= sizez));
//			tmp.f =  s_4_4_4_arr[k] ;
			tmp.f = change_cond ? s_4_4_4_arr[k] : mem_wr_y_tmp[k];
			update_j.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}


		yy_final_arr2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.f = yy_final_arr[k];
			yy_final_vec_write.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}


		bool cond_wr = (i >= ORDER) && ( i < grid_sizez + ORDER);
		if(cond_wr ) {
			wr_buffer <<  update_j;
			yy_write <<  yy_vec;
			yy_final_write << yy_final_vec_write;
		}

		// move the cell block
		k++;
	}
}
