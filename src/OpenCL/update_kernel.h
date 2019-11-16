__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_update(
		struct bram_cluster u,
		struct bram_cluster u2,
		const int base0,
		const int base1,
		const int size0,
		const int size1){


	for(int i = 0; i < size1; i++){
		int end_loc =  (size0 >> 6) + 1;
		const int row_block = MAX_X_DIM/P_FACTOR;

		for(int i  = 0; i < size1; i++){

		int base_index = base0  + i * row_block;
		__attribute__((xcl_pipeline_loop))
		for(int j = 0; j < end_loc ; j++){

			int index0 = j * 64 + 0;
			if(index0  < size0)
				u.u0[base_index+j] = u2.u0[base_index+j];

			int index1 = j * 64 + 1;
			if(index1  < size0)
				u.u1[base_index+j] = u2.u1[base_index+j];

			int index2 = j * 64 + 2;
			if(index2  < size0)
				u.u2[base_index+j] = u2.u2[base_index+j];

			int index3 = j * 64 + 3;
			if(index3  < size0)
				u.u3[base_index+j] = u2.u3[base_index+j];

			int index4 = j * 64 + 4;
			if(index4  < size0)
				u.u4[base_index+j] = u2.u4[base_index+j];

			int index5 = j * 64 + 5;
			if(index5  < size0)
				u.u5[base_index+j] = u2.u5[base_index+j];

			int index6 = j * 64 + 6;
			if(index6  < size0)
				u.u6[base_index+j] = u2.u6[base_index+j];

			int index7 = j * 64 + 7;
			if(index7  < size0)
				u.u7[base_index+j] = u2.u7[base_index+j];

			int index8 = j * 64 + 8;
			if(index8  < size0)
				u.u8[base_index+j] = u2.u8[base_index+j];

			int index9 = j * 64 + 9;
			if(index9  < size0)
				u.u9[base_index+j] = u2.u9[base_index+j];

			int index10 = j * 64 + 10;
			if(index10  < size0)
				u.u10[base_index+j] = u2.u10[base_index+j];

			int index11 = j * 64 + 11;
			if(index11  < size0)
				u.u11[base_index+j] = u2.u11[base_index+j];

			int index12 = j * 64 + 12;
			if(index12  < size0)
				u.u12[base_index+j] = u2.u12[base_index+j];

			int index13 = j * 64 + 13;
			if(index13  < size0)
				u.u13[base_index+j] = u2.u13[base_index+j];

			int index14 = j * 64 + 14;
			if(index14  < size0)
				u.u14[base_index+j] = u2.u14[base_index+j];

			int index15 = j * 64 + 15;
			if(index15  < size0)
				u.u15[base_index+j] = u2.u15[base_index+j];

			int index16 = j * 64 + 16;
			if(index16  < size0)
				u.u16[base_index+j] = u2.u16[base_index+j];

			int index17 = j * 64 + 17;
			if(index17  < size0)
				u.u17[base_index+j] = u2.u17[base_index+j];

			int index18 = j * 64 + 18;
			if(index18  < size0)
				u.u18[base_index+j] = u2.u18[base_index+j];

			int index19 = j * 64 + 19;
			if(index19  < size0)
				u.u19[base_index+j] = u2.u19[base_index+j];

			int index20 = j * 64 + 20;
			if(index20  < size0)
				u.u20[base_index+j] = u2.u20[base_index+j];

			int index21 = j * 64 + 21;
			if(index21  < size0)
				u.u21[base_index+j] = u2.u21[base_index+j];

			int index22 = j * 64 + 22;
			if(index22  < size0)
				u.u22[base_index+j] = u2.u22[base_index+j];

			int index23 = j * 64 + 23;
			if(index23  < size0)
				u.u23[base_index+j] = u2.u23[base_index+j];

			int index24 = j * 64 + 24;
			if(index24  < size0)
				u.u24[base_index+j] = u2.u24[base_index+j];

			int index25 = j * 64 + 25;
			if(index25  < size0)
				u.u25[base_index+j] = u2.u25[base_index+j];

			int index26 = j * 64 + 26;
			if(index26  < size0)
				u.u26[base_index+j] = u2.u26[base_index+j];

			int index27 = j * 64 + 27;
			if(index27  < size0)
				u.u27[base_index+j] = u2.u27[base_index+j];

			int index28 = j * 64 + 28;
			if(index28  < size0)
				u.u28[base_index+j] = u2.u28[base_index+j];

			int index29 = j * 64 + 29;
			if(index29  < size0)
				u.u29[base_index+j] = u2.u29[base_index+j];

			int index30 = j * 64 + 30;
			if(index30  < size0)
				u.u30[base_index+j] = u2.u30[base_index+j];

			int index31 = j * 64 + 31;
			if(index31  < size0)
				u.u31[base_index+j] = u2.u31[base_index+j];

			int index32 = j * 64 + 32;
			if(index32  < size0)
				u.u32[base_index+j] = u2.u32[base_index+j];

			int index33 = j * 64 + 33;
			if(index33  < size0)
				u.u33[base_index+j] = u2.u33[base_index+j];

			int index34 = j * 64 + 34;
			if(index34  < size0)
				u.u34[base_index+j] = u2.u34[base_index+j];

			int index35 = j * 64 + 35;
			if(index35  < size0)
				u.u35[base_index+j] = u2.u35[base_index+j];

			int index36 = j * 64 + 36;
			if(index36  < size0)
				u.u36[base_index+j] = u2.u36[base_index+j];

			int index37 = j * 64 + 37;
			if(index37  < size0)
				u.u37[base_index+j] = u2.u37[base_index+j];

			int index38 = j * 64 + 38;
			if(index38  < size0)
				u.u38[base_index+j] = u2.u38[base_index+j];

			int index39 = j * 64 + 39;
			if(index39  < size0)
				u.u39[base_index+j] = u2.u39[base_index+j];

			int index40 = j * 64 + 40;
			if(index40  < size0)
				u.u40[base_index+j] = u2.u40[base_index+j];

			int index41 = j * 64 + 41;
			if(index41  < size0)
				u.u41[base_index+j] = u2.u41[base_index+j];

			int index42 = j * 64 + 42;
			if(index42  < size0)
				u.u42[base_index+j] = u2.u42[base_index+j];

			int index43 = j * 64 + 43;
			if(index43  < size0)
				u.u43[base_index+j] = u2.u43[base_index+j];

			int index44 = j * 64 + 44;
			if(index44  < size0)
				u.u44[base_index+j] = u2.u44[base_index+j];

			int index45 = j * 64 + 45;
			if(index45  < size0)
				u.u45[base_index+j] = u2.u45[base_index+j];

			int index46 = j * 64 + 46;
			if(index46  < size0)
				u.u46[base_index+j] = u2.u46[base_index+j];

			int index47 = j * 64 + 47;
			if(index47  < size0)
				u.u47[base_index+j] = u2.u47[base_index+j];

			int index48 = j * 64 + 48;
			if(index48  < size0)
				u.u48[base_index+j] = u2.u48[base_index+j];

			int index49 = j * 64 + 49;
			if(index49  < size0)
				u.u49[base_index+j] = u2.u49[base_index+j];

			int index50 = j * 64 + 50;
			if(index50  < size0)
				u.u50[base_index+j] = u2.u50[base_index+j];

			int index51 = j * 64 + 51;
			if(index51  < size0)
				u.u51[base_index+j] = u2.u51[base_index+j];

			int index52 = j * 64 + 52;
			if(index52  < size0)
				u.u52[base_index+j] = u2.u52[base_index+j];

			int index53 = j * 64 + 53;
			if(index53  < size0)
				u.u53[base_index+j] = u2.u53[base_index+j];

			int index54 = j * 64 + 54;
			if(index54  < size0)
				u.u54[base_index+j] = u2.u54[base_index+j];

			int index55 = j * 64 + 55;
			if(index55  < size0)
				u.u55[base_index+j] = u2.u55[base_index+j];

			int index56 = j * 64 + 56;
			if(index56  < size0)
				u.u56[base_index+j] = u2.u56[base_index+j];

			int index57 = j * 64 + 57;
			if(index57  < size0)
				u.u57[base_index+j] = u2.u57[base_index+j];

			int index58 = j * 64 + 58;
			if(index58  < size0)
				u.u58[base_index+j] = u2.u58[base_index+j];

			int index59 = j * 64 + 59;
			if(index59  < size0)
				u.u59[base_index+j] = u2.u59[base_index+j];

			int index60 = j * 64 + 60;
			if(index60  < size0)
				u.u60[base_index+j] = u2.u60[base_index+j];

			int index61 = j * 64 + 61;
			if(index61  < size0)
				u.u61[base_index+j] = u2.u61[base_index+j];

			int index62 = j * 64 + 62;
			if(index62  < size0)
				u.u62[base_index+j] = u2.u62[base_index+j];

			int index63 = j * 64 + 63;
			if(index63  < size0)
				u.u63[base_index+j] = u2.u63[base_index+j];


		}
	}

}