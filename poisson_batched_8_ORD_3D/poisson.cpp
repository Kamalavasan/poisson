/*
* Open source copyright declaration based on BSD open source template:
* http://www.opensource.org/licenses/bsd-license.php
*
* This file is part of the OPS distribution.
*
* Copyright (c) 2013, Mike Giles and others. Please see the AUTHORS file in
* the main source directory for a full list of copyright holders.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * The name of Mike Giles may not be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mike Giles ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mike Giles BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** @Test application for multi-block functionality
  * @author Gihan Mudalige, Istvan Reguly
  */

// standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float dx,dy,dz;


// OPS header file
#define OPS_2D
#include "ops_seq_v2.h"
#include "user_types.h"
#include "poisson_kernel.h"

/******************************************************************************
* Main program
*******************************************************************************/
int main(int argc, char **argv)
{
  /**-------------------------- Initialisation --------------------------**/

  // OPS initialisation
  ops_init(argc,argv,1);


  //Mesh
  int logical_size_x = 20;
  int logical_size_y = 20;
  int logical_size_z = 20;

  int ngrid_x = 1;
  int ngrid_y = 1;
  int ngrid_z = 1;
  int n_iter = 10;
  int itertile = n_iter;
  int non_copy = 0;

  int num_systems = 100;

  const char* pch;
  for ( int n = 1; n < argc; n++ ) {
    pch = strstr(argv[n], "-sizex=");
    if(pch != NULL) {
      logical_size_x = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-sizey=");
    if(pch != NULL) {
      logical_size_y = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-sizez=");
    if(pch != NULL) {
      logical_size_z = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-iters=");
    if(pch != NULL) {
      n_iter = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-itert=");
    if(pch != NULL) {
      itertile = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-batch=");
    if(pch != NULL) {
      num_systems = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-non-copy");
    if(pch != NULL) {
      non_copy = 1; continue;
    }
  }

  ops_printf("Grid: %dx%d in %dx%d blocks, %d iterations, %d tile height\n",logical_size_x,logical_size_y,ngrid_x,ngrid_y,n_iter,itertile);
  dx = 0.01;
  dy = 0.01;
  dz = 0.01;
  ops_decl_const("dx",1,"float",&dx);
  ops_decl_const("dy",1,"float",&dy);
  ops_decl_const("dz",1,"float",&dz);

  //declare blocks
  char buf[50];
  sprintf(buf,"block");
  ops_block blocks = ops_decl_block_batch(3,"block", num_systems, OPS_BATCHED);


  //declare stencils
  int s3D_00[]         = {0,0,0};
  ops_stencil S3D_00 = ops_decl_stencil( 3, 1, s3D_00, "00");
  int s3D_00_P10_M10_0P1_0M1[27]; 
  for(int i =-4; i <= 4; i++){
    s3D_00_P10_M10_0P1_0M1[i];
    s3D_00_P10_M10_0P1_0M1[0];
    s3D_00_P10_M10_0P1_0M1[0];

    s3D_00_P10_M10_0P1_0M1[0];
    s3D_00_P10_M10_0P1_0M1[i];
    s3D_00_P10_M10_0P1_0M1[0];

    s3D_00_P10_M10_0P1_0M1[0];
    s3D_00_P10_M10_0P1_0M1[0];
    s3D_00_P10_M10_0P1_0M1[i];


  }

  ops_stencil S3D_00_P10_M10_0P1_0M1 = ops_decl_stencil( 3, 27, s3D_00_P10_M10_0P1_0M1, "00:10:-10:01:0-1");

  ops_reduction red_err = ops_decl_reduction_handle_batch(sizeof(float), "float", "err", num_systems);

  //declare datasets
  int d_p[3] = {1,1,1}; //max halo depths for the dat in the possitive direction
  int d_m[3] = {-1,-1, -1}; //max halo depths for the dat in the negative direction
  int base[3] = {0,0,0};
  int uniform_size[3] = {logical_size_x, logical_size_y, logical_size_z};
  float* temp = NULL;
  int *sizes = (int*)malloc(3*sizeof(int));
  int *disps = (int*)malloc(3*sizeof(int));



      //printf("%d, %d\n", size[0],size[1]);
  int size[3] = {uniform_size[0], uniform_size[1], uniform_size[2]};

  sprintf(buf,"coordx %d,%d",0,0);
  ops_dat coordx = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"coordy %d,%d",0,0);
  ops_dat coordy = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"coordz %d,%d",0,0);
  ops_dat coordz = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"u %d,%d",0,0);
  ops_dat u = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"u2 %d,%d",0,0);
  ops_dat u2 = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"f %d,%d",0,0);
  ops_dat f = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sprintf(buf,"ref %d,%d",0,0);
  ops_dat ref = ops_decl_dat(blocks, 1, size, base, d_m, d_p, temp, "float", buf);

  sizes[0]   = size[0];
  sizes[1]   = size[1];
  sizes[2]   = size[2];
  disps[0]   = uniform_size[0];
  disps[1]   = uniform_size[1];
  disps[2]   = uniform_size[2];


  /*for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      printf("sizes %d, %d\n", sizes[2*(i+ngrid_x*j)],sizes[2*(i+ngrid_x*j)+1]);
      printf("disps %d, %d\n", disps[2*(i+ngrid_x*j)],disps[2*(i+ngrid_x*j)+1]);
    }
  }*/


  ops_partition("");
  ops_checkpointing_init("check.h5", 5.0, 0);
	ops_diagnostic_output();
  /**-------------------------- Computations --------------------------**/

  ops_par_loop_blocks_all(num_systems);


  double ct0, ct1, et0, et1;
  ops_timers(&ct0, &et0);

  //populate forcing, reference solution and boundary conditions

    int iter_range1[] = {-1,sizes[0]+1,-1,sizes[1]+1, -1, sizes[2]+1};

    //for (int k = 0;k<4; k++)
      //printf("iter_range[%d] = %d\n",k,iter_range[k]);

    ops_par_loop(poisson_kernel_populate, "poisson_kernel_populate", blocks, 3, iter_range1,
             ops_arg_gbl(&disps[0], 1, "int", OPS_READ),
             ops_arg_gbl(&disps[1], 1, "int", OPS_READ),
             ops_arg_idx(),
             ops_arg_dat(u, 1, S3D_00, "float", OPS_WRITE),
             ops_arg_dat(f, 1, S3D_00, "float", OPS_WRITE),
             ops_arg_dat(ref, 1, S3D_00, "float", OPS_WRITE));

		ops_par_loop(poisson_kernel_update, "poisson_kernel_update", blocks, 3, iter_range1,
						ops_arg_dat(u, 1, S3D_00, "float", OPS_READ),
						ops_arg_dat(u2, 1, S3D_00, "float", OPS_WRITE));



  //initial guess 0

    int iter_range2[] = {0,sizes[0],0,sizes[1], 0, sizes[2]};

    //for (int k = 0;k<4; k++)
      //printf("iter_range[%d] = %d\n",k,iter_range[k]);

    ops_par_loop(poisson_kernel_initialguess, "poisson_kernel_initialguess", blocks, 3, iter_range2,
             ops_arg_dat(u, 1, S3D_00, "float", OPS_WRITE));


  double it0, it1;
  ops_timers(&ct0, &it0);

  for (int iter = 0; iter < n_iter; iter++) {
    if (iter%itertile == 0) ops_execute();


    ops_par_loop(poisson_kernel_stencil, "poisson_kernel_stencil", blocks, 3, iter_range2,
             ops_arg_dat(u, 1, S3D_00_P10_M10_0P1_0M1, "float", OPS_READ),
             ops_arg_dat(u2, 1, S3D_00, "float", OPS_WRITE));


		if (non_copy) {
			ops_par_loop(poisson_kernel_stencil, "poisson_kernel_stencil", blocks, 3, iter_range2,
					ops_arg_dat(u2, 1, S3D_00_P10_M10_0P1_0M1, "float", OPS_READ),
					ops_arg_dat(u, 1, S3D_00, "float", OPS_WRITE));

		} else {
			ops_par_loop(poisson_kernel_update, "poisson_kernel_update", blocks, 3, iter_range2,
					ops_arg_dat(u2, 1, S3D_00, "float", OPS_READ),
					ops_arg_dat(u , 1, S3D_00, "float", OPS_WRITE));
		}
//    if (iter == 5) u[0] = ops_dat_copy(u[0]); //TESTING
  }
	ops_execute();
  ops_timers(&ct0, &it1);

  //ops_print_dat_to_txtfile(u[0], "poisson.dat");
  //ops_print_dat_to_txtfile(ref[0], "poisson.dat");
  //exit(0);

  float err = 0.0;

  ops_par_loop(poisson_kernel_error, "poisson_kernel_error", blocks, 3, iter_range2,
           ops_arg_dat(u, 1, S3D_00, "float", OPS_READ),
           ops_arg_dat(ref , 1, S3D_00, "float", OPS_READ),
           ops_arg_reduce(red_err, 1, "float", OPS_INC));

  ops_reduction_result(red_err,&err);

  ops_timers(&ct1, &et1);
  ops_timing_output(stdout);
  ops_printf("\nTotal Wall time %lf\n",et1-et0);
  float err_diff=fabs((100.0*(err/20.727007094619303))-100.0);
  ops_printf("Total error: %3.15g\n",err);
  ops_printf("Total error is within %3.15E %% of the expected error\n",err_diff);

  if(err_diff < 0.001) {
    ops_printf("This run is considered PASSED\n");
  }
  else {
    ops_printf("This test is considered FAILED\n");
  }

  ops_printf("%lf\n",it1-it0);


  // free(coordx);
  // free(coordy);
  // free(coordy);
  // free(u);
  // free(u2);
  // free(f);
  // free(ref);
  // free(sizes);
  // free(disps);

  // free(blocks);


  ops_exit();
  return 0;
}
