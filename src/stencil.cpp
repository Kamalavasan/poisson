static void axis2_fifo256(hls::stream <t_pkt> &in, hls::stream<uint256_dt> &out, unsigned int total_itr){
//	unsigned short end_index = (tile_x >> (SHIFT_BITS));
//	unsigned short end_row = size_y+2;
//	unsigned int tot_itr = end_row * end_index;
	for (unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp = in.read();
		out << tmp.data;
	}
}

static void fifo256_2axis(hls::stream <uint256_dt> &in, hls::stream<t_pkt> &out, unsigned int total_itr){
//	unsigned short end_index = (tile_x >> (SHIFT_BITS));
//	unsigned short end_row = size_y+2;
//	unsigned int tot_itr = end_row * end_index;
	for (unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt tmp;
		tmp.data = in.read();
		out.write(tmp);
	}
}


static void process_tile( hls::stream<uint256_dt> &rd_buffer, hls::stream<uint256_dt> &wr_buffer, struct data_G data_g){
	unsigned short xblocks = data_g.xblocks;
	unsigned short sizex = data_g.sizex;
	unsigned short sizey = data_g.sizey;
	unsigned short sizez = data_g.sizez;
	unsigned short limit_z = data_g.limit_z;
	unsigned short grid_sizey = data_g.grid_sizey;
	unsigned short grid_sizez = data_g.grid_sizez;
	unsigned int line_diff = data_g.line_diff;
	unsigned int plane_diff = data_g.plane_diff;
	unsigned int gridsize = data_g.gridsize_pr;

	float s_1_1_2_arr[PORT_WIDTH];
	float s_1_2_1_arr[PORT_WIDTH];
	float s_1_1_1_arr[PORT_WIDTH+2];
	float s_1_0_1_arr[PORT_WIDTH];
	float s_1_1_0_arr[PORT_WIDTH];

	float mem_wr[PORT_WIDTH];

	#pragma HLS ARRAY_PARTITION variable=s_1_1_2_arr complete dim=1
	#pragma HLS ARRAY_PARTITION variable=s_1_2_1_arr complete dim=1
	#pragma HLS ARRAY_PARTITION variable=s_1_1_1_arr complete dim=1
	#pragma HLS ARRAY_PARTITION variable=s_1_0_1_arr complete dim=1
	#pragma HLS ARRAY_PARTITION variable=s_1_1_0_arr complete dim=1
	#pragma HLS ARRAY_PARTITION variable=mem_wr complete dim=1

	uint256_dt window_1[8192];
	uint256_dt window_2[256];
	uint256_dt window_3[256];
	uint256_dt window_4[8192];

	#pragma HLS RESOURCE variable=window_1 core=XPM_MEMORY uram latency=1
	#pragma HLS RESOURCE variable=window_2 core=XPM_MEMORY uram latency=1
	#pragma HLS RESOURCE variable=window_3 core=XPM_MEMORY uram latency=1
	#pragma HLS RESOURCE variable=window_4 core=XPM_MEMORY uram latency=1

	uint256_dt s_1_1_2, s_1_2_1, s_1_1_1, s_1_1_1_b, s_1_1_1_f, s_1_0_1, s_1_1_0;
	uint256_dt update_j;


	unsigned short i = 0, j = 0, k = 0;
	unsigned short j_p = 0, j_l = 0;
	for(unsigned int itr = 0; itr < gridsize; itr++) {
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=1

		if(k == xblocks){
			k = 0;
			j++;
		}

		if(j == grid_sizey){
			j = 0;
			i++;
		}



//		printf("i, j ,k is %d, %d, %d, itr %d gridsize %d\n", i,j,k,itr, gridsize);


		s_1_1_0 = window_4[j_p];

		s_1_0_1 = window_3[j_l];
		window_4[j_p] = s_1_0_1;

		s_1_1_1_b = s_1_1_1;
		window_3[j_l] = s_1_1_1_b;

		s_1_1_1 = s_1_1_1_f;
		s_1_1_1_f = window_2[j_l]; 	// read

		s_1_2_1 = window_1[j_p];   // read
		window_2[j_l] = s_1_2_1;	//set


		bool cond_tmp1 = (i < grid_sizez);
		if(cond_tmp1){
			s_1_1_2 = rd_buffer.read(); // set
		}
		window_1[j_p] = s_1_1_2; // set



		j_p++;
		if(j_p == plane_diff){
			j_p = 0;
		}

		j_l++;
		if(j_l == line_diff){
			j_l = 0;
		}

		vec2arr: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv s_1_1_2_u, s_1_2_1_u, s_1_1_1_u, s_1_0_1_u, s_1_1_0_u;
			s_1_1_2_u.i = s_1_1_2.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_2_1_u.i = s_1_2_1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_1_u.i = s_1_1_1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_0_1_u.i = s_1_0_1.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_0_u.i = s_1_1_0.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);

			s_1_1_2_arr[k]   =  s_1_1_2_u.f;
			s_1_2_1_arr[k]   =  s_1_2_1_u.f;
			s_1_1_1_arr[k+1] =  s_1_1_1_u.f;
			s_1_0_1_arr[k]   =  s_1_0_1_u.f;
			s_1_1_0_arr[k]   =  s_1_1_0_u.f;

		}
		data_conv tmp1_o1, tmp2_o2;
		tmp1_o1.i = s_1_1_1_b.range(DATATYPE_SIZE * (PORT_WIDTH) - 1, (PORT_WIDTH-1) * DATATYPE_SIZE);
		tmp2_o2.i = s_1_1_1_f.range(DATATYPE_SIZE * (0 + 1) - 1, 0 * DATATYPE_SIZE);
		s_1_1_1_arr[0] = tmp1_o1.f;
		s_1_1_1_arr[PORT_WIDTH + 1] = tmp2_o2.f;



		process: for(short q = 0; q < PORT_WIDTH; q++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			short index = (k << SHIFT_BITS) + q;
			float r1_1_2 =  s_1_1_2_arr[q] * 0.02;
			float r1_2_1 =  s_1_2_1_arr[q] * 0.04;
			float r0_1_1 =  s_1_1_1_arr[q] * 0.05;
			float r1_1_1 =  s_1_1_1_arr[q+1] * 0.79;
			float r2_1_1 =  s_1_1_1_arr[q+2] * 0.06;
			float r1_0_1 =  s_1_0_1_arr[q] * 0.03;
			float r1_1_0 =  s_1_1_0_arr[q] * 0.01;

			float f1 = r1_1_2 + r1_2_1;
			float f2 = r0_1_1 + r1_1_1;
			float f3 = r2_1_1 + r1_0_1;

			float r1 = f1 + f2;
			float r2=  f3 + r1_1_0;

			float result  = r1 + r2;
			bool change_cond = (index <= 0 || index > sizex || (i <= 1) || (i >= limit_z -1) || (j == 0) || (j == grid_sizey -1));
			mem_wr[q] = change_cond ? s_1_1_1_arr[q+1] : result;
		}

		array2vec: for(int k = 0; k < PORT_WIDTH; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp;
			tmp.f = mem_wr[k];
			update_j.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp.i;
		}

		bool cond_wr = (i >= 1) && ( i <= limit_z);
		if(cond_wr ) {
			wr_buffer << update_j;
		}

		// move the cell block
		k++;
	}
}
