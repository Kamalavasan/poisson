__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_populate(
		const int arg0,
		const int arg1,
		struct bram_cluster u,
		struct bram_cluster ref,
		const float dx,
		const float dy,
		const int base3,
		const int base4,
		const int base5,
		int arg_idx0, int arg_idx1,
		const int size0,
		const int size1){


		v1_rd: __attribute__((xcl_pipeline_loop))
		for(int i  = 0; i < size1; i++){

			int arg_idx[2];
			double x, y;
			int end_loc = (size0 >> 6) + 1;
			int base_index = base0  + i * row_block;

			arg_idx[1] = arg_idx1 + i;

			__attribute__((xcl_pipeline_loop))
			__attribute__((opencl_unroll_hint(P_FACTOR)))
			for(int j = 0; j < end_loc ; j++){

										\\------------COMPUTE UNIT 0  ----------------
					int index0 = j * 64 + 0;
					double arg_idx_0 = arg_idx0 + index0;

					double x_0  = dx * (double)(arg_idx[0]+arg0);
					double y_0 = dy * (double)(arg_idx_0 +arg1);

					float f3_0 = myfun(native_sin(M_PI*x_0),native_cos(2.0*M_PI*y_0))-1.0;
					float f5_0 = native_sin(M_PI*x_0)*native_cos(2.0*M_PI*y_0);

					if(index0  < size0){
						u.u0[base_index +j] = f3_0;
						ref.u0[base_index +j] = f5_0;
					}

					\\------------COMPUTE UNIT 1  ----------------
					int index1 = j * 64 + 1;
					double arg_idx_1 = arg_idx0 + index1;

					double x_1  = dx * (double)(arg_idx[0]+arg0);
					double y_1 = dy * (double)(arg_idx_1 +arg1);

					float f3_1 = myfun(native_sin(M_PI*x_1),native_cos(2.0*M_PI*y_1))-1.0;
					float f5_1 = native_sin(M_PI*x_1)*native_cos(2.0*M_PI*y_1);

					if(index1  < size0){
						u.u1[base_index +j] = f3_1;
						ref.u1[base_index +j] = f5_1;
					}

					\\------------COMPUTE UNIT 2  ----------------
					int index2 = j * 64 + 2;
					double arg_idx_2 = arg_idx0 + index2;

					double x_2  = dx * (double)(arg_idx[0]+arg0);
					double y_2 = dy * (double)(arg_idx_2 +arg1);

					float f3_2 = myfun(native_sin(M_PI*x_2),native_cos(2.0*M_PI*y_2))-1.0;
					float f5_2 = native_sin(M_PI*x_2)*native_cos(2.0*M_PI*y_2);

					if(index2  < size0){
						u.u2[base_index +j] = f3_2;
						ref.u2[base_index +j] = f5_2;
					}

					\\------------COMPUTE UNIT 3  ----------------
					int index3 = j * 64 + 3;
					double arg_idx_3 = arg_idx0 + index3;

					double x_3  = dx * (double)(arg_idx[0]+arg0);
					double y_3 = dy * (double)(arg_idx_3 +arg1);

					float f3_3 = myfun(native_sin(M_PI*x_3),native_cos(2.0*M_PI*y_3))-1.0;
					float f5_3 = native_sin(M_PI*x_3)*native_cos(2.0*M_PI*y_3);

					if(index3  < size0){
						u.u3[base_index +j] = f3_3;
						ref.u3[base_index +j] = f5_3;
					}

					\\------------COMPUTE UNIT 4  ----------------
					int index4 = j * 64 + 4;
					double arg_idx_4 = arg_idx0 + index4;

					double x_4  = dx * (double)(arg_idx[0]+arg0);
					double y_4 = dy * (double)(arg_idx_4 +arg1);

					float f3_4 = myfun(native_sin(M_PI*x_4),native_cos(2.0*M_PI*y_4))-1.0;
					float f5_4 = native_sin(M_PI*x_4)*native_cos(2.0*M_PI*y_4);

					if(index4  < size0){
						u.u4[base_index +j] = f3_4;
						ref.u4[base_index +j] = f5_4;
					}

					\\------------COMPUTE UNIT 5  ----------------
					int index5 = j * 64 + 5;
					double arg_idx_5 = arg_idx0 + index5;

					double x_5  = dx * (double)(arg_idx[0]+arg0);
					double y_5 = dy * (double)(arg_idx_5 +arg1);

					float f3_5 = myfun(native_sin(M_PI*x_5),native_cos(2.0*M_PI*y_5))-1.0;
					float f5_5 = native_sin(M_PI*x_5)*native_cos(2.0*M_PI*y_5);

					if(index5  < size0){
						u.u5[base_index +j] = f3_5;
						ref.u5[base_index +j] = f5_5;
					}

					\\------------COMPUTE UNIT 6  ----------------
					int index6 = j * 64 + 6;
					double arg_idx_6 = arg_idx0 + index6;

					double x_6  = dx * (double)(arg_idx[0]+arg0);
					double y_6 = dy * (double)(arg_idx_6 +arg1);

					float f3_6 = myfun(native_sin(M_PI*x_6),native_cos(2.0*M_PI*y_6))-1.0;
					float f5_6 = native_sin(M_PI*x_6)*native_cos(2.0*M_PI*y_6);

					if(index6  < size0){
						u.u6[base_index +j] = f3_6;
						ref.u6[base_index +j] = f5_6;
					}

					\\------------COMPUTE UNIT 7  ----------------
					int index7 = j * 64 + 7;
					double arg_idx_7 = arg_idx0 + index7;

					double x_7  = dx * (double)(arg_idx[0]+arg0);
					double y_7 = dy * (double)(arg_idx_7 +arg1);

					float f3_7 = myfun(native_sin(M_PI*x_7),native_cos(2.0*M_PI*y_7))-1.0;
					float f5_7 = native_sin(M_PI*x_7)*native_cos(2.0*M_PI*y_7);

					if(index7  < size0){
						u.u7[base_index +j] = f3_7;
						ref.u7[base_index +j] = f5_7;
					}

					\\------------COMPUTE UNIT 8  ----------------
					int index8 = j * 64 + 8;
					double arg_idx_8 = arg_idx0 + index8;

					double x_8  = dx * (double)(arg_idx[0]+arg0);
					double y_8 = dy * (double)(arg_idx_8 +arg1);

					float f3_8 = myfun(native_sin(M_PI*x_8),native_cos(2.0*M_PI*y_8))-1.0;
					float f5_8 = native_sin(M_PI*x_8)*native_cos(2.0*M_PI*y_8);

					if(index8  < size0){
						u.u8[base_index +j] = f3_8;
						ref.u8[base_index +j] = f5_8;
					}

					\\------------COMPUTE UNIT 9  ----------------
					int index9 = j * 64 + 9;
					double arg_idx_9 = arg_idx0 + index9;

					double x_9  = dx * (double)(arg_idx[0]+arg0);
					double y_9 = dy * (double)(arg_idx_9 +arg1);

					float f3_9 = myfun(native_sin(M_PI*x_9),native_cos(2.0*M_PI*y_9))-1.0;
					float f5_9 = native_sin(M_PI*x_9)*native_cos(2.0*M_PI*y_9);

					if(index9  < size0){
						u.u9[base_index +j] = f3_9;
						ref.u9[base_index +j] = f5_9;
					}

					\\------------COMPUTE UNIT 10  ----------------
					int index10 = j * 64 + 10;
					double arg_idx_10 = arg_idx0 + index10;

					double x_10  = dx * (double)(arg_idx[0]+arg0);
					double y_10 = dy * (double)(arg_idx_10 +arg1);

					float f3_10 = myfun(native_sin(M_PI*x_10),native_cos(2.0*M_PI*y_10))-1.0;
					float f5_10 = native_sin(M_PI*x_10)*native_cos(2.0*M_PI*y_10);

					if(index10  < size0){
						u.u10[base_index +j] = f3_10;
						ref.u10[base_index +j] = f5_10;
					}

					\\------------COMPUTE UNIT 11  ----------------
					int index11 = j * 64 + 11;
					double arg_idx_11 = arg_idx0 + index11;

					double x_11  = dx * (double)(arg_idx[0]+arg0);
					double y_11 = dy * (double)(arg_idx_11 +arg1);

					float f3_11 = myfun(native_sin(M_PI*x_11),native_cos(2.0*M_PI*y_11))-1.0;
					float f5_11 = native_sin(M_PI*x_11)*native_cos(2.0*M_PI*y_11);

					if(index11  < size0){
						u.u11[base_index +j] = f3_11;
						ref.u11[base_index +j] = f5_11;
					}

					\\------------COMPUTE UNIT 12  ----------------
					int index12 = j * 64 + 12;
					double arg_idx_12 = arg_idx0 + index12;

					double x_12  = dx * (double)(arg_idx[0]+arg0);
					double y_12 = dy * (double)(arg_idx_12 +arg1);

					float f3_12 = myfun(native_sin(M_PI*x_12),native_cos(2.0*M_PI*y_12))-1.0;
					float f5_12 = native_sin(M_PI*x_12)*native_cos(2.0*M_PI*y_12);

					if(index12  < size0){
						u.u12[base_index +j] = f3_12;
						ref.u12[base_index +j] = f5_12;
					}

					\\------------COMPUTE UNIT 13  ----------------
					int index13 = j * 64 + 13;
					double arg_idx_13 = arg_idx0 + index13;

					double x_13  = dx * (double)(arg_idx[0]+arg0);
					double y_13 = dy * (double)(arg_idx_13 +arg1);

					float f3_13 = myfun(native_sin(M_PI*x_13),native_cos(2.0*M_PI*y_13))-1.0;
					float f5_13 = native_sin(M_PI*x_13)*native_cos(2.0*M_PI*y_13);

					if(index13  < size0){
						u.u13[base_index +j] = f3_13;
						ref.u13[base_index +j] = f5_13;
					}

					\\------------COMPUTE UNIT 14  ----------------
					int index14 = j * 64 + 14;
					double arg_idx_14 = arg_idx0 + index14;

					double x_14  = dx * (double)(arg_idx[0]+arg0);
					double y_14 = dy * (double)(arg_idx_14 +arg1);

					float f3_14 = myfun(native_sin(M_PI*x_14),native_cos(2.0*M_PI*y_14))-1.0;
					float f5_14 = native_sin(M_PI*x_14)*native_cos(2.0*M_PI*y_14);

					if(index14  < size0){
						u.u14[base_index +j] = f3_14;
						ref.u14[base_index +j] = f5_14;
					}

					\\------------COMPUTE UNIT 15  ----------------
					int index15 = j * 64 + 15;
					double arg_idx_15 = arg_idx0 + index15;

					double x_15  = dx * (double)(arg_idx[0]+arg0);
					double y_15 = dy * (double)(arg_idx_15 +arg1);

					float f3_15 = myfun(native_sin(M_PI*x_15),native_cos(2.0*M_PI*y_15))-1.0;
					float f5_15 = native_sin(M_PI*x_15)*native_cos(2.0*M_PI*y_15);

					if(index15  < size0){
						u.u15[base_index +j] = f3_15;
						ref.u15[base_index +j] = f5_15;
					}

					\\------------COMPUTE UNIT 16  ----------------
					int index16 = j * 64 + 16;
					double arg_idx_16 = arg_idx0 + index16;

					double x_16  = dx * (double)(arg_idx[0]+arg0);
					double y_16 = dy * (double)(arg_idx_16 +arg1);

					float f3_16 = myfun(native_sin(M_PI*x_16),native_cos(2.0*M_PI*y_16))-1.0;
					float f5_16 = native_sin(M_PI*x_16)*native_cos(2.0*M_PI*y_16);

					if(index16  < size0){
						u.u16[base_index +j] = f3_16;
						ref.u16[base_index +j] = f5_16;
					}

					\\------------COMPUTE UNIT 17  ----------------
					int index17 = j * 64 + 17;
					double arg_idx_17 = arg_idx0 + index17;

					double x_17  = dx * (double)(arg_idx[0]+arg0);
					double y_17 = dy * (double)(arg_idx_17 +arg1);

					float f3_17 = myfun(native_sin(M_PI*x_17),native_cos(2.0*M_PI*y_17))-1.0;
					float f5_17 = native_sin(M_PI*x_17)*native_cos(2.0*M_PI*y_17);

					if(index17  < size0){
						u.u17[base_index +j] = f3_17;
						ref.u17[base_index +j] = f5_17;
					}

					\\------------COMPUTE UNIT 18  ----------------
					int index18 = j * 64 + 18;
					double arg_idx_18 = arg_idx0 + index18;

					double x_18  = dx * (double)(arg_idx[0]+arg0);
					double y_18 = dy * (double)(arg_idx_18 +arg1);

					float f3_18 = myfun(native_sin(M_PI*x_18),native_cos(2.0*M_PI*y_18))-1.0;
					float f5_18 = native_sin(M_PI*x_18)*native_cos(2.0*M_PI*y_18);

					if(index18  < size0){
						u.u18[base_index +j] = f3_18;
						ref.u18[base_index +j] = f5_18;
					}

					\\------------COMPUTE UNIT 19  ----------------
					int index19 = j * 64 + 19;
					double arg_idx_19 = arg_idx0 + index19;

					double x_19  = dx * (double)(arg_idx[0]+arg0);
					double y_19 = dy * (double)(arg_idx_19 +arg1);

					float f3_19 = myfun(native_sin(M_PI*x_19),native_cos(2.0*M_PI*y_19))-1.0;
					float f5_19 = native_sin(M_PI*x_19)*native_cos(2.0*M_PI*y_19);

					if(index19  < size0){
						u.u19[base_index +j] = f3_19;
						ref.u19[base_index +j] = f5_19;
					}

					\\------------COMPUTE UNIT 20  ----------------
					int index20 = j * 64 + 20;
					double arg_idx_20 = arg_idx0 + index20;

					double x_20  = dx * (double)(arg_idx[0]+arg0);
					double y_20 = dy * (double)(arg_idx_20 +arg1);

					float f3_20 = myfun(native_sin(M_PI*x_20),native_cos(2.0*M_PI*y_20))-1.0;
					float f5_20 = native_sin(M_PI*x_20)*native_cos(2.0*M_PI*y_20);

					if(index20  < size0){
						u.u20[base_index +j] = f3_20;
						ref.u20[base_index +j] = f5_20;
					}

					\\------------COMPUTE UNIT 21  ----------------
					int index21 = j * 64 + 21;
					double arg_idx_21 = arg_idx0 + index21;

					double x_21  = dx * (double)(arg_idx[0]+arg0);
					double y_21 = dy * (double)(arg_idx_21 +arg1);

					float f3_21 = myfun(native_sin(M_PI*x_21),native_cos(2.0*M_PI*y_21))-1.0;
					float f5_21 = native_sin(M_PI*x_21)*native_cos(2.0*M_PI*y_21);

					if(index21  < size0){
						u.u21[base_index +j] = f3_21;
						ref.u21[base_index +j] = f5_21;
					}

					\\------------COMPUTE UNIT 22  ----------------
					int index22 = j * 64 + 22;
					double arg_idx_22 = arg_idx0 + index22;

					double x_22  = dx * (double)(arg_idx[0]+arg0);
					double y_22 = dy * (double)(arg_idx_22 +arg1);

					float f3_22 = myfun(native_sin(M_PI*x_22),native_cos(2.0*M_PI*y_22))-1.0;
					float f5_22 = native_sin(M_PI*x_22)*native_cos(2.0*M_PI*y_22);

					if(index22  < size0){
						u.u22[base_index +j] = f3_22;
						ref.u22[base_index +j] = f5_22;
					}

					\\------------COMPUTE UNIT 23  ----------------
					int index23 = j * 64 + 23;
					double arg_idx_23 = arg_idx0 + index23;

					double x_23  = dx * (double)(arg_idx[0]+arg0);
					double y_23 = dy * (double)(arg_idx_23 +arg1);

					float f3_23 = myfun(native_sin(M_PI*x_23),native_cos(2.0*M_PI*y_23))-1.0;
					float f5_23 = native_sin(M_PI*x_23)*native_cos(2.0*M_PI*y_23);

					if(index23  < size0){
						u.u23[base_index +j] = f3_23;
						ref.u23[base_index +j] = f5_23;
					}

					\\------------COMPUTE UNIT 24  ----------------
					int index24 = j * 64 + 24;
					double arg_idx_24 = arg_idx0 + index24;

					double x_24  = dx * (double)(arg_idx[0]+arg0);
					double y_24 = dy * (double)(arg_idx_24 +arg1);

					float f3_24 = myfun(native_sin(M_PI*x_24),native_cos(2.0*M_PI*y_24))-1.0;
					float f5_24 = native_sin(M_PI*x_24)*native_cos(2.0*M_PI*y_24);

					if(index24  < size0){
						u.u24[base_index +j] = f3_24;
						ref.u24[base_index +j] = f5_24;
					}

					\\------------COMPUTE UNIT 25  ----------------
					int index25 = j * 64 + 25;
					double arg_idx_25 = arg_idx0 + index25;

					double x_25  = dx * (double)(arg_idx[0]+arg0);
					double y_25 = dy * (double)(arg_idx_25 +arg1);

					float f3_25 = myfun(native_sin(M_PI*x_25),native_cos(2.0*M_PI*y_25))-1.0;
					float f5_25 = native_sin(M_PI*x_25)*native_cos(2.0*M_PI*y_25);

					if(index25  < size0){
						u.u25[base_index +j] = f3_25;
						ref.u25[base_index +j] = f5_25;
					}

					\\------------COMPUTE UNIT 26  ----------------
					int index26 = j * 64 + 26;
					double arg_idx_26 = arg_idx0 + index26;

					double x_26  = dx * (double)(arg_idx[0]+arg0);
					double y_26 = dy * (double)(arg_idx_26 +arg1);

					float f3_26 = myfun(native_sin(M_PI*x_26),native_cos(2.0*M_PI*y_26))-1.0;
					float f5_26 = native_sin(M_PI*x_26)*native_cos(2.0*M_PI*y_26);

					if(index26  < size0){
						u.u26[base_index +j] = f3_26;
						ref.u26[base_index +j] = f5_26;
					}

					\\------------COMPUTE UNIT 27  ----------------
					int index27 = j * 64 + 27;
					double arg_idx_27 = arg_idx0 + index27;

					double x_27  = dx * (double)(arg_idx[0]+arg0);
					double y_27 = dy * (double)(arg_idx_27 +arg1);

					float f3_27 = myfun(native_sin(M_PI*x_27),native_cos(2.0*M_PI*y_27))-1.0;
					float f5_27 = native_sin(M_PI*x_27)*native_cos(2.0*M_PI*y_27);

					if(index27  < size0){
						u.u27[base_index +j] = f3_27;
						ref.u27[base_index +j] = f5_27;
					}

					\\------------COMPUTE UNIT 28  ----------------
					int index28 = j * 64 + 28;
					double arg_idx_28 = arg_idx0 + index28;

					double x_28  = dx * (double)(arg_idx[0]+arg0);
					double y_28 = dy * (double)(arg_idx_28 +arg1);

					float f3_28 = myfun(native_sin(M_PI*x_28),native_cos(2.0*M_PI*y_28))-1.0;
					float f5_28 = native_sin(M_PI*x_28)*native_cos(2.0*M_PI*y_28);

					if(index28  < size0){
						u.u28[base_index +j] = f3_28;
						ref.u28[base_index +j] = f5_28;
					}

					\\------------COMPUTE UNIT 29  ----------------
					int index29 = j * 64 + 29;
					double arg_idx_29 = arg_idx0 + index29;

					double x_29  = dx * (double)(arg_idx[0]+arg0);
					double y_29 = dy * (double)(arg_idx_29 +arg1);

					float f3_29 = myfun(native_sin(M_PI*x_29),native_cos(2.0*M_PI*y_29))-1.0;
					float f5_29 = native_sin(M_PI*x_29)*native_cos(2.0*M_PI*y_29);

					if(index29  < size0){
						u.u29[base_index +j] = f3_29;
						ref.u29[base_index +j] = f5_29;
					}

					\\------------COMPUTE UNIT 30  ----------------
					int index30 = j * 64 + 30;
					double arg_idx_30 = arg_idx0 + index30;

					double x_30  = dx * (double)(arg_idx[0]+arg0);
					double y_30 = dy * (double)(arg_idx_30 +arg1);

					float f3_30 = myfun(native_sin(M_PI*x_30),native_cos(2.0*M_PI*y_30))-1.0;
					float f5_30 = native_sin(M_PI*x_30)*native_cos(2.0*M_PI*y_30);

					if(index30  < size0){
						u.u30[base_index +j] = f3_30;
						ref.u30[base_index +j] = f5_30;
					}

					\\------------COMPUTE UNIT 31  ----------------
					int index31 = j * 64 + 31;
					double arg_idx_31 = arg_idx0 + index31;

					double x_31  = dx * (double)(arg_idx[0]+arg0);
					double y_31 = dy * (double)(arg_idx_31 +arg1);

					float f3_31 = myfun(native_sin(M_PI*x_31),native_cos(2.0*M_PI*y_31))-1.0;
					float f5_31 = native_sin(M_PI*x_31)*native_cos(2.0*M_PI*y_31);

					if(index31  < size0){
						u.u31[base_index +j] = f3_31;
						ref.u31[base_index +j] = f5_31;
					}

					\\------------COMPUTE UNIT 32  ----------------
					int index32 = j * 64 + 32;
					double arg_idx_32 = arg_idx0 + index32;

					double x_32  = dx * (double)(arg_idx[0]+arg0);
					double y_32 = dy * (double)(arg_idx_32 +arg1);

					float f3_32 = myfun(native_sin(M_PI*x_32),native_cos(2.0*M_PI*y_32))-1.0;
					float f5_32 = native_sin(M_PI*x_32)*native_cos(2.0*M_PI*y_32);

					if(index32  < size0){
						u.u32[base_index +j] = f3_32;
						ref.u32[base_index +j] = f5_32;
					}

					\\------------COMPUTE UNIT 33  ----------------
					int index33 = j * 64 + 33;
					double arg_idx_33 = arg_idx0 + index33;

					double x_33  = dx * (double)(arg_idx[0]+arg0);
					double y_33 = dy * (double)(arg_idx_33 +arg1);

					float f3_33 = myfun(native_sin(M_PI*x_33),native_cos(2.0*M_PI*y_33))-1.0;
					float f5_33 = native_sin(M_PI*x_33)*native_cos(2.0*M_PI*y_33);

					if(index33  < size0){
						u.u33[base_index +j] = f3_33;
						ref.u33[base_index +j] = f5_33;
					}

					\\------------COMPUTE UNIT 34  ----------------
					int index34 = j * 64 + 34;
					double arg_idx_34 = arg_idx0 + index34;

					double x_34  = dx * (double)(arg_idx[0]+arg0);
					double y_34 = dy * (double)(arg_idx_34 +arg1);

					float f3_34 = myfun(native_sin(M_PI*x_34),native_cos(2.0*M_PI*y_34))-1.0;
					float f5_34 = native_sin(M_PI*x_34)*native_cos(2.0*M_PI*y_34);

					if(index34  < size0){
						u.u34[base_index +j] = f3_34;
						ref.u34[base_index +j] = f5_34;
					}

					\\------------COMPUTE UNIT 35  ----------------
					int index35 = j * 64 + 35;
					double arg_idx_35 = arg_idx0 + index35;

					double x_35  = dx * (double)(arg_idx[0]+arg0);
					double y_35 = dy * (double)(arg_idx_35 +arg1);

					float f3_35 = myfun(native_sin(M_PI*x_35),native_cos(2.0*M_PI*y_35))-1.0;
					float f5_35 = native_sin(M_PI*x_35)*native_cos(2.0*M_PI*y_35);

					if(index35  < size0){
						u.u35[base_index +j] = f3_35;
						ref.u35[base_index +j] = f5_35;
					}

					\\------------COMPUTE UNIT 36  ----------------
					int index36 = j * 64 + 36;
					double arg_idx_36 = arg_idx0 + index36;

					double x_36  = dx * (double)(arg_idx[0]+arg0);
					double y_36 = dy * (double)(arg_idx_36 +arg1);

					float f3_36 = myfun(native_sin(M_PI*x_36),native_cos(2.0*M_PI*y_36))-1.0;
					float f5_36 = native_sin(M_PI*x_36)*native_cos(2.0*M_PI*y_36);

					if(index36  < size0){
						u.u36[base_index +j] = f3_36;
						ref.u36[base_index +j] = f5_36;
					}

					\\------------COMPUTE UNIT 37  ----------------
					int index37 = j * 64 + 37;
					double arg_idx_37 = arg_idx0 + index37;

					double x_37  = dx * (double)(arg_idx[0]+arg0);
					double y_37 = dy * (double)(arg_idx_37 +arg1);

					float f3_37 = myfun(native_sin(M_PI*x_37),native_cos(2.0*M_PI*y_37))-1.0;
					float f5_37 = native_sin(M_PI*x_37)*native_cos(2.0*M_PI*y_37);

					if(index37  < size0){
						u.u37[base_index +j] = f3_37;
						ref.u37[base_index +j] = f5_37;
					}

					\\------------COMPUTE UNIT 38  ----------------
					int index38 = j * 64 + 38;
					double arg_idx_38 = arg_idx0 + index38;

					double x_38  = dx * (double)(arg_idx[0]+arg0);
					double y_38 = dy * (double)(arg_idx_38 +arg1);

					float f3_38 = myfun(native_sin(M_PI*x_38),native_cos(2.0*M_PI*y_38))-1.0;
					float f5_38 = native_sin(M_PI*x_38)*native_cos(2.0*M_PI*y_38);

					if(index38  < size0){
						u.u38[base_index +j] = f3_38;
						ref.u38[base_index +j] = f5_38;
					}

					\\------------COMPUTE UNIT 39  ----------------
					int index39 = j * 64 + 39;
					double arg_idx_39 = arg_idx0 + index39;

					double x_39  = dx * (double)(arg_idx[0]+arg0);
					double y_39 = dy * (double)(arg_idx_39 +arg1);

					float f3_39 = myfun(native_sin(M_PI*x_39),native_cos(2.0*M_PI*y_39))-1.0;
					float f5_39 = native_sin(M_PI*x_39)*native_cos(2.0*M_PI*y_39);

					if(index39  < size0){
						u.u39[base_index +j] = f3_39;
						ref.u39[base_index +j] = f5_39;
					}

					\\------------COMPUTE UNIT 40  ----------------
					int index40 = j * 64 + 40;
					double arg_idx_40 = arg_idx0 + index40;

					double x_40  = dx * (double)(arg_idx[0]+arg0);
					double y_40 = dy * (double)(arg_idx_40 +arg1);

					float f3_40 = myfun(native_sin(M_PI*x_40),native_cos(2.0*M_PI*y_40))-1.0;
					float f5_40 = native_sin(M_PI*x_40)*native_cos(2.0*M_PI*y_40);

					if(index40  < size0){
						u.u40[base_index +j] = f3_40;
						ref.u40[base_index +j] = f5_40;
					}

					\\------------COMPUTE UNIT 41  ----------------
					int index41 = j * 64 + 41;
					double arg_idx_41 = arg_idx0 + index41;

					double x_41  = dx * (double)(arg_idx[0]+arg0);
					double y_41 = dy * (double)(arg_idx_41 +arg1);

					float f3_41 = myfun(native_sin(M_PI*x_41),native_cos(2.0*M_PI*y_41))-1.0;
					float f5_41 = native_sin(M_PI*x_41)*native_cos(2.0*M_PI*y_41);

					if(index41  < size0){
						u.u41[base_index +j] = f3_41;
						ref.u41[base_index +j] = f5_41;
					}

					\\------------COMPUTE UNIT 42  ----------------
					int index42 = j * 64 + 42;
					double arg_idx_42 = arg_idx0 + index42;

					double x_42  = dx * (double)(arg_idx[0]+arg0);
					double y_42 = dy * (double)(arg_idx_42 +arg1);

					float f3_42 = myfun(native_sin(M_PI*x_42),native_cos(2.0*M_PI*y_42))-1.0;
					float f5_42 = native_sin(M_PI*x_42)*native_cos(2.0*M_PI*y_42);

					if(index42  < size0){
						u.u42[base_index +j] = f3_42;
						ref.u42[base_index +j] = f5_42;
					}

					\\------------COMPUTE UNIT 43  ----------------
					int index43 = j * 64 + 43;
					double arg_idx_43 = arg_idx0 + index43;

					double x_43  = dx * (double)(arg_idx[0]+arg0);
					double y_43 = dy * (double)(arg_idx_43 +arg1);

					float f3_43 = myfun(native_sin(M_PI*x_43),native_cos(2.0*M_PI*y_43))-1.0;
					float f5_43 = native_sin(M_PI*x_43)*native_cos(2.0*M_PI*y_43);

					if(index43  < size0){
						u.u43[base_index +j] = f3_43;
						ref.u43[base_index +j] = f5_43;
					}

					\\------------COMPUTE UNIT 44  ----------------
					int index44 = j * 64 + 44;
					double arg_idx_44 = arg_idx0 + index44;

					double x_44  = dx * (double)(arg_idx[0]+arg0);
					double y_44 = dy * (double)(arg_idx_44 +arg1);

					float f3_44 = myfun(native_sin(M_PI*x_44),native_cos(2.0*M_PI*y_44))-1.0;
					float f5_44 = native_sin(M_PI*x_44)*native_cos(2.0*M_PI*y_44);

					if(index44  < size0){
						u.u44[base_index +j] = f3_44;
						ref.u44[base_index +j] = f5_44;
					}

					\\------------COMPUTE UNIT 45  ----------------
					int index45 = j * 64 + 45;
					double arg_idx_45 = arg_idx0 + index45;

					double x_45  = dx * (double)(arg_idx[0]+arg0);
					double y_45 = dy * (double)(arg_idx_45 +arg1);

					float f3_45 = myfun(native_sin(M_PI*x_45),native_cos(2.0*M_PI*y_45))-1.0;
					float f5_45 = native_sin(M_PI*x_45)*native_cos(2.0*M_PI*y_45);

					if(index45  < size0){
						u.u45[base_index +j] = f3_45;
						ref.u45[base_index +j] = f5_45;
					}

					\\------------COMPUTE UNIT 46  ----------------
					int index46 = j * 64 + 46;
					double arg_idx_46 = arg_idx0 + index46;

					double x_46  = dx * (double)(arg_idx[0]+arg0);
					double y_46 = dy * (double)(arg_idx_46 +arg1);

					float f3_46 = myfun(native_sin(M_PI*x_46),native_cos(2.0*M_PI*y_46))-1.0;
					float f5_46 = native_sin(M_PI*x_46)*native_cos(2.0*M_PI*y_46);

					if(index46  < size0){
						u.u46[base_index +j] = f3_46;
						ref.u46[base_index +j] = f5_46;
					}

					\\------------COMPUTE UNIT 47  ----------------
					int index47 = j * 64 + 47;
					double arg_idx_47 = arg_idx0 + index47;

					double x_47  = dx * (double)(arg_idx[0]+arg0);
					double y_47 = dy * (double)(arg_idx_47 +arg1);

					float f3_47 = myfun(native_sin(M_PI*x_47),native_cos(2.0*M_PI*y_47))-1.0;
					float f5_47 = native_sin(M_PI*x_47)*native_cos(2.0*M_PI*y_47);

					if(index47  < size0){
						u.u47[base_index +j] = f3_47;
						ref.u47[base_index +j] = f5_47;
					}

					\\------------COMPUTE UNIT 48  ----------------
					int index48 = j * 64 + 48;
					double arg_idx_48 = arg_idx0 + index48;

					double x_48  = dx * (double)(arg_idx[0]+arg0);
					double y_48 = dy * (double)(arg_idx_48 +arg1);

					float f3_48 = myfun(native_sin(M_PI*x_48),native_cos(2.0*M_PI*y_48))-1.0;
					float f5_48 = native_sin(M_PI*x_48)*native_cos(2.0*M_PI*y_48);

					if(index48  < size0){
						u.u48[base_index +j] = f3_48;
						ref.u48[base_index +j] = f5_48;
					}

					\\------------COMPUTE UNIT 49  ----------------
					int index49 = j * 64 + 49;
					double arg_idx_49 = arg_idx0 + index49;

					double x_49  = dx * (double)(arg_idx[0]+arg0);
					double y_49 = dy * (double)(arg_idx_49 +arg1);

					float f3_49 = myfun(native_sin(M_PI*x_49),native_cos(2.0*M_PI*y_49))-1.0;
					float f5_49 = native_sin(M_PI*x_49)*native_cos(2.0*M_PI*y_49);

					if(index49  < size0){
						u.u49[base_index +j] = f3_49;
						ref.u49[base_index +j] = f5_49;
					}

					\\------------COMPUTE UNIT 50  ----------------
					int index50 = j * 64 + 50;
					double arg_idx_50 = arg_idx0 + index50;

					double x_50  = dx * (double)(arg_idx[0]+arg0);
					double y_50 = dy * (double)(arg_idx_50 +arg1);

					float f3_50 = myfun(native_sin(M_PI*x_50),native_cos(2.0*M_PI*y_50))-1.0;
					float f5_50 = native_sin(M_PI*x_50)*native_cos(2.0*M_PI*y_50);

					if(index50  < size0){
						u.u50[base_index +j] = f3_50;
						ref.u50[base_index +j] = f5_50;
					}

					\\------------COMPUTE UNIT 51  ----------------
					int index51 = j * 64 + 51;
					double arg_idx_51 = arg_idx0 + index51;

					double x_51  = dx * (double)(arg_idx[0]+arg0);
					double y_51 = dy * (double)(arg_idx_51 +arg1);

					float f3_51 = myfun(native_sin(M_PI*x_51),native_cos(2.0*M_PI*y_51))-1.0;
					float f5_51 = native_sin(M_PI*x_51)*native_cos(2.0*M_PI*y_51);

					if(index51  < size0){
						u.u51[base_index +j] = f3_51;
						ref.u51[base_index +j] = f5_51;
					}

					\\------------COMPUTE UNIT 52  ----------------
					int index52 = j * 64 + 52;
					double arg_idx_52 = arg_idx0 + index52;

					double x_52  = dx * (double)(arg_idx[0]+arg0);
					double y_52 = dy * (double)(arg_idx_52 +arg1);

					float f3_52 = myfun(native_sin(M_PI*x_52),native_cos(2.0*M_PI*y_52))-1.0;
					float f5_52 = native_sin(M_PI*x_52)*native_cos(2.0*M_PI*y_52);

					if(index52  < size0){
						u.u52[base_index +j] = f3_52;
						ref.u52[base_index +j] = f5_52;
					}

					\\------------COMPUTE UNIT 53  ----------------
					int index53 = j * 64 + 53;
					double arg_idx_53 = arg_idx0 + index53;

					double x_53  = dx * (double)(arg_idx[0]+arg0);
					double y_53 = dy * (double)(arg_idx_53 +arg1);

					float f3_53 = myfun(native_sin(M_PI*x_53),native_cos(2.0*M_PI*y_53))-1.0;
					float f5_53 = native_sin(M_PI*x_53)*native_cos(2.0*M_PI*y_53);

					if(index53  < size0){
						u.u53[base_index +j] = f3_53;
						ref.u53[base_index +j] = f5_53;
					}

					\\------------COMPUTE UNIT 54  ----------------
					int index54 = j * 64 + 54;
					double arg_idx_54 = arg_idx0 + index54;

					double x_54  = dx * (double)(arg_idx[0]+arg0);
					double y_54 = dy * (double)(arg_idx_54 +arg1);

					float f3_54 = myfun(native_sin(M_PI*x_54),native_cos(2.0*M_PI*y_54))-1.0;
					float f5_54 = native_sin(M_PI*x_54)*native_cos(2.0*M_PI*y_54);

					if(index54  < size0){
						u.u54[base_index +j] = f3_54;
						ref.u54[base_index +j] = f5_54;
					}

					\\------------COMPUTE UNIT 55  ----------------
					int index55 = j * 64 + 55;
					double arg_idx_55 = arg_idx0 + index55;

					double x_55  = dx * (double)(arg_idx[0]+arg0);
					double y_55 = dy * (double)(arg_idx_55 +arg1);

					float f3_55 = myfun(native_sin(M_PI*x_55),native_cos(2.0*M_PI*y_55))-1.0;
					float f5_55 = native_sin(M_PI*x_55)*native_cos(2.0*M_PI*y_55);

					if(index55  < size0){
						u.u55[base_index +j] = f3_55;
						ref.u55[base_index +j] = f5_55;
					}

					\\------------COMPUTE UNIT 56  ----------------
					int index56 = j * 64 + 56;
					double arg_idx_56 = arg_idx0 + index56;

					double x_56  = dx * (double)(arg_idx[0]+arg0);
					double y_56 = dy * (double)(arg_idx_56 +arg1);

					float f3_56 = myfun(native_sin(M_PI*x_56),native_cos(2.0*M_PI*y_56))-1.0;
					float f5_56 = native_sin(M_PI*x_56)*native_cos(2.0*M_PI*y_56);

					if(index56  < size0){
						u.u56[base_index +j] = f3_56;
						ref.u56[base_index +j] = f5_56;
					}

					\\------------COMPUTE UNIT 57  ----------------
					int index57 = j * 64 + 57;
					double arg_idx_57 = arg_idx0 + index57;

					double x_57  = dx * (double)(arg_idx[0]+arg0);
					double y_57 = dy * (double)(arg_idx_57 +arg1);

					float f3_57 = myfun(native_sin(M_PI*x_57),native_cos(2.0*M_PI*y_57))-1.0;
					float f5_57 = native_sin(M_PI*x_57)*native_cos(2.0*M_PI*y_57);

					if(index57  < size0){
						u.u57[base_index +j] = f3_57;
						ref.u57[base_index +j] = f5_57;
					}

					\\------------COMPUTE UNIT 58  ----------------
					int index58 = j * 64 + 58;
					double arg_idx_58 = arg_idx0 + index58;

					double x_58  = dx * (double)(arg_idx[0]+arg0);
					double y_58 = dy * (double)(arg_idx_58 +arg1);

					float f3_58 = myfun(native_sin(M_PI*x_58),native_cos(2.0*M_PI*y_58))-1.0;
					float f5_58 = native_sin(M_PI*x_58)*native_cos(2.0*M_PI*y_58);

					if(index58  < size0){
						u.u58[base_index +j] = f3_58;
						ref.u58[base_index +j] = f5_58;
					}

					\\------------COMPUTE UNIT 59  ----------------
					int index59 = j * 64 + 59;
					double arg_idx_59 = arg_idx0 + index59;

					double x_59  = dx * (double)(arg_idx[0]+arg0);
					double y_59 = dy * (double)(arg_idx_59 +arg1);

					float f3_59 = myfun(native_sin(M_PI*x_59),native_cos(2.0*M_PI*y_59))-1.0;
					float f5_59 = native_sin(M_PI*x_59)*native_cos(2.0*M_PI*y_59);

					if(index59  < size0){
						u.u59[base_index +j] = f3_59;
						ref.u59[base_index +j] = f5_59;
					}

					\\------------COMPUTE UNIT 60  ----------------
					int index60 = j * 64 + 60;
					double arg_idx_60 = arg_idx0 + index60;

					double x_60  = dx * (double)(arg_idx[0]+arg0);
					double y_60 = dy * (double)(arg_idx_60 +arg1);

					float f3_60 = myfun(native_sin(M_PI*x_60),native_cos(2.0*M_PI*y_60))-1.0;
					float f5_60 = native_sin(M_PI*x_60)*native_cos(2.0*M_PI*y_60);

					if(index60  < size0){
						u.u60[base_index +j] = f3_60;
						ref.u60[base_index +j] = f5_60;
					}

					\\------------COMPUTE UNIT 61  ----------------
					int index61 = j * 64 + 61;
					double arg_idx_61 = arg_idx0 + index61;

					double x_61  = dx * (double)(arg_idx[0]+arg0);
					double y_61 = dy * (double)(arg_idx_61 +arg1);

					float f3_61 = myfun(native_sin(M_PI*x_61),native_cos(2.0*M_PI*y_61))-1.0;
					float f5_61 = native_sin(M_PI*x_61)*native_cos(2.0*M_PI*y_61);

					if(index61  < size0){
						u.u61[base_index +j] = f3_61;
						ref.u61[base_index +j] = f5_61;
					}

					\\------------COMPUTE UNIT 62  ----------------
					int index62 = j * 64 + 62;
					double arg_idx_62 = arg_idx0 + index62;

					double x_62  = dx * (double)(arg_idx[0]+arg0);
					double y_62 = dy * (double)(arg_idx_62 +arg1);

					float f3_62 = myfun(native_sin(M_PI*x_62),native_cos(2.0*M_PI*y_62))-1.0;
					float f5_62 = native_sin(M_PI*x_62)*native_cos(2.0*M_PI*y_62);

					if(index62  < size0){
						u.u62[base_index +j] = f3_62;
						ref.u62[base_index +j] = f5_62;
					}

					\\------------COMPUTE UNIT 63  ----------------
					int index63 = j * 64 + 63;
					double arg_idx_63 = arg_idx0 + index63;

					double x_63  = dx * (double)(arg_idx[0]+arg0);
					double y_63 = dy * (double)(arg_idx_63 +arg1);

					float f3_63 = myfun(native_sin(M_PI*x_63),native_cos(2.0*M_PI*y_63))-1.0;
					float f5_63 = native_sin(M_PI*x_63)*native_cos(2.0*M_PI*y_63);

					if(index63  < size0){
						u.u63[base_index +j] = f3_63;
						ref.u63[base_index +j] = f5_63;
					}

			}
		}

}