__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_stencil(
		struct bram_cluster u1,
		struct bram_cluster u2,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	float first_element = 0;
	int base_index1, base_index2, base_index3, base_index0;
	int end_loc =  (size0 >> 6) + 1;
	const int row_block = MAX_X_DIM/P_FACTOR;




	for(int i = 0; i < size1; i++){

		int base_index0 = (i) * row_block;
		int base_index1 = (i+1) * row_block;
		int base_index2 = (i+2) * row_block;
		
		__attribute__((xcl_pipeline_loop))
		for(int j = 0; j < end_loc ; j++){


			int index0 = j * 64 + 0;
			if( index0 > 0 && index0  < size0){
				float f1_0 = (u1.u0[base_index0+j] + u1.u0[base_index2+j]) * 0.125;
				float f2_0 = (u1.u63[base_index1+j-1] + u1.u1[base_index1+j]) * 0.125;
				float f3_0 = u1.u0[base_index1+j] * 0.5;
				float result_0 = f1_1 + f2_1 + f3_1;
				u2.u0[base_index+j] = result_0;
			}

			int index1 = j * 64 + 1;
			if(index1  < size0){
				float f1_1 = (u1.u1[base_index0+j] + u1.u1[base_index2+j]) * 0.125;
				float f2_1 = (u1.u0[base_index1+j] + u1.u2[base_index1+j]) * 0.125;
				float f3_1 = u1.u1[base_index1+j] * 0.5;
				float result_1 = f1_1 + f2_1 + f3_1;
				u2.u1[base_index+j] = result_1;
			}

			int index2 = j * 64 + 2;
			if(index2  < size0){
				float f1_2 = (u1.u2[base_index0+j] + u1.u2[base_index2+j]) * 0.125;
				float f2_2 = (u1.u1[base_index1+j] + u1.u3[base_index1+j]) * 0.125;
				float f3_2 = u1.u2[base_index1+j] * 0.5;
				float result_2 = f1_2 + f2_2 + f3_2;
				u2.u2[base_index+j] = result_2;
			}

			int index3 = j * 64 + 3;
			if(index3  < size0){
				float f1_3 = (u1.u3[base_index0+j] + u1.u3[base_index2+j]) * 0.125;
				float f2_3 = (u1.u2[base_index1+j] + u1.u4[base_index1+j]) * 0.125;
				float f3_3 = u1.u3[base_index1+j] * 0.5;
				float result_3 = f1_3 + f2_3 + f3_3;
				u2.u3[base_index+j] = result_3;
			}

			int index4 = j * 64 + 4;
			if(index4  < size0){
				float f1_4 = (u1.u4[base_index0+j] + u1.u4[base_index2+j]) * 0.125;
				float f2_4 = (u1.u3[base_index1+j] + u1.u5[base_index1+j]) * 0.125;
				float f3_4 = u1.u4[base_index1+j] * 0.5;
				float result_4 = f1_4 + f2_4 + f3_4;
				u2.u4[base_index+j] = result_4;
			}

			int index5 = j * 64 + 5;
			if(index5  < size0){
				float f1_5 = (u1.u5[base_index0+j] + u1.u5[base_index2+j]) * 0.125;
				float f2_5 = (u1.u4[base_index1+j] + u1.u6[base_index1+j]) * 0.125;
				float f3_5 = u1.u5[base_index1+j] * 0.5;
				float result_5 = f1_5 + f2_5 + f3_5;
				u2.u5[base_index+j] = result_5;
			}

			int index6 = j * 64 + 6;
			if(index6  < size0){
				float f1_6 = (u1.u6[base_index0+j] + u1.u6[base_index2+j]) * 0.125;
				float f2_6 = (u1.u5[base_index1+j] + u1.u7[base_index1+j]) * 0.125;
				float f3_6 = u1.u6[base_index1+j] * 0.5;
				float result_6 = f1_6 + f2_6 + f3_6;
				u2.u6[base_index+j] = result_6;
			}

			int index7 = j * 64 + 7;
			if(index7  < size0){
				float f1_7 = (u1.u7[base_index0+j] + u1.u7[base_index2+j]) * 0.125;
				float f2_7 = (u1.u6[base_index1+j] + u1.u8[base_index1+j]) * 0.125;
				float f3_7 = u1.u7[base_index1+j] * 0.5;
				float result_7 = f1_7 + f2_7 + f3_7;
				u2.u7[base_index+j] = result_7;
			}

			int index8 = j * 64 + 8;
			if(index8  < size0){
				float f1_8 = (u1.u8[base_index0+j] + u1.u8[base_index2+j]) * 0.125;
				float f2_8 = (u1.u7[base_index1+j] + u1.u9[base_index1+j]) * 0.125;
				float f3_8 = u1.u8[base_index1+j] * 0.5;
				float result_8 = f1_8 + f2_8 + f3_8;
				u2.u8[base_index+j] = result_8;
			}

			int index9 = j * 64 + 9;
			if(index9  < size0){
				float f1_9 = (u1.u9[base_index0+j] + u1.u9[base_index2+j]) * 0.125;
				float f2_9 = (u1.u8[base_index1+j] + u1.u10[base_index1+j]) * 0.125;
				float f3_9 = u1.u9[base_index1+j] * 0.5;
				float result_9 = f1_9 + f2_9 + f3_9;
				u2.u9[base_index+j] = result_9;
			}

			int index10 = j * 64 + 10;
			if(index10  < size0){
				float f1_10 = (u1.u10[base_index0+j] + u1.u10[base_index2+j]) * 0.125;
				float f2_10 = (u1.u9[base_index1+j] + u1.u11[base_index1+j]) * 0.125;
				float f3_10 = u1.u10[base_index1+j] * 0.5;
				float result_10 = f1_10 + f2_10 + f3_10;
				u2.u10[base_index+j] = result_10;
			}

			int index11 = j * 64 + 11;
			if(index11  < size0){
				float f1_11 = (u1.u11[base_index0+j] + u1.u11[base_index2+j]) * 0.125;
				float f2_11 = (u1.u10[base_index1+j] + u1.u12[base_index1+j]) * 0.125;
				float f3_11 = u1.u11[base_index1+j] * 0.5;
				float result_11 = f1_11 + f2_11 + f3_11;
				u2.u11[base_index+j] = result_11;
			}

			int index12 = j * 64 + 12;
			if(index12  < size0){
				float f1_12 = (u1.u12[base_index0+j] + u1.u12[base_index2+j]) * 0.125;
				float f2_12 = (u1.u11[base_index1+j] + u1.u13[base_index1+j]) * 0.125;
				float f3_12 = u1.u12[base_index1+j] * 0.5;
				float result_12 = f1_12 + f2_12 + f3_12;
				u2.u12[base_index+j] = result_12;
			}

			int index13 = j * 64 + 13;
			if(index13  < size0){
				float f1_13 = (u1.u13[base_index0+j] + u1.u13[base_index2+j]) * 0.125;
				float f2_13 = (u1.u12[base_index1+j] + u1.u14[base_index1+j]) * 0.125;
				float f3_13 = u1.u13[base_index1+j] * 0.5;
				float result_13 = f1_13 + f2_13 + f3_13;
				u2.u13[base_index+j] = result_13;
			}

			int index14 = j * 64 + 14;
			if(index14  < size0){
				float f1_14 = (u1.u14[base_index0+j] + u1.u14[base_index2+j]) * 0.125;
				float f2_14 = (u1.u13[base_index1+j] + u1.u15[base_index1+j]) * 0.125;
				float f3_14 = u1.u14[base_index1+j] * 0.5;
				float result_14 = f1_14 + f2_14 + f3_14;
				u2.u14[base_index+j] = result_14;
			}

			int index15 = j * 64 + 15;
			if(index15  < size0){
				float f1_15 = (u1.u15[base_index0+j] + u1.u15[base_index2+j]) * 0.125;
				float f2_15 = (u1.u14[base_index1+j] + u1.u16[base_index1+j]) * 0.125;
				float f3_15 = u1.u15[base_index1+j] * 0.5;
				float result_15 = f1_15 + f2_15 + f3_15;
				u2.u15[base_index+j] = result_15;
			}

			int index16 = j * 64 + 16;
			if(index16  < size0){
				float f1_16 = (u1.u16[base_index0+j] + u1.u16[base_index2+j]) * 0.125;
				float f2_16 = (u1.u15[base_index1+j] + u1.u17[base_index1+j]) * 0.125;
				float f3_16 = u1.u16[base_index1+j] * 0.5;
				float result_16 = f1_16 + f2_16 + f3_16;
				u2.u16[base_index+j] = result_16;
			}

			int index17 = j * 64 + 17;
			if(index17  < size0){
				float f1_17 = (u1.u17[base_index0+j] + u1.u17[base_index2+j]) * 0.125;
				float f2_17 = (u1.u16[base_index1+j] + u1.u18[base_index1+j]) * 0.125;
				float f3_17 = u1.u17[base_index1+j] * 0.5;
				float result_17 = f1_17 + f2_17 + f3_17;
				u2.u17[base_index+j] = result_17;
			}

			int index18 = j * 64 + 18;
			if(index18  < size0){
				float f1_18 = (u1.u18[base_index0+j] + u1.u18[base_index2+j]) * 0.125;
				float f2_18 = (u1.u17[base_index1+j] + u1.u19[base_index1+j]) * 0.125;
				float f3_18 = u1.u18[base_index1+j] * 0.5;
				float result_18 = f1_18 + f2_18 + f3_18;
				u2.u18[base_index+j] = result_18;
			}

			int index19 = j * 64 + 19;
			if(index19  < size0){
				float f1_19 = (u1.u19[base_index0+j] + u1.u19[base_index2+j]) * 0.125;
				float f2_19 = (u1.u18[base_index1+j] + u1.u20[base_index1+j]) * 0.125;
				float f3_19 = u1.u19[base_index1+j] * 0.5;
				float result_19 = f1_19 + f2_19 + f3_19;
				u2.u19[base_index+j] = result_19;
			}

			int index20 = j * 64 + 20;
			if(index20  < size0){
				float f1_20 = (u1.u20[base_index0+j] + u1.u20[base_index2+j]) * 0.125;
				float f2_20 = (u1.u19[base_index1+j] + u1.u21[base_index1+j]) * 0.125;
				float f3_20 = u1.u20[base_index1+j] * 0.5;
				float result_20 = f1_20 + f2_20 + f3_20;
				u2.u20[base_index+j] = result_20;
			}

			int index21 = j * 64 + 21;
			if(index21  < size0){
				float f1_21 = (u1.u21[base_index0+j] + u1.u21[base_index2+j]) * 0.125;
				float f2_21 = (u1.u20[base_index1+j] + u1.u22[base_index1+j]) * 0.125;
				float f3_21 = u1.u21[base_index1+j] * 0.5;
				float result_21 = f1_21 + f2_21 + f3_21;
				u2.u21[base_index+j] = result_21;
			}

			int index22 = j * 64 + 22;
			if(index22  < size0){
				float f1_22 = (u1.u22[base_index0+j] + u1.u22[base_index2+j]) * 0.125;
				float f2_22 = (u1.u21[base_index1+j] + u1.u23[base_index1+j]) * 0.125;
				float f3_22 = u1.u22[base_index1+j] * 0.5;
				float result_22 = f1_22 + f2_22 + f3_22;
				u2.u22[base_index+j] = result_22;
			}

			int index23 = j * 64 + 23;
			if(index23  < size0){
				float f1_23 = (u1.u23[base_index0+j] + u1.u23[base_index2+j]) * 0.125;
				float f2_23 = (u1.u22[base_index1+j] + u1.u24[base_index1+j]) * 0.125;
				float f3_23 = u1.u23[base_index1+j] * 0.5;
				float result_23 = f1_23 + f2_23 + f3_23;
				u2.u23[base_index+j] = result_23;
			}

			int index24 = j * 64 + 24;
			if(index24  < size0){
				float f1_24 = (u1.u24[base_index0+j] + u1.u24[base_index2+j]) * 0.125;
				float f2_24 = (u1.u23[base_index1+j] + u1.u25[base_index1+j]) * 0.125;
				float f3_24 = u1.u24[base_index1+j] * 0.5;
				float result_24 = f1_24 + f2_24 + f3_24;
				u2.u24[base_index+j] = result_24;
			}

			int index25 = j * 64 + 25;
			if(index25  < size0){
				float f1_25 = (u1.u25[base_index0+j] + u1.u25[base_index2+j]) * 0.125;
				float f2_25 = (u1.u24[base_index1+j] + u1.u26[base_index1+j]) * 0.125;
				float f3_25 = u1.u25[base_index1+j] * 0.5;
				float result_25 = f1_25 + f2_25 + f3_25;
				u2.u25[base_index+j] = result_25;
			}

			int index26 = j * 64 + 26;
			if(index26  < size0){
				float f1_26 = (u1.u26[base_index0+j] + u1.u26[base_index2+j]) * 0.125;
				float f2_26 = (u1.u25[base_index1+j] + u1.u27[base_index1+j]) * 0.125;
				float f3_26 = u1.u26[base_index1+j] * 0.5;
				float result_26 = f1_26 + f2_26 + f3_26;
				u2.u26[base_index+j] = result_26;
			}

			int index27 = j * 64 + 27;
			if(index27  < size0){
				float f1_27 = (u1.u27[base_index0+j] + u1.u27[base_index2+j]) * 0.125;
				float f2_27 = (u1.u26[base_index1+j] + u1.u28[base_index1+j]) * 0.125;
				float f3_27 = u1.u27[base_index1+j] * 0.5;
				float result_27 = f1_27 + f2_27 + f3_27;
				u2.u27[base_index+j] = result_27;
			}

			int index28 = j * 64 + 28;
			if(index28  < size0){
				float f1_28 = (u1.u28[base_index0+j] + u1.u28[base_index2+j]) * 0.125;
				float f2_28 = (u1.u27[base_index1+j] + u1.u29[base_index1+j]) * 0.125;
				float f3_28 = u1.u28[base_index1+j] * 0.5;
				float result_28 = f1_28 + f2_28 + f3_28;
				u2.u28[base_index+j] = result_28;
			}

			int index29 = j * 64 + 29;
			if(index29  < size0){
				float f1_29 = (u1.u29[base_index0+j] + u1.u29[base_index2+j]) * 0.125;
				float f2_29 = (u1.u28[base_index1+j] + u1.u30[base_index1+j]) * 0.125;
				float f3_29 = u1.u29[base_index1+j] * 0.5;
				float result_29 = f1_29 + f2_29 + f3_29;
				u2.u29[base_index+j] = result_29;
			}

			int index30 = j * 64 + 30;
			if(index30  < size0){
				float f1_30 = (u1.u30[base_index0+j] + u1.u30[base_index2+j]) * 0.125;
				float f2_30 = (u1.u29[base_index1+j] + u1.u31[base_index1+j]) * 0.125;
				float f3_30 = u1.u30[base_index1+j] * 0.5;
				float result_30 = f1_30 + f2_30 + f3_30;
				u2.u30[base_index+j] = result_30;
			}

			int index31 = j * 64 + 31;
			if(index31  < size0){
				float f1_31 = (u1.u31[base_index0+j] + u1.u31[base_index2+j]) * 0.125;
				float f2_31 = (u1.u30[base_index1+j] + u1.u32[base_index1+j]) * 0.125;
				float f3_31 = u1.u31[base_index1+j] * 0.5;
				float result_31 = f1_31 + f2_31 + f3_31;
				u2.u31[base_index+j] = result_31;
			}

			int index32 = j * 64 + 32;
			if(index32  < size0){
				float f1_32 = (u1.u32[base_index0+j] + u1.u32[base_index2+j]) * 0.125;
				float f2_32 = (u1.u31[base_index1+j] + u1.u33[base_index1+j]) * 0.125;
				float f3_32 = u1.u32[base_index1+j] * 0.5;
				float result_32 = f1_32 + f2_32 + f3_32;
				u2.u32[base_index+j] = result_32;
			}

			int index33 = j * 64 + 33;
			if(index33  < size0){
				float f1_33 = (u1.u33[base_index0+j] + u1.u33[base_index2+j]) * 0.125;
				float f2_33 = (u1.u32[base_index1+j] + u1.u34[base_index1+j]) * 0.125;
				float f3_33 = u1.u33[base_index1+j] * 0.5;
				float result_33 = f1_33 + f2_33 + f3_33;
				u2.u33[base_index+j] = result_33;
			}

			int index34 = j * 64 + 34;
			if(index34  < size0){
				float f1_34 = (u1.u34[base_index0+j] + u1.u34[base_index2+j]) * 0.125;
				float f2_34 = (u1.u33[base_index1+j] + u1.u35[base_index1+j]) * 0.125;
				float f3_34 = u1.u34[base_index1+j] * 0.5;
				float result_34 = f1_34 + f2_34 + f3_34;
				u2.u34[base_index+j] = result_34;
			}

			int index35 = j * 64 + 35;
			if(index35  < size0){
				float f1_35 = (u1.u35[base_index0+j] + u1.u35[base_index2+j]) * 0.125;
				float f2_35 = (u1.u34[base_index1+j] + u1.u36[base_index1+j]) * 0.125;
				float f3_35 = u1.u35[base_index1+j] * 0.5;
				float result_35 = f1_35 + f2_35 + f3_35;
				u2.u35[base_index+j] = result_35;
			}

			int index36 = j * 64 + 36;
			if(index36  < size0){
				float f1_36 = (u1.u36[base_index0+j] + u1.u36[base_index2+j]) * 0.125;
				float f2_36 = (u1.u35[base_index1+j] + u1.u37[base_index1+j]) * 0.125;
				float f3_36 = u1.u36[base_index1+j] * 0.5;
				float result_36 = f1_36 + f2_36 + f3_36;
				u2.u36[base_index+j] = result_36;
			}

			int index37 = j * 64 + 37;
			if(index37  < size0){
				float f1_37 = (u1.u37[base_index0+j] + u1.u37[base_index2+j]) * 0.125;
				float f2_37 = (u1.u36[base_index1+j] + u1.u38[base_index1+j]) * 0.125;
				float f3_37 = u1.u37[base_index1+j] * 0.5;
				float result_37 = f1_37 + f2_37 + f3_37;
				u2.u37[base_index+j] = result_37;
			}

			int index38 = j * 64 + 38;
			if(index38  < size0){
				float f1_38 = (u1.u38[base_index0+j] + u1.u38[base_index2+j]) * 0.125;
				float f2_38 = (u1.u37[base_index1+j] + u1.u39[base_index1+j]) * 0.125;
				float f3_38 = u1.u38[base_index1+j] * 0.5;
				float result_38 = f1_38 + f2_38 + f3_38;
				u2.u38[base_index+j] = result_38;
			}

			int index39 = j * 64 + 39;
			if(index39  < size0){
				float f1_39 = (u1.u39[base_index0+j] + u1.u39[base_index2+j]) * 0.125;
				float f2_39 = (u1.u38[base_index1+j] + u1.u40[base_index1+j]) * 0.125;
				float f3_39 = u1.u39[base_index1+j] * 0.5;
				float result_39 = f1_39 + f2_39 + f3_39;
				u2.u39[base_index+j] = result_39;
			}

			int index40 = j * 64 + 40;
			if(index40  < size0){
				float f1_40 = (u1.u40[base_index0+j] + u1.u40[base_index2+j]) * 0.125;
				float f2_40 = (u1.u39[base_index1+j] + u1.u41[base_index1+j]) * 0.125;
				float f3_40 = u1.u40[base_index1+j] * 0.5;
				float result_40 = f1_40 + f2_40 + f3_40;
				u2.u40[base_index+j] = result_40;
			}

			int index41 = j * 64 + 41;
			if(index41  < size0){
				float f1_41 = (u1.u41[base_index0+j] + u1.u41[base_index2+j]) * 0.125;
				float f2_41 = (u1.u40[base_index1+j] + u1.u42[base_index1+j]) * 0.125;
				float f3_41 = u1.u41[base_index1+j] * 0.5;
				float result_41 = f1_41 + f2_41 + f3_41;
				u2.u41[base_index+j] = result_41;
			}

			int index42 = j * 64 + 42;
			if(index42  < size0){
				float f1_42 = (u1.u42[base_index0+j] + u1.u42[base_index2+j]) * 0.125;
				float f2_42 = (u1.u41[base_index1+j] + u1.u43[base_index1+j]) * 0.125;
				float f3_42 = u1.u42[base_index1+j] * 0.5;
				float result_42 = f1_42 + f2_42 + f3_42;
				u2.u42[base_index+j] = result_42;
			}

			int index43 = j * 64 + 43;
			if(index43  < size0){
				float f1_43 = (u1.u43[base_index0+j] + u1.u43[base_index2+j]) * 0.125;
				float f2_43 = (u1.u42[base_index1+j] + u1.u44[base_index1+j]) * 0.125;
				float f3_43 = u1.u43[base_index1+j] * 0.5;
				float result_43 = f1_43 + f2_43 + f3_43;
				u2.u43[base_index+j] = result_43;
			}

			int index44 = j * 64 + 44;
			if(index44  < size0){
				float f1_44 = (u1.u44[base_index0+j] + u1.u44[base_index2+j]) * 0.125;
				float f2_44 = (u1.u43[base_index1+j] + u1.u45[base_index1+j]) * 0.125;
				float f3_44 = u1.u44[base_index1+j] * 0.5;
				float result_44 = f1_44 + f2_44 + f3_44;
				u2.u44[base_index+j] = result_44;
			}

			int index45 = j * 64 + 45;
			if(index45  < size0){
				float f1_45 = (u1.u45[base_index0+j] + u1.u45[base_index2+j]) * 0.125;
				float f2_45 = (u1.u44[base_index1+j] + u1.u46[base_index1+j]) * 0.125;
				float f3_45 = u1.u45[base_index1+j] * 0.5;
				float result_45 = f1_45 + f2_45 + f3_45;
				u2.u45[base_index+j] = result_45;
			}

			int index46 = j * 64 + 46;
			if(index46  < size0){
				float f1_46 = (u1.u46[base_index0+j] + u1.u46[base_index2+j]) * 0.125;
				float f2_46 = (u1.u45[base_index1+j] + u1.u47[base_index1+j]) * 0.125;
				float f3_46 = u1.u46[base_index1+j] * 0.5;
				float result_46 = f1_46 + f2_46 + f3_46;
				u2.u46[base_index+j] = result_46;
			}

			int index47 = j * 64 + 47;
			if(index47  < size0){
				float f1_47 = (u1.u47[base_index0+j] + u1.u47[base_index2+j]) * 0.125;
				float f2_47 = (u1.u46[base_index1+j] + u1.u48[base_index1+j]) * 0.125;
				float f3_47 = u1.u47[base_index1+j] * 0.5;
				float result_47 = f1_47 + f2_47 + f3_47;
				u2.u47[base_index+j] = result_47;
			}

			int index48 = j * 64 + 48;
			if(index48  < size0){
				float f1_48 = (u1.u48[base_index0+j] + u1.u48[base_index2+j]) * 0.125;
				float f2_48 = (u1.u47[base_index1+j] + u1.u49[base_index1+j]) * 0.125;
				float f3_48 = u1.u48[base_index1+j] * 0.5;
				float result_48 = f1_48 + f2_48 + f3_48;
				u2.u48[base_index+j] = result_48;
			}

			int index49 = j * 64 + 49;
			if(index49  < size0){
				float f1_49 = (u1.u49[base_index0+j] + u1.u49[base_index2+j]) * 0.125;
				float f2_49 = (u1.u48[base_index1+j] + u1.u50[base_index1+j]) * 0.125;
				float f3_49 = u1.u49[base_index1+j] * 0.5;
				float result_49 = f1_49 + f2_49 + f3_49;
				u2.u49[base_index+j] = result_49;
			}

			int index50 = j * 64 + 50;
			if(index50  < size0){
				float f1_50 = (u1.u50[base_index0+j] + u1.u50[base_index2+j]) * 0.125;
				float f2_50 = (u1.u49[base_index1+j] + u1.u51[base_index1+j]) * 0.125;
				float f3_50 = u1.u50[base_index1+j] * 0.5;
				float result_50 = f1_50 + f2_50 + f3_50;
				u2.u50[base_index+j] = result_50;
			}

			int index51 = j * 64 + 51;
			if(index51  < size0){
				float f1_51 = (u1.u51[base_index0+j] + u1.u51[base_index2+j]) * 0.125;
				float f2_51 = (u1.u50[base_index1+j] + u1.u52[base_index1+j]) * 0.125;
				float f3_51 = u1.u51[base_index1+j] * 0.5;
				float result_51 = f1_51 + f2_51 + f3_51;
				u2.u51[base_index+j] = result_51;
			}

			int index52 = j * 64 + 52;
			if(index52  < size0){
				float f1_52 = (u1.u52[base_index0+j] + u1.u52[base_index2+j]) * 0.125;
				float f2_52 = (u1.u51[base_index1+j] + u1.u53[base_index1+j]) * 0.125;
				float f3_52 = u1.u52[base_index1+j] * 0.5;
				float result_52 = f1_52 + f2_52 + f3_52;
				u2.u52[base_index+j] = result_52;
			}

			int index53 = j * 64 + 53;
			if(index53  < size0){
				float f1_53 = (u1.u53[base_index0+j] + u1.u53[base_index2+j]) * 0.125;
				float f2_53 = (u1.u52[base_index1+j] + u1.u54[base_index1+j]) * 0.125;
				float f3_53 = u1.u53[base_index1+j] * 0.5;
				float result_53 = f1_53 + f2_53 + f3_53;
				u2.u53[base_index+j] = result_53;
			}

			int index54 = j * 64 + 54;
			if(index54  < size0){
				float f1_54 = (u1.u54[base_index0+j] + u1.u54[base_index2+j]) * 0.125;
				float f2_54 = (u1.u53[base_index1+j] + u1.u55[base_index1+j]) * 0.125;
				float f3_54 = u1.u54[base_index1+j] * 0.5;
				float result_54 = f1_54 + f2_54 + f3_54;
				u2.u54[base_index+j] = result_54;
			}

			int index55 = j * 64 + 55;
			if(index55  < size0){
				float f1_55 = (u1.u55[base_index0+j] + u1.u55[base_index2+j]) * 0.125;
				float f2_55 = (u1.u54[base_index1+j] + u1.u56[base_index1+j]) * 0.125;
				float f3_55 = u1.u55[base_index1+j] * 0.5;
				float result_55 = f1_55 + f2_55 + f3_55;
				u2.u55[base_index+j] = result_55;
			}

			int index56 = j * 64 + 56;
			if(index56  < size0){
				float f1_56 = (u1.u56[base_index0+j] + u1.u56[base_index2+j]) * 0.125;
				float f2_56 = (u1.u55[base_index1+j] + u1.u57[base_index1+j]) * 0.125;
				float f3_56 = u1.u56[base_index1+j] * 0.5;
				float result_56 = f1_56 + f2_56 + f3_56;
				u2.u56[base_index+j] = result_56;
			}

			int index57 = j * 64 + 57;
			if(index57  < size0){
				float f1_57 = (u1.u57[base_index0+j] + u1.u57[base_index2+j]) * 0.125;
				float f2_57 = (u1.u56[base_index1+j] + u1.u58[base_index1+j]) * 0.125;
				float f3_57 = u1.u57[base_index1+j] * 0.5;
				float result_57 = f1_57 + f2_57 + f3_57;
				u2.u57[base_index+j] = result_57;
			}

			int index58 = j * 64 + 58;
			if(index58  < size0){
				float f1_58 = (u1.u58[base_index0+j] + u1.u58[base_index2+j]) * 0.125;
				float f2_58 = (u1.u57[base_index1+j] + u1.u59[base_index1+j]) * 0.125;
				float f3_58 = u1.u58[base_index1+j] * 0.5;
				float result_58 = f1_58 + f2_58 + f3_58;
				u2.u58[base_index+j] = result_58;
			}

			int index59 = j * 64 + 59;
			if(index59  < size0){
				float f1_59 = (u1.u59[base_index0+j] + u1.u59[base_index2+j]) * 0.125;
				float f2_59 = (u1.u58[base_index1+j] + u1.u60[base_index1+j]) * 0.125;
				float f3_59 = u1.u59[base_index1+j] * 0.5;
				float result_59 = f1_59 + f2_59 + f3_59;
				u2.u59[base_index+j] = result_59;
			}

			int index60 = j * 64 + 60;
			if(index60  < size0){
				float f1_60 = (u1.u60[base_index0+j] + u1.u60[base_index2+j]) * 0.125;
				float f2_60 = (u1.u59[base_index1+j] + u1.u61[base_index1+j]) * 0.125;
				float f3_60 = u1.u60[base_index1+j] * 0.5;
				float result_60 = f1_60 + f2_60 + f3_60;
				u2.u60[base_index+j] = result_60;
			}

			int index61 = j * 64 + 61;
			if(index61  < size0){
				float f1_61 = (u1.u61[base_index0+j] + u1.u61[base_index2+j]) * 0.125;
				float f2_61 = (u1.u60[base_index1+j] + u1.u62[base_index1+j]) * 0.125;
				float f3_61 = u1.u61[base_index1+j] * 0.5;
				float result_61 = f1_61 + f2_61 + f3_61;
				u2.u61[base_index+j] = result_61;
			}

			int index62 = j * 64 + 62;
			if(index62  < size0){
				float f1_62 = (u1.u62[base_index0+j] + u1.u62[base_index2+j]) * 0.125;
				float f2_62 = (u1.u61[base_index1+j] + u1.u63[base_index1+j]) * 0.125;
				float f3_62 = u1.u62[base_index1+j] * 0.5;
				float result_62 = f1_62 + f2_62 + f3_62;
				u2.u62[base_index+j] = result_62;
			}

			int index63 = j * 64 + 0;
			if(index63  < size0){
				float f1_63 = (u1.u63[base_index0+j] + u1.u63[base_index2+j]) * 0.125;
				float f2_63 = (u1.u0[base_index1+j+1] + u1.u62[base_index1+j]) * 0.125;
				float f3_63 = u1.u63[base_index1+j] * 0.5;
				float result_63 = f1_63 + f2_63 + f3_63;
				u2.u63[base_index+j] = result_63;
			}
			
		}
	}
}