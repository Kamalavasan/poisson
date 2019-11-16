__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_error(
		local float* restrict arg0,
		local const float* restrict arg1,
		__global float* restrict arg2,
		__local float* scratch2,
		int r_bytes2,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	float g_sum = 0;

	int row_shift0 = 0;
	int row_shift1 = 0;
	float arr_focus[P_FACTOR] __attribute__((xcl_array_partition(complete, 1)));
	for(int i = 0; i < P_FACTOR; i++){
		arr_focus[i]   = 0;
	}

	for(int i  = 0; i < size1; i++){
			int end_loc =  (size0 >> 6) + 1;
		const int row_block = MAX_X_DIM/P_FACTOR;

		for(int i  = 0; i < size1; i++){

		int base_index = base0  + i * row_block;
		__attribute__((xcl_pipeline_loop))
		for(int j = 0; j < end_loc ; j++){

			int index0 = j * 64 + 0;
			if(index0 != 0 || index0  < size0){
				float diff_0 = u.u0[base_index+j] - u2.u0[base_index+j];
				arr_focus[0] = arr_focus[0] +  diff_0 * diff_0
			}

			int index1 = j * 64 + 1;
			if(index1 != 0 || index1  < size0){
				float diff_1 = u.u1[base_index+j] - u2.u1[base_index+j];
				arr_focus[1] = arr_focus[1] +  diff_1 * diff_1
			}

			int index2 = j * 64 + 2;
			if(index2 != 0 || index2  < size0){
				float diff_2 = u.u2[base_index+j] - u2.u2[base_index+j];
				arr_focus[2] = arr_focus[2] +  diff_2 * diff_2
			}

			int index3 = j * 64 + 3;
			if(index3 != 0 || index3  < size0){
				float diff_3 = u.u3[base_index+j] - u2.u3[base_index+j];
				arr_focus[3] = arr_focus[3] +  diff_3 * diff_3
			}

			int index4 = j * 64 + 4;
			if(index4 != 0 || index4  < size0){
				float diff_4 = u.u4[base_index+j] - u2.u4[base_index+j];
				arr_focus[4] = arr_focus[4] +  diff_4 * diff_4
			}

			int index5 = j * 64 + 5;
			if(index5 != 0 || index5  < size0){
				float diff_5 = u.u5[base_index+j] - u2.u5[base_index+j];
				arr_focus[5] = arr_focus[5] +  diff_5 * diff_5
			}

			int index6 = j * 64 + 6;
			if(index6 != 0 || index6  < size0){
				float diff_6 = u.u6[base_index+j] - u2.u6[base_index+j];
				arr_focus[6] = arr_focus[6] +  diff_6 * diff_6
			}

			int index7 = j * 64 + 7;
			if(index7 != 0 || index7  < size0){
				float diff_7 = u.u7[base_index+j] - u2.u7[base_index+j];
				arr_focus[7] = arr_focus[7] +  diff_7 * diff_7
			}

			int index8 = j * 64 + 8;
			if(index8 != 0 || index8  < size0){
				float diff_8 = u.u8[base_index+j] - u2.u8[base_index+j];
				arr_focus[8] = arr_focus[8] +  diff_8 * diff_8
			}

			int index9 = j * 64 + 9;
			if(index9 != 0 || index9  < size0){
				float diff_9 = u.u9[base_index+j] - u2.u9[base_index+j];
				arr_focus[9] = arr_focus[9] +  diff_9 * diff_9
			}

			int index10 = j * 64 + 10;
			if(index10 != 0 || index10  < size0){
				float diff_10 = u.u10[base_index+j] - u2.u10[base_index+j];
				arr_focus[10] = arr_focus[10] +  diff_10 * diff_10
			}

			int index11 = j * 64 + 11;
			if(index11 != 0 || index11  < size0){
				float diff_11 = u.u11[base_index+j] - u2.u11[base_index+j];
				arr_focus[11] = arr_focus[11] +  diff_11 * diff_11
			}

			int index12 = j * 64 + 12;
			if(index12 != 0 || index12  < size0){
				float diff_12 = u.u12[base_index+j] - u2.u12[base_index+j];
				arr_focus[12] = arr_focus[12] +  diff_12 * diff_12
			}

			int index13 = j * 64 + 13;
			if(index13 != 0 || index13  < size0){
				float diff_13 = u.u13[base_index+j] - u2.u13[base_index+j];
				arr_focus[13] = arr_focus[13] +  diff_13 * diff_13
			}

			int index14 = j * 64 + 14;
			if(index14 != 0 || index14  < size0){
				float diff_14 = u.u14[base_index+j] - u2.u14[base_index+j];
				arr_focus[14] = arr_focus[14] +  diff_14 * diff_14
			}

			int index15 = j * 64 + 15;
			if(index15 != 0 || index15  < size0){
				float diff_15 = u.u15[base_index+j] - u2.u15[base_index+j];
				arr_focus[15] = arr_focus[15] +  diff_15 * diff_15
			}

			int index16 = j * 64 + 16;
			if(index16 != 0 || index16  < size0){
				float diff_16 = u.u16[base_index+j] - u2.u16[base_index+j];
				arr_focus[16] = arr_focus[16] +  diff_16 * diff_16
			}

			int index17 = j * 64 + 17;
			if(index17 != 0 || index17  < size0){
				float diff_17 = u.u17[base_index+j] - u2.u17[base_index+j];
				arr_focus[17] = arr_focus[17] +  diff_17 * diff_17
			}

			int index18 = j * 64 + 18;
			if(index18 != 0 || index18  < size0){
				float diff_18 = u.u18[base_index+j] - u2.u18[base_index+j];
				arr_focus[18] = arr_focus[18] +  diff_18 * diff_18
			}

			int index19 = j * 64 + 19;
			if(index19 != 0 || index19  < size0){
				float diff_19 = u.u19[base_index+j] - u2.u19[base_index+j];
				arr_focus[19] = arr_focus[19] +  diff_19 * diff_19
			}

			int index20 = j * 64 + 20;
			if(index20 != 0 || index20  < size0){
				float diff_20 = u.u20[base_index+j] - u2.u20[base_index+j];
				arr_focus[20] = arr_focus[20] +  diff_20 * diff_20
			}

			int index21 = j * 64 + 21;
			if(index21 != 0 || index21  < size0){
				float diff_21 = u.u21[base_index+j] - u2.u21[base_index+j];
				arr_focus[21] = arr_focus[21] +  diff_21 * diff_21
			}

			int index22 = j * 64 + 22;
			if(index22 != 0 || index22  < size0){
				float diff_22 = u.u22[base_index+j] - u2.u22[base_index+j];
				arr_focus[22] = arr_focus[22] +  diff_22 * diff_22
			}

			int index23 = j * 64 + 23;
			if(index23 != 0 || index23  < size0){
				float diff_23 = u.u23[base_index+j] - u2.u23[base_index+j];
				arr_focus[23] = arr_focus[23] +  diff_23 * diff_23
			}

			int index24 = j * 64 + 24;
			if(index24 != 0 || index24  < size0){
				float diff_24 = u.u24[base_index+j] - u2.u24[base_index+j];
				arr_focus[24] = arr_focus[24] +  diff_24 * diff_24
			}

			int index25 = j * 64 + 25;
			if(index25 != 0 || index25  < size0){
				float diff_25 = u.u25[base_index+j] - u2.u25[base_index+j];
				arr_focus[25] = arr_focus[25] +  diff_25 * diff_25
			}

			int index26 = j * 64 + 26;
			if(index26 != 0 || index26  < size0){
				float diff_26 = u.u26[base_index+j] - u2.u26[base_index+j];
				arr_focus[26] = arr_focus[26] +  diff_26 * diff_26
			}

			int index27 = j * 64 + 27;
			if(index27 != 0 || index27  < size0){
				float diff_27 = u.u27[base_index+j] - u2.u27[base_index+j];
				arr_focus[27] = arr_focus[27] +  diff_27 * diff_27
			}

			int index28 = j * 64 + 28;
			if(index28 != 0 || index28  < size0){
				float diff_28 = u.u28[base_index+j] - u2.u28[base_index+j];
				arr_focus[28] = arr_focus[28] +  diff_28 * diff_28
			}

			int index29 = j * 64 + 29;
			if(index29 != 0 || index29  < size0){
				float diff_29 = u.u29[base_index+j] - u2.u29[base_index+j];
				arr_focus[29] = arr_focus[29] +  diff_29 * diff_29
			}

			int index30 = j * 64 + 30;
			if(index30 != 0 || index30  < size0){
				float diff_30 = u.u30[base_index+j] - u2.u30[base_index+j];
				arr_focus[30] = arr_focus[30] +  diff_30 * diff_30
			}

			int index31 = j * 64 + 31;
			if(index31 != 0 || index31  < size0){
				float diff_31 = u.u31[base_index+j] - u2.u31[base_index+j];
				arr_focus[31] = arr_focus[31] +  diff_31 * diff_31
			}

			int index32 = j * 64 + 32;
			if(index32 != 0 || index32  < size0){
				float diff_32 = u.u32[base_index+j] - u2.u32[base_index+j];
				arr_focus[32] = arr_focus[32] +  diff_32 * diff_32
			}

			int index33 = j * 64 + 33;
			if(index33 != 0 || index33  < size0){
				float diff_33 = u.u33[base_index+j] - u2.u33[base_index+j];
				arr_focus[33] = arr_focus[33] +  diff_33 * diff_33
			}

			int index34 = j * 64 + 34;
			if(index34 != 0 || index34  < size0){
				float diff_34 = u.u34[base_index+j] - u2.u34[base_index+j];
				arr_focus[34] = arr_focus[34] +  diff_34 * diff_34
			}

			int index35 = j * 64 + 35;
			if(index35 != 0 || index35  < size0){
				float diff_35 = u.u35[base_index+j] - u2.u35[base_index+j];
				arr_focus[35] = arr_focus[35] +  diff_35 * diff_35
			}

			int index36 = j * 64 + 36;
			if(index36 != 0 || index36  < size0){
				float diff_36 = u.u36[base_index+j] - u2.u36[base_index+j];
				arr_focus[36] = arr_focus[36] +  diff_36 * diff_36
			}

			int index37 = j * 64 + 37;
			if(index37 != 0 || index37  < size0){
				float diff_37 = u.u37[base_index+j] - u2.u37[base_index+j];
				arr_focus[37] = arr_focus[37] +  diff_37 * diff_37
			}

			int index38 = j * 64 + 38;
			if(index38 != 0 || index38  < size0){
				float diff_38 = u.u38[base_index+j] - u2.u38[base_index+j];
				arr_focus[38] = arr_focus[38] +  diff_38 * diff_38
			}

			int index39 = j * 64 + 39;
			if(index39 != 0 || index39  < size0){
				float diff_39 = u.u39[base_index+j] - u2.u39[base_index+j];
				arr_focus[39] = arr_focus[39] +  diff_39 * diff_39
			}

			int index40 = j * 64 + 40;
			if(index40 != 0 || index40  < size0){
				float diff_40 = u.u40[base_index+j] - u2.u40[base_index+j];
				arr_focus[40] = arr_focus[40] +  diff_40 * diff_40
			}

			int index41 = j * 64 + 41;
			if(index41 != 0 || index41  < size0){
				float diff_41 = u.u41[base_index+j] - u2.u41[base_index+j];
				arr_focus[41] = arr_focus[41] +  diff_41 * diff_41
			}

			int index42 = j * 64 + 42;
			if(index42 != 0 || index42  < size0){
				float diff_42 = u.u42[base_index+j] - u2.u42[base_index+j];
				arr_focus[42] = arr_focus[42] +  diff_42 * diff_42
			}

			int index43 = j * 64 + 43;
			if(index43 != 0 || index43  < size0){
				float diff_43 = u.u43[base_index+j] - u2.u43[base_index+j];
				arr_focus[43] = arr_focus[43] +  diff_43 * diff_43
			}

			int index44 = j * 64 + 44;
			if(index44 != 0 || index44  < size0){
				float diff_44 = u.u44[base_index+j] - u2.u44[base_index+j];
				arr_focus[44] = arr_focus[44] +  diff_44 * diff_44
			}

			int index45 = j * 64 + 45;
			if(index45 != 0 || index45  < size0){
				float diff_45 = u.u45[base_index+j] - u2.u45[base_index+j];
				arr_focus[45] = arr_focus[45] +  diff_45 * diff_45
			}

			int index46 = j * 64 + 46;
			if(index46 != 0 || index46  < size0){
				float diff_46 = u.u46[base_index+j] - u2.u46[base_index+j];
				arr_focus[46] = arr_focus[46] +  diff_46 * diff_46
			}

			int index47 = j * 64 + 47;
			if(index47 != 0 || index47  < size0){
				float diff_47 = u.u47[base_index+j] - u2.u47[base_index+j];
				arr_focus[47] = arr_focus[47] +  diff_47 * diff_47
			}

			int index48 = j * 64 + 48;
			if(index48 != 0 || index48  < size0){
				float diff_48 = u.u48[base_index+j] - u2.u48[base_index+j];
				arr_focus[48] = arr_focus[48] +  diff_48 * diff_48
			}

			int index49 = j * 64 + 49;
			if(index49 != 0 || index49  < size0){
				float diff_49 = u.u49[base_index+j] - u2.u49[base_index+j];
				arr_focus[49] = arr_focus[49] +  diff_49 * diff_49
			}

			int index50 = j * 64 + 50;
			if(index50 != 0 || index50  < size0){
				float diff_50 = u.u50[base_index+j] - u2.u50[base_index+j];
				arr_focus[50] = arr_focus[50] +  diff_50 * diff_50
			}

			int index51 = j * 64 + 51;
			if(index51 != 0 || index51  < size0){
				float diff_51 = u.u51[base_index+j] - u2.u51[base_index+j];
				arr_focus[51] = arr_focus[51] +  diff_51 * diff_51
			}

			int index52 = j * 64 + 52;
			if(index52 != 0 || index52  < size0){
				float diff_52 = u.u52[base_index+j] - u2.u52[base_index+j];
				arr_focus[52] = arr_focus[52] +  diff_52 * diff_52
			}

			int index53 = j * 64 + 53;
			if(index53 != 0 || index53  < size0){
				float diff_53 = u.u53[base_index+j] - u2.u53[base_index+j];
				arr_focus[53] = arr_focus[53] +  diff_53 * diff_53
			}

			int index54 = j * 64 + 54;
			if(index54 != 0 || index54  < size0){
				float diff_54 = u.u54[base_index+j] - u2.u54[base_index+j];
				arr_focus[54] = arr_focus[54] +  diff_54 * diff_54
			}

			int index55 = j * 64 + 55;
			if(index55 != 0 || index55  < size0){
				float diff_55 = u.u55[base_index+j] - u2.u55[base_index+j];
				arr_focus[55] = arr_focus[55] +  diff_55 * diff_55
			}

			int index56 = j * 64 + 56;
			if(index56 != 0 || index56  < size0){
				float diff_56 = u.u56[base_index+j] - u2.u56[base_index+j];
				arr_focus[56] = arr_focus[56] +  diff_56 * diff_56
			}

			int index57 = j * 64 + 57;
			if(index57 != 0 || index57  < size0){
				float diff_57 = u.u57[base_index+j] - u2.u57[base_index+j];
				arr_focus[57] = arr_focus[57] +  diff_57 * diff_57
			}

			int index58 = j * 64 + 58;
			if(index58 != 0 || index58  < size0){
				float diff_58 = u.u58[base_index+j] - u2.u58[base_index+j];
				arr_focus[58] = arr_focus[58] +  diff_58 * diff_58
			}

			int index59 = j * 64 + 59;
			if(index59 != 0 || index59  < size0){
				float diff_59 = u.u59[base_index+j] - u2.u59[base_index+j];
				arr_focus[59] = arr_focus[59] +  diff_59 * diff_59
			}

			int index60 = j * 64 + 60;
			if(index60 != 0 || index60  < size0){
				float diff_60 = u.u60[base_index+j] - u2.u60[base_index+j];
				arr_focus[60] = arr_focus[60] +  diff_60 * diff_60
			}

			int index61 = j * 64 + 61;
			if(index61 != 0 || index61  < size0){
				float diff_61 = u.u61[base_index+j] - u2.u61[base_index+j];
				arr_focus[61] = arr_focus[61] +  diff_61 * diff_61
			}

			int index62 = j * 64 + 62;
			if(index62 != 0 || index62  < size0){
				float diff_62 = u.u62[base_index+j] - u2.u62[base_index+j];
				arr_focus[62] = arr_focus[62] +  diff_62 * diff_62
			}

			int index63 = j * 64 + 63;
			if(index63 != 0 || index63  < size0){
				float diff_63 = u.u63[base_index+j] - u2.u63[base_index+j];
				arr_focus[63] = arr_focus[63] +  diff_63 * diff_63
			}

		}
	}

		float sum32[P_FACTOR/2] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/2)))
		for(int k = 0; k < P_FACTOR/2; k++){
			sum32[k] = arr_focus[2*k] + arr_focus[2*k + 1];
		}

		float sum16[P_FACTOR/4] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/4)))
		for(int k = 0; k < P_FACTOR/4; k++){
			sum16[k] = sum32[2*k] + sum32[2*k + 1];
		}

		float sum8[P_FACTOR/8] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/8)))
		for(int k = 0; k < P_FACTOR/8; k++){
			sum8[k] = sum16[2*k] + sum16[2*k + 1];
		}

		float sum4[P_FACTOR/16] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/16)))
		for(int k = 0; k < P_FACTOR/16; k++){
			sum4[k] = sum8[2*k] + sum8[2*k + 1];
		}

		float sum2[P_FACTOR/32] __attribute__((xcl_array_partition(complete, 1)));
		__attribute__((xcl_pipeline_loop))
		__attribute__((opencl_unroll_hint(P_FACTOR/32)))
		for(int k = 0; k < P_FACTOR/32; k++){
			sum2[k] = sum4[2*k] + sum4[2*k + 1];
		}

		float sum = sum2[0] + sum2[1];
		g_sum = sum;
		arg2[r_bytes2] = g_sum;
}