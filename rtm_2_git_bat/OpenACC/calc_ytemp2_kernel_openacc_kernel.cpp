//
// auto-generated by ops.py
//

#define OPS_GPU

extern int xdim5_calc_ytemp2_kernel;
int xdim5_calc_ytemp2_kernel_h = -1;
extern int ydim5_calc_ytemp2_kernel;
int ydim5_calc_ytemp2_kernel_h = -1;
extern int xdim6_calc_ytemp2_kernel;
int xdim6_calc_ytemp2_kernel_h = -1;
extern int ydim6_calc_ytemp2_kernel;
int ydim6_calc_ytemp2_kernel_h = -1;
extern int xdim7_calc_ytemp2_kernel;
int xdim7_calc_ytemp2_kernel_h = -1;
extern int ydim7_calc_ytemp2_kernel;
int ydim7_calc_ytemp2_kernel_h = -1;

#ifdef __cplusplus
extern "C" {
#endif
void calc_ytemp2_kernel_c_wrapper(int p_a0, int p_a1, int p_a2, int *p_a3,
                                  float p_a4, float *p_a5, float *p_a6,
                                  float *p_a7, int arg_idx0, int arg_idx1,
                                  int arg_idx2, int x_size, int y_size,
                                  int z_size);

#ifdef __cplusplus
}
#endif

// host stub function
void ops_par_loop_calc_ytemp2_kernel(char const *name, ops_block block, int dim,
                                     int *range, ops_arg arg0, ops_arg arg1,
                                     ops_arg arg2, ops_arg arg3, ops_arg arg4,
                                     ops_arg arg5, ops_arg arg6, ops_arg arg7) {

  // Timing
  double t1, t2, c1, c2;
  ops_arg args[8] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7};

#ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 8, range, 3))
    return;
#endif

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timing_realloc(3, "calc_ytemp2_kernel");
    OPS_instance::getOPSInstance()->OPS_kernels[3].count++;
    ops_timers_core(&c1, &t1);
  }

  // compute localy allocated range for the sub-block

  int start[3];
  int end[3];
#ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
#endif // OPS_MPI

  int arg_idx[3];
  int arg_idx_base[3];
#ifdef OPS_MPI
  if (compute_ranges(args, 8, block, range, start, end, arg_idx) < 0)
    return;
#else // OPS_MPI
  for (int n = 0; n < 3; n++) {
    start[n] = range[2 * n];
    end[n] = range[2 * n + 1];
    arg_idx[n] = start[n];
  }
#endif
  for (int n = 0; n < 3; n++) {
    arg_idx_base[n] = arg_idx[n];
  }

  int dat5 = args[5].dat->elem_size;
  int dat6 = args[6].dat->elem_size;
  int dat7 = args[7].dat->elem_size;

  // set up initial pointers
  int *p_a0 = (int *)args[0].data;
  int *p_a1 = (int *)args[1].data;
  int *p_a2 = (int *)args[2].data;
  int *p_a3 = NULL;

  float *p_a4 = (float *)args[4].data;
  int base5 =
      args[5].dat->base_offset + (OPS_instance::getOPSInstance()->OPS_soa
                                      ? args[5].dat->type_size
                                      : args[5].dat->elem_size) *
                                     start[0] * args[5].stencil->stride[0];
  base5 = base5 +
          (OPS_instance::getOPSInstance()->OPS_soa ? args[5].dat->type_size
                                                   : args[5].dat->elem_size) *
              args[5].dat->size[0] * start[1] * args[5].stencil->stride[1];
  base5 = base5 + (OPS_instance::getOPSInstance()->OPS_soa
                       ? args[5].dat->type_size
                       : args[5].dat->elem_size) *
                      args[5].dat->size[0] * args[5].dat->size[1] * start[2] *
                      args[5].stencil->stride[2];
#ifdef OPS_GPU
  float *p_a5 = (float *)((char *)args[5].data_d + base5);
#else
  float *p_a5 = (float *)((char *)args[5].data + base5);
#endif

  int base6 =
      args[6].dat->base_offset + (OPS_instance::getOPSInstance()->OPS_soa
                                      ? args[6].dat->type_size
                                      : args[6].dat->elem_size) *
                                     start[0] * args[6].stencil->stride[0];
  base6 = base6 +
          (OPS_instance::getOPSInstance()->OPS_soa ? args[6].dat->type_size
                                                   : args[6].dat->elem_size) *
              args[6].dat->size[0] * start[1] * args[6].stencil->stride[1];
  base6 = base6 + (OPS_instance::getOPSInstance()->OPS_soa
                       ? args[6].dat->type_size
                       : args[6].dat->elem_size) *
                      args[6].dat->size[0] * args[6].dat->size[1] * start[2] *
                      args[6].stencil->stride[2];
#ifdef OPS_GPU
  float *p_a6 = (float *)((char *)args[6].data_d + base6);
#else
  float *p_a6 = (float *)((char *)args[6].data + base6);
#endif

  int base7 =
      args[7].dat->base_offset + (OPS_instance::getOPSInstance()->OPS_soa
                                      ? args[7].dat->type_size
                                      : args[7].dat->elem_size) *
                                     start[0] * args[7].stencil->stride[0];
  base7 = base7 +
          (OPS_instance::getOPSInstance()->OPS_soa ? args[7].dat->type_size
                                                   : args[7].dat->elem_size) *
              args[7].dat->size[0] * start[1] * args[7].stencil->stride[1];
  base7 = base7 + (OPS_instance::getOPSInstance()->OPS_soa
                       ? args[7].dat->type_size
                       : args[7].dat->elem_size) *
                      args[7].dat->size[0] * args[7].dat->size[1] * start[2] *
                      args[7].stencil->stride[2];
#ifdef OPS_GPU
  float *p_a7 = (float *)((char *)args[7].data_d + base7);
#else
  float *p_a7 = (float *)((char *)args[7].data + base7);
#endif

  int x_size = MAX(0, end[0] - start[0]);
  int y_size = MAX(0, end[1] - start[1]);
  int z_size = MAX(0, end[2] - start[2]);

  // initialize global variable with the dimension of dats
  int xdim5 = args[5].dat->size[0];
  int ydim5 = args[5].dat->size[1];
  int xdim6 = args[6].dat->size[0];
  int ydim6 = args[6].dat->size[1];
  int xdim7 = args[7].dat->size[0];
  int ydim7 = args[7].dat->size[1];
  if (xdim5 != xdim5_calc_ytemp2_kernel_h ||
      ydim5 != ydim5_calc_ytemp2_kernel_h ||
      xdim6 != xdim6_calc_ytemp2_kernel_h ||
      ydim6 != ydim6_calc_ytemp2_kernel_h ||
      xdim7 != xdim7_calc_ytemp2_kernel_h ||
      ydim7 != ydim7_calc_ytemp2_kernel_h) {
    xdim5_calc_ytemp2_kernel = xdim5;
    xdim5_calc_ytemp2_kernel_h = xdim5;
    ydim5_calc_ytemp2_kernel = ydim5;
    ydim5_calc_ytemp2_kernel_h = ydim5;
    xdim6_calc_ytemp2_kernel = xdim6;
    xdim6_calc_ytemp2_kernel_h = xdim6;
    ydim6_calc_ytemp2_kernel = ydim6;
    ydim6_calc_ytemp2_kernel_h = ydim6;
    xdim7_calc_ytemp2_kernel = xdim7;
    xdim7_calc_ytemp2_kernel_h = xdim7;
    ydim7_calc_ytemp2_kernel = ydim7;
    ydim7_calc_ytemp2_kernel_h = ydim7;
  }

  // Halo Exchanges

#ifdef OPS_GPU
  ops_H_D_exchanges_device(args, 8);
#else
  ops_H_D_exchanges_host(args, 8);
#endif
  ops_halo_exchanges(args, 8, range);

#ifdef OPS_GPU
  ops_H_D_exchanges_device(args, 8);
#else
  ops_H_D_exchanges_host(args, 8);
#endif
  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timers_core(&c2, &t2);
    OPS_instance::getOPSInstance()->OPS_kernels[3].mpi_time += t2 - t1;
  }

  calc_ytemp2_kernel_c_wrapper(*p_a0, *p_a1, *p_a2, p_a3, *p_a4, p_a5, p_a6,
                               p_a7, arg_idx[0], arg_idx[1], arg_idx[2], x_size,
                               y_size, z_size);

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timers_core(&c1, &t1);
    OPS_instance::getOPSInstance()->OPS_kernels[3].time += t1 - t2;
  }
#ifdef OPS_GPU
  ops_set_dirtybit_device(args, 8);
#else
  ops_set_dirtybit_host(args, 8);
#endif
  ops_set_halo_dirtybit3(&args[6], range);
  ops_set_halo_dirtybit3(&args[7], range);

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    // Update kernel record
    ops_timers_core(&c2, &t2);
    OPS_instance::getOPSInstance()->OPS_kernels[3].mpi_time += t2 - t1;
    OPS_instance::getOPSInstance()->OPS_kernels[3].transfer +=
        ops_compute_transfer(dim, start, end, &arg5);
    OPS_instance::getOPSInstance()->OPS_kernels[3].transfer +=
        ops_compute_transfer(dim, start, end, &arg6);
    OPS_instance::getOPSInstance()->OPS_kernels[3].transfer +=
        ops_compute_transfer(dim, start, end, &arg7);
  }
}
