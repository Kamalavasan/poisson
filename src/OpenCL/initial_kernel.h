__attribute__((xcl_dataflow))
__attribute__((always_inline))
static void ops_poisson_kernel_initial(
		struct bram_cluster u,
		const int base0,
		const int size0,
		const int size1){

	int end_loc =  (size0 >> 6) + 1;
	const int row_block = MAX_X_DIM/P_FACTOR;

	for(int i  = 0; i < size1; i++){

		int base_index = base0  + i * row_block;
		__attribute__((xcl_pipeline_loop))
		for(int j = 0; j < end_loc ; j++){

			int index0 = j * 64 + 0;
			if(index0 != 0 || index0  < size0)
				u.u0[base_index+j] = 0;


			int index1 = j * 64 + 1;
			if(index1 != 0 || index1  < size0)
				u.u1[base_index+j] = 0;


			int index2 = j * 64 + 2;
			if(index2 != 0 || index2  < size0)
				u.u2[base_index+j] = 0;


			int index3 = j * 64 + 3;
			if(index3 != 0 || index3  < size0)
				u.u3[base_index+j] = 0;


			int index4 = j * 64 + 4;
			if(index4 != 0 || index4  < size0)
				u.u4[base_index+j] = 0;


			int index5 = j * 64 + 5;
			if(index5 != 0 || index5  < size0)
				u.u5[base_index+j] = 0;


			int index6 = j * 64 + 6;
			if(index6 != 0 || index6  < size0)
				u.u6[base_index+j] = 0;


			int index7 = j * 64 + 7;
			if(index7 != 0 || index7  < size0)
				u.u7[base_index+j] = 0;


			int index8 = j * 64 + 8;
			if(index8 != 0 || index8  < size0)
				u.u8[base_index+j] = 0;


			int index9 = j * 64 + 9;
			if(index9 != 0 || index9  < size0)
				u.u9[base_index+j] = 0;


			int index10 = j * 64 + 10;
			if(index10 != 0 || index10  < size0)
				u.u10[base_index+j] = 0;


			int index11 = j * 64 + 11;
			if(index11 != 0 || index11  < size0)
				u.u11[base_index+j] = 0;


			int index12 = j * 64 + 12;
			if(index12 != 0 || index12  < size0)
				u.u12[base_index+j] = 0;


			int index13 = j * 64 + 13;
			if(index13 != 0 || index13  < size0)
				u.u13[base_index+j] = 0;


			int index14 = j * 64 + 14;
			if(index14 != 0 || index14  < size0)
				u.u14[base_index+j] = 0;


			int index15 = j * 64 + 15;
			if(index15 != 0 || index15  < size0)
				u.u15[base_index+j] = 0;


			int index16 = j * 64 + 16;
			if(index16 != 0 || index16  < size0)
				u.u16[base_index+j] = 0;


			int index17 = j * 64 + 17;
			if(index17 != 0 || index17  < size0)
				u.u17[base_index+j] = 0;


			int index18 = j * 64 + 18;
			if(index18 != 0 || index18  < size0)
				u.u18[base_index+j] = 0;


			int index19 = j * 64 + 19;
			if(index19 != 0 || index19  < size0)
				u.u19[base_index+j] = 0;


			int index20 = j * 64 + 20;
			if(index20 != 0 || index20  < size0)
				u.u20[base_index+j] = 0;


			int index21 = j * 64 + 21;
			if(index21 != 0 || index21  < size0)
				u.u21[base_index+j] = 0;


			int index22 = j * 64 + 22;
			if(index22 != 0 || index22  < size0)
				u.u22[base_index+j] = 0;


			int index23 = j * 64 + 23;
			if(index23 != 0 || index23  < size0)
				u.u23[base_index+j] = 0;


			int index24 = j * 64 + 24;
			if(index24 != 0 || index24  < size0)
				u.u24[base_index+j] = 0;


			int index25 = j * 64 + 25;
			if(index25 != 0 || index25  < size0)
				u.u25[base_index+j] = 0;


			int index26 = j * 64 + 26;
			if(index26 != 0 || index26  < size0)
				u.u26[base_index+j] = 0;


			int index27 = j * 64 + 27;
			if(index27 != 0 || index27  < size0)
				u.u27[base_index+j] = 0;


			int index28 = j * 64 + 28;
			if(index28 != 0 || index28  < size0)
				u.u28[base_index+j] = 0;


			int index29 = j * 64 + 29;
			if(index29 != 0 || index29  < size0)
				u.u29[base_index+j] = 0;


			int index30 = j * 64 + 30;
			if(index30 != 0 || index30  < size0)
				u.u30[base_index+j] = 0;


			int index31 = j * 64 + 31;
			if(index31 != 0 || index31  < size0)
				u.u31[base_index+j] = 0;


			int index32 = j * 64 + 32;
			if(index32 != 0 || index32  < size0)
				u.u32[base_index+j] = 0;


			int index33 = j * 64 + 33;
			if(index33 != 0 || index33  < size0)
				u.u33[base_index+j] = 0;


			int index34 = j * 64 + 34;
			if(index34 != 0 || index34  < size0)
				u.u34[base_index+j] = 0;


			int index35 = j * 64 + 35;
			if(index35 != 0 || index35  < size0)
				u.u35[base_index+j] = 0;


			int index36 = j * 64 + 36;
			if(index36 != 0 || index36  < size0)
				u.u36[base_index+j] = 0;


			int index37 = j * 64 + 37;
			if(index37 != 0 || index37  < size0)
				u.u37[base_index+j] = 0;


			int index38 = j * 64 + 38;
			if(index38 != 0 || index38  < size0)
				u.u38[base_index+j] = 0;


			int index39 = j * 64 + 39;
			if(index39 != 0 || index39  < size0)
				u.u39[base_index+j] = 0;


			int index40 = j * 64 + 40;
			if(index40 != 0 || index40  < size0)
				u.u40[base_index+j] = 0;


			int index41 = j * 64 + 41;
			if(index41 != 0 || index41  < size0)
				u.u41[base_index+j] = 0;


			int index42 = j * 64 + 42;
			if(index42 != 0 || index42  < size0)
				u.u42[base_index+j] = 0;


			int index43 = j * 64 + 43;
			if(index43 != 0 || index43  < size0)
				u.u43[base_index+j] = 0;


			int index44 = j * 64 + 44;
			if(index44 != 0 || index44  < size0)
				u.u44[base_index+j] = 0;


			int index45 = j * 64 + 45;
			if(index45 != 0 || index45  < size0)
				u.u45[base_index+j] = 0;


			int index46 = j * 64 + 46;
			if(index46 != 0 || index46  < size0)
				u.u46[base_index+j] = 0;


			int index47 = j * 64 + 47;
			if(index47 != 0 || index47  < size0)
				u.u47[base_index+j] = 0;


			int index48 = j * 64 + 48;
			if(index48 != 0 || index48  < size0)
				u.u48[base_index+j] = 0;


			int index49 = j * 64 + 49;
			if(index49 != 0 || index49  < size0)
				u.u49[base_index+j] = 0;


			int index50 = j * 64 + 50;
			if(index50 != 0 || index50  < size0)
				u.u50[base_index+j] = 0;


			int index51 = j * 64 + 51;
			if(index51 != 0 || index51  < size0)
				u.u51[base_index+j] = 0;


			int index52 = j * 64 + 52;
			if(index52 != 0 || index52  < size0)
				u.u52[base_index+j] = 0;


			int index53 = j * 64 + 53;
			if(index53 != 0 || index53  < size0)
				u.u53[base_index+j] = 0;


			int index54 = j * 64 + 54;
			if(index54 != 0 || index54  < size0)
				u.u54[base_index+j] = 0;


			int index55 = j * 64 + 55;
			if(index55 != 0 || index55  < size0)
				u.u55[base_index+j] = 0;


			int index56 = j * 64 + 56;
			if(index56 != 0 || index56  < size0)
				u.u56[base_index+j] = 0;


			int index57 = j * 64 + 57;
			if(index57 != 0 || index57  < size0)
				u.u57[base_index+j] = 0;


			int index58 = j * 64 + 58;
			if(index58 != 0 || index58  < size0)
				u.u58[base_index+j] = 0;


			int index59 = j * 64 + 59;
			if(index59 != 0 || index59  < size0)
				u.u59[base_index+j] = 0;


			int index60 = j * 64 + 60;
			if(index60 != 0 || index60  < size0)
				u.u60[base_index+j] = 0;


			int index61 = j * 64 + 61;
			if(index61 != 0 || index61  < size0)
				u.u61[base_index+j] = 0;


			int index62 = j * 64 + 62;
			if(index62 != 0 || index62  < size0)
				u.u62[base_index+j] = 0;


			int index63 = j * 64 + 63;
			if(index63 != 0 || index63  < size0)
				u.u63[base_index+j] = 0;
			
		}
	}
}
