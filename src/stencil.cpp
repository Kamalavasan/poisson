static void axis2_fifo2048(hls::stream <t_pkt_1024> &in0, hls::stream <t_pkt_1024> &in1,
		hls::stream<uint1024_dt> &out0, hls::stream<uint1024_dt> &out1, unsigned int total_itr){

	for (unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt_1024 tmp0 = in0.read();
		t_pkt_1024 tmp1 = in1.read();

		out0 << tmp0.data;
		out1 << tmp1.data;
	}
}

static void fifo2048_2axis(hls::stream <uint1024_dt> &in0, hls::stream <uint1024_dt> &in1,
		hls::stream<t_pkt_1024> &out0, hls::stream<t_pkt_1024> &out1, unsigned int total_itr){

	for (unsigned int itr = 0; itr < total_itr; itr++){
		#pragma HLS PIPELINE II=1
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		t_pkt_1024 tmp0, tmp1;
		tmp0.data = in0.read();
		tmp1.data = in1.read();
		out0.write(tmp0);
		out1.write(tmp1);
	}
}


static void process_tile( hls::stream<uint1024_dt> &rd_buffer0, hls::stream<uint1024_dt> &rd_buffer1,
		hls::stream<uint1024_dt> &wr_buffer0, hls::stream<uint1024_dt> &wr_buffer1, struct data_G data_g){
	unsigned short xblocks = (data_g.xblocks);
	unsigned short sizex = data_g.sizex;
	unsigned short sizey = data_g.sizey;
	unsigned short sizez = data_g.sizez;
	unsigned short limit_z = data_g.limit_z;
	unsigned short grid_sizey = data_g.grid_sizey;
	unsigned short grid_sizez = data_g.grid_sizez;
	unsigned short tile_y = data_g.tile_y;

	unsigned short offset_x = data_g.offset_x;
	unsigned short offset_y = data_g.offset_y;

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

	uint1024_dt window_1_l[MAX_DEPTH_P];
	uint1024_dt window_2_l[MAX_DEPTH_L*2];
	uint1024_dt window_3_l[MAX_DEPTH_L*2];
	uint1024_dt window_4_l[MAX_DEPTH_P];

	uint1024_dt window_1_u[MAX_DEPTH_P];
	uint1024_dt window_2_u[MAX_DEPTH_L*2];
	uint1024_dt window_3_u[MAX_DEPTH_L*2];
	uint1024_dt window_4_u[MAX_DEPTH_P];

	#pragma HLS RESOURCE variable=window_1_l core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_2_l core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_3_l core=XPM_MEMORY uram latency=2
//	#pragma HLS RESOURCE variable=window_2_l core=RAM_1P_BRAM latency=2
//	#pragma HLS RESOURCE variable=window_3_l core=RAM_1P_BRAM latency=2
	#pragma HLS RESOURCE variable=window_4_l core=XPM_MEMORY uram latency=2

	#pragma HLS RESOURCE variable=window_1_u core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_2_u core=XPM_MEMORY uram latency=2
	#pragma HLS RESOURCE variable=window_3_u core=XPM_MEMORY uram latency=2
//	#pragma HLS RESOURCE variable=window_2_u core=RAM_1P_BRAM latency=2
//	#pragma HLS RESOURCE variable=window_3_u core=RAM_1P_BRAM latency=2
	#pragma HLS RESOURCE variable=window_4_u core=XPM_MEMORY uram latency=2

	uint1024_dt s_1_1_2_l, s_1_2_1_l, s_1_1_1_l, s_1_1_1_l_b, s_1_1_1_l_f, s_1_0_1_l, s_1_1_0_l;
	uint1024_dt s_1_1_2_u, s_1_2_1_u, s_1_1_1_u, s_1_1_1_u_b, s_1_1_1_u_f, s_1_0_1_u, s_1_1_0_u;
	uint1024_dt update_j_l, update_j_u;


	unsigned short i = 0, j = 0, k = 0;
	unsigned short i_dum = 0, j_dum = 0, k_dum = 0;
	unsigned short j_p = 0, j_l = 0;
	for(unsigned int itr = 0; itr < gridsize; itr++) {
		#pragma HLS loop_tripcount min=min_grid max=max_grid avg=avg_grid
		#pragma HLS PIPELINE II=1
		bool cond_k = (k == xblocks - 1);
		bool cond_j = (j == tile_y - 1);

		if(cond_k){
			k_dum = 0;
		}
		// fix me: will fail for xblocks = 1

		if(cond_k && cond_j){
			j_dum = 0;
		} else if(cond_k){
			j_dum = j +  1;
		}

		if(cond_k && cond_j){
			i_dum = i + 1;
		}


		k = k_dum;
		j = j_dum;
		i = i_dum;


		s_1_1_0_l = window_4_l[j_p];
		s_1_1_0_u = window_4_u[j_p];

		s_1_0_1_l = window_3_l[j_l];
		window_4_l[j_p] = s_1_0_1_l;
		s_1_0_1_u = window_3_u[j_l];
		window_4_u[j_p] = s_1_0_1_u;

		s_1_1_1_l_b = s_1_1_1_l;
		window_3_l[j_l] = s_1_1_1_l_b;
		s_1_1_1_u_b = s_1_1_1_u;
		window_3_u[j_l] = s_1_1_1_u_b;

		s_1_1_1_l = s_1_1_1_l_f;
		s_1_1_1_l_f = window_2_l[j_l];
		s_1_1_1_u = s_1_1_1_u_f;
		s_1_1_1_u_f = window_2_u[j_l];

		s_1_2_1_l = window_1_l[j_p];
		window_2_l[j_l] = s_1_2_1_l;
		s_1_2_1_u = window_1_u[j_p];
		window_2_u[j_l] = s_1_2_1_u;


		bool cond_tmp1 = (i < grid_sizez);
		if(cond_tmp1){
			s_1_1_2_l = rd_buffer0.read(); // set
			s_1_1_2_u = rd_buffer1.read(); // set
		}
		window_1_l[j_p] = s_1_1_2_l; // set
		window_1_u[j_p] = s_1_1_2_u; // set



		j_p++;
		if(j_p == plane_diff){
			j_p = 0;
		}

		j_l++;
		if(j_l == line_diff){
			j_l = 0;
		}

		vec2arr: for(int k = 0; k < PORT_WIDTH/2; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv s_1_1_2_u_conv, s_1_2_1_u_conv, s_1_1_1_u_conv, s_1_0_1_u_conv, s_1_1_0_u_conv;
			data_conv s_1_1_2_l_conv, s_1_2_1_l_conv, s_1_1_1_l_conv, s_1_0_1_l_conv, s_1_1_0_l_conv;

			s_1_1_2_u_conv.i = s_1_1_2_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_2_1_u_conv.i = s_1_2_1_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_1_u_conv.i = s_1_1_1_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_0_1_u_conv.i = s_1_0_1_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_0_u_conv.i = s_1_1_0_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);

			s_1_1_2_arr[k+PORT_WIDTH/2]   =  s_1_1_2_u_conv.f;
			s_1_2_1_arr[k+PORT_WIDTH/2]   =  s_1_2_1_u_conv.f;
			s_1_1_1_arr[k+1+PORT_WIDTH/2] =  s_1_1_1_u_conv.f;
			s_1_0_1_arr[k+PORT_WIDTH/2]   =  s_1_0_1_u_conv.f;
			s_1_1_0_arr[k+PORT_WIDTH/2]   =  s_1_1_0_u_conv.f;

			s_1_1_2_l_conv.i = s_1_1_2_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_2_1_l_conv.i = s_1_2_1_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_1_l_conv.i = s_1_1_1_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_0_1_l_conv.i = s_1_0_1_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);
			s_1_1_0_l_conv.i = s_1_1_0_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE);

			s_1_1_2_arr[k]   =  s_1_1_2_l_conv.f;
			s_1_2_1_arr[k]   =  s_1_2_1_l_conv.f;
			s_1_1_1_arr[k+1] =  s_1_1_1_l_conv.f;
			s_1_0_1_arr[k]   =  s_1_0_1_l_conv.f;
			s_1_1_0_arr[k]   =  s_1_1_0_l_conv.f;

		}
		data_conv tmp1_o1, tmp2_o2;
		tmp1_o1.i = s_1_1_1_u_b.range(DATATYPE_SIZE * (PORT_WIDTH/2) - 1, (PORT_WIDTH/2-1) * DATATYPE_SIZE);
		tmp2_o2.i = s_1_1_1_l_f.range(DATATYPE_SIZE * (0 + 1) - 1, 0 * DATATYPE_SIZE);


		s_1_1_1_arr[0] = tmp1_o1.f;
		s_1_1_1_arr[PORT_WIDTH + 1] = tmp2_o2.f;


		unsigned short y_index = j + offset_y;
		process_l: for(short q = 0; q < PORT_WIDTH; q++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			short index = (k << (SHIFT_BITS+3)) + q + offset_x;
			float r1_1_2 =  s_1_1_2_arr[q] * 0.02f;
			float r1_2_1 =  s_1_2_1_arr[q] * 0.04f;
			float r0_1_1 =  s_1_1_1_arr[q] * 0.05f;
			float r1_1_1 =  s_1_1_1_arr[q+1] * 0.79f;
			float r2_1_1 =  s_1_1_1_arr[q+2] * 0.06f;
			float r1_0_1 =  s_1_0_1_arr[q] * 0.03f;
			float r1_1_0 =  s_1_1_0_arr[q] * 0.01f;

			float f1 = r1_1_2 + r1_2_1;
			float f2 = r0_1_1 + r1_1_1;
			float f3 = r2_1_1 + r1_0_1;

			float r1 = f1 + f2;
			float r2=  f3 + r1_1_0;

			float result  = r1 + r2;
			bool change_cond = (index <= offset_x || index > sizex || (i <= 1) || (i >= limit_z -1) || (y_index <= 0) || (y_index >= grid_sizey -1));
			mem_wr[q] =  change_cond ? s_1_1_1_arr[q+1] : result;
		}



		array2vec: for(int k = 0; k < PORT_WIDTH/2; k++){
			#pragma HLS loop_tripcount min=port_width max=port_width avg=port_width
			data_conv tmp_l, tmp_u;
			tmp_l.f = mem_wr[k];
			update_j_l.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp_l.i;
			tmp_u.f = mem_wr[k+PORT_WIDTH/2];
			update_j_u.range(DATATYPE_SIZE * (k + 1) - 1, k * DATATYPE_SIZE) = tmp_u.i;
		}

		bool cond_wr = (i >= 1) && ( i <= limit_z);
		if(cond_wr ) {
			wr_buffer0 << update_j_l;
			wr_buffer1 << update_j_u;
		}

		// move the cell block
		k_dum++;
	}
}
