//
// auto-generated by ops.py
//



void ops_init_backend();
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../ops/c/src/ops_opencl_rt_support.c"

float dx,dy;


#define OPS_2D
//#include  "ops_lib_cpp.h"
#include "../ops/c/include/ops_lib_cpp.h"


//
// ops_par_loop declarations
//

void ops_par_loop_poisson_kernel_populate(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg,
  ops_arg,
  ops_arg,
  ops_arg,
  ops_arg );

void ops_par_loop_poisson_kernel_update(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg );

void ops_par_loop_poisson_kernel_initial(char const *, ops_block, int , int*,
  ops_arg );

void ops_par_loop_poisson_kernel_stencil(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg );

void ops_par_loop_poisson_kernel_error(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg,
  ops_arg );

void ops_release_program();

#include "OpenCL/user_types.h"
//#include "poisson_kernel.h"

int main(int argc,  char **argv)
{


  ops_init(argc,argv,1);
  ops_init_backend();


  int logical_size_x = 20;
  int logical_size_y = 20;
  int ngrid_x = 1;
  int ngrid_y = 1;
  int n_iter = 10;
  int itertile = n_iter;
  int non_copy = 0;

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
    pch = strstr(argv[n], "-iters=");
    if(pch != NULL) {
      n_iter = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-itert=");
    if(pch != NULL) {
      itertile = atoi ( argv[n] + 7 ); continue;
    }
    pch = strstr(argv[n], "-non-copy");
    if(pch != NULL) {
      non_copy = 1; continue;
    }
  }

  ops_printf("Grid: %dx%d in %dx%d blocks, %d iterations, %d tile height\n",logical_size_x,logical_size_y,ngrid_x,ngrid_y,n_iter,itertile);
  dx = 0.01f;
  dy = 0.01f;
  ops_decl_const2( "dx",1, "float",&dx);
  ops_decl_const2( "dy",1, "float",&dy);

  ops_block *blocks = (ops_block *)malloc(ngrid_x*ngrid_y*sizeof(ops_block*));
  char buf[50];
  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      sprintf(buf,"block %d,%d",i,j);
      blocks[i+ngrid_x*j] = ops_decl_block(2,buf);
    }
  }

  int s2D_00[]         = {0,0};
  ops_stencil S2D_00 = ops_decl_stencil( 2, 1, s2D_00, "00");
  int s2D_00_P10_M10_0P1_0M1[]         = {0,0, 1,0, -1,0, 0,1, 0,-1};
  ops_stencil S2D_00_P10_M10_0P1_0M1 = ops_decl_stencil( 2, 5, s2D_00_P10_M10_0P1_0M1, "00:10:-10:01:0-1");

  ops_reduction red_err = ops_decl_reduction_handle(sizeof(float), "float", "err");

  int d_p[2] = {1,1};
  int d_m[2] = {-1,-1};
  int base[2] = {0,0};
  int uniform_size[2] = {(logical_size_x-1)/ngrid_x+1,(logical_size_y-1)/ngrid_y+1};
  float* temp = NULL;
  ops_dat *coordx = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  ops_dat *coordy = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  ops_dat *u = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  ops_dat *u2 = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  ops_dat *f = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  ops_dat *ref = (ops_dat *)malloc(ngrid_x*ngrid_y*sizeof(ops_dat*));
  int *sizes = (int*)malloc(2*ngrid_x*ngrid_y*sizeof(int));
  int *disps = (int*)malloc(2*ngrid_x*ngrid_y*sizeof(int));

  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      int size[2] = {uniform_size[0], uniform_size[1]};
      if ((i+1)*size[0]>logical_size_x) size[0] = logical_size_x - i*size[0];
      if ((j+1)*size[1]>logical_size_y) size[1] = logical_size_y - j*size[1];

      printf("size[0]: %d, size[1]: %d\n", size[0], size[1]);

      sprintf(buf,"coordx %d,%d",i,j);
      coordx[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);
      sprintf(buf,"coordy %d,%d",i,j);
      coordy[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);
      sprintf(buf,"u %d,%d",i,j);
      u[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);
      sprintf(buf,"u2 %d,%d",i,j);
      u2[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);
      sprintf(buf,"f %d,%d",i,j);
      f[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);
      sprintf(buf,"ref %d,%d",i,j);
      ref[i+ngrid_x*j] = ops_decl_dat(blocks[i+ngrid_x*j], 1, size, base, d_m, d_p, temp, "float", buf);

      sizes[2*(i+ngrid_x*j)]   = size[0];
      sizes[2*(i+ngrid_x*j)+1] = size[1];
      disps[2*(i+ngrid_x*j)]   = i*uniform_size[0];
      disps[2*(i+ngrid_x*j)+1] = j*uniform_size[1];
    }
  }


  ops_halo *halos = (ops_halo *)malloc(2*(ngrid_x*(ngrid_y-1)+(ngrid_x-1)*ngrid_y)*sizeof(ops_halo));
  int off = 0;
  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      if (i > 0) {
        int halo_iter[] = {1,sizes[2*(i+ngrid_x*j)+1]};
        int base_from[] = {sizes[2*(i-1+ngrid_x*j)]-1,0};
        int base_to[] = {-1,0};
        int dir[] = {1,2};

        halos[off++] = ops_decl_halo(u[i-1+ngrid_x*j], u[i+ngrid_x*j], halo_iter, base_from, base_to, dir, dir);
        base_from[0] = 0; base_to[0] = sizes[2*(i+ngrid_x*j)];
        halos[off++] = ops_decl_halo(u[i+ngrid_x*j], u[i-1+ngrid_x*j], halo_iter, base_from, base_to, dir, dir);
      }
      if (j > 0) {
        int halo_iter[] = {sizes[2*(i+ngrid_x*j)],1};
        int base_from[] = {0,sizes[2*(i+ngrid_x*(j-1))+1]-1};
        int base_to[] = {0,-1};
        int dir[] = {1,2};

        halos[off++] = ops_decl_halo(u[i+ngrid_x*(j-1)], u[i+ngrid_x*j], halo_iter, base_from, base_to, dir, dir);
        base_from[1] = 0; base_to[1] = sizes[2*(i+ngrid_x*j)+1];
        halos[off++] = ops_decl_halo(u[i+ngrid_x*j], u[i+ngrid_x*(j-1)], halo_iter, base_from, base_to, dir, dir);
      }
    }
  }
  if (off != 2*(ngrid_x*(ngrid_y-1)+(ngrid_x-1)*ngrid_y)) printf("Something is not right\n");
  ops_halo_group u_halos = ops_decl_halo_group(off,halos);

  ops_partition("");
  ops_checkpointing_init("check.h5", 5.0, 0);
	ops_diagnostic_output();


  double ct0, ct1, et0, et1;
  ops_timers(&ct0, &et0);

  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      int iter_range[] = {-1,sizes[2*(i+ngrid_x*j)]+1,-1,sizes[2*(i+ngrid_x*j)+1]+1};



      ops_par_loop_poisson_kernel_populate("poisson_kernel_populate", blocks[i+ngrid_x*j], 2, iter_range,
                   ops_arg_gbl(&disps[2*(i+ngrid_x*j)], 1, "int", OPS_READ),
                   ops_arg_gbl(&disps[2*(i+ngrid_x*j)+1], 1, "int", OPS_READ),
                   ops_arg_idx(),
                   ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE),
                   ops_arg_dat(f[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE),
                   ops_arg_dat(ref[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));


//      ops_print_dat_to_txtfile((f[i+ngrid_x*j]), "/home/vasan/faraz/poisson_1/f.txt");
//      ops_print_dat_to_txtfile((ref[i+ngrid_x*j]), "/home/vasan/faraz/poisson_1/ref.txt");

		 ops_par_loop_poisson_kernel_update("poisson_kernel_update", blocks[i+ngrid_x*j], 2, iter_range,
                ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_READ),
                ops_arg_dat(u2[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));

    }
  }

  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      int iter_range[] = {0,sizes[2*(i+ngrid_x*j)],0,sizes[2*(i+ngrid_x*j)+1]};


//
      ops_par_loop_poisson_kernel_initial("poisson_kernel_initialguess", blocks[i+ngrid_x*j], 2, iter_range,
                   ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));

    }
  }

  double it0, it1;
  ops_timers(&ct0, &it0);

  for (int iter = 0; iter < n_iter; iter++) {
    if (ngrid_x>1 || ngrid_y>1) ops_halo_transfer(u_halos);
    if (iter%itertile == 0) ops_execute();

    for (int j = 0; j < ngrid_y; j++) {
      for (int i = 0; i < ngrid_x; i++) {
        int iter_range[] = {0,sizes[2*(i+ngrid_x*j)],0,sizes[2*(i+ngrid_x*j)+1]};


        ops_par_loop_poisson_kernel_stencil("poisson_kernel_stencil", blocks[i+ngrid_x*j], 2, iter_range,
                     ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00_P10_M10_0P1_0M1, "float", OPS_READ),
                     ops_arg_dat(u2[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));

//        ops_print_dat_to_txtfile((u2[i+ngrid_x*j]), "u.txt");


      }
    }



		if (non_copy) {
			for (int j = 0; j < ngrid_y; j++) {
				for (int i = 0; i < ngrid_x; i++) {
					int iter_range[] = {0,sizes[2*(i+ngrid_x*j)],0,sizes[2*(i+ngrid_x*j)+1]};
				 ops_par_loop_poisson_kernel_stencil("poisson_kernel_stencil", blocks[i+ngrid_x*j], 2, iter_range,
                  ops_arg_dat(u2[i+ngrid_x*j], 1, S2D_00_P10_M10_0P1_0M1, "float", OPS_READ),
                  ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));
				}
			}
		} else {
			for (int j = 0; j < ngrid_y; j++) {
				for (int i = 0; i < ngrid_x; i++) {
					int iter_range[] = {0,sizes[2*(i+ngrid_x*j)],0,sizes[2*(i+ngrid_x*j)+1]};
				 ops_par_loop_poisson_kernel_update("poisson_kernel_update", blocks[i+ngrid_x*j], 2, iter_range,
                  ops_arg_dat(u2[i+ngrid_x*j], 1, S2D_00, "float", OPS_READ),
                  ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_WRITE));
				}
			}
		}
  }
	ops_execute();
  ops_timers(&ct0, &it1);




  float err = 0.0;
  for (int j = 0; j < ngrid_y; j++) {
    for (int i = 0; i < ngrid_x; i++) {
      int iter_range[] = {0,sizes[2*(i+ngrid_x*j)],0,sizes[2*(i+ngrid_x*j)+1]};
      ops_par_loop_poisson_kernel_error("poisson_kernel_error", blocks[i+ngrid_x*j], 2, iter_range,
                   ops_arg_dat(u[i+ngrid_x*j], 1, S2D_00, "float", OPS_READ),
                   ops_arg_dat(ref[i+ngrid_x*j], 1, S2D_00, "float", OPS_READ),
                   ops_arg_reduce(red_err, 1, "float", OPS_INC));
    }
  }

  ops_reduction_result(red_err,&err);

  ops_timers(&ct1, &et1);
  ops_timing_output(stdout);
  ops_printf("\nTotal Wall time %lf\n",et1-et0);
  float err_diff=fabs((100.0*(err/37.7403193644329))-100.0);
  ops_printf("Total error: %3.15g\n",err);
  ops_printf("Total error is within %3.15E %% of the expected error\n",err_diff);

  if(err_diff < 0.001) {
    ops_printf("This run is considered PASSED\n");
  }
  else {
    ops_printf("This test is considered FAILED\n");
  }

  ops_printf("%lf\n",it1-it0);

  free(coordx);
  free(coordy);
  free(u);
  free(u2);
  free(f);
  free(ref);
  free(sizes);
  free(disps);

  free(blocks);
  free(halos);

  ops_release_program();
  ops_exit();
  return 0;
}
