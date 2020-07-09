//
// auto-generated by ops.py
//

// host stub function
#ifndef OPS_LAZY
void ops_par_loop_rtm_kernel_populate(char const *name, ops_block block,
                                      int dim, int *range, ops_arg arg0,
                                      ops_arg arg1, ops_arg arg2, ops_arg arg3,
                                      ops_arg arg4, ops_arg arg5,
                                      ops_arg arg6) {
  const int blockidx_start = 0;
  const int blockidx_end = block->count;
#ifdef OPS_BATCHED
  const int batch_size = block->count;
#endif
#else
void ops_par_loop_rtm_kernel_populate_execute(const char *name, ops_block block,
                                              int blockidx_start,
                                              int blockidx_end, int dim,
                                              int *range, int nargs,
                                              ops_arg *args) {
#ifdef OPS_BATCHED
  const int batch_size = OPS_BATCH_SIZE;
#endif
  ops_arg arg0 = args[0];
  ops_arg arg1 = args[1];
  ops_arg arg2 = args[2];
  ops_arg arg3 = args[3];
  ops_arg arg4 = args[4];
  ops_arg arg5 = args[5];
  ops_arg arg6 = args[6];
#endif

  // Timing
  double __t1, __t2, __c1, __c2;

#ifndef OPS_LAZY
  ops_arg args[7] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6};

#endif

#if defined(CHECKPOINTING) && !defined(OPS_LAZY)
  if (!ops_checkpointing_before(args, 7, range, 0))
    return;
#endif

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timing_realloc(0, "rtm_kernel_populate");
    OPS_instance::getOPSInstance()->OPS_kernels[0].count++;
    ops_timers_core(&__c2, &__t2);
  }

#ifdef OPS_DEBUG
  ops_register_args(args, "rtm_kernel_populate");
#endif

  // compute locally allocated range for the sub-block
  int start[3];
  int end[3];
  int arg_idx[3];
#if defined(OPS_LAZY) || !defined(OPS_MPI)
  for (int n = 0; n < 3; n++) {
    start[n] = range[2 * n];
    end[n] = range[2 * n + 1];
  }
#else
  if (compute_ranges(args, 7, block, range, start, end, arg_idx) < 0)
    return;
#endif

#ifdef OPS_MPI
  sub_dat_list sd = OPS_sub_dat_list[args[6].dat->index];
  arg_idx[0] = MAX(0, sd->decomp_disp[0]);
  arg_idx[1] = MAX(0, sd->decomp_disp[1]);
  arg_idx[2] = MAX(0, sd->decomp_disp[2]);
#else  // OPS_MPI
  arg_idx[0] = 0;
  arg_idx[1] = 0;
  arg_idx[2] = 0;
#endif // OPS_MPI

// initialize variable with the dimension of dats
#if defined(OPS_BATCHED) && OPS_BATCHED == 0 && defined(OPS_HYBRID_LAYOUT)
  const int xdim4 = OPS_BATCH_SIZE;
  const int xdim5 = OPS_BATCH_SIZE;
  const int xdim6 = OPS_BATCH_SIZE;
#else
  const int xdim4 = args[4].dat->size[0];
  const int xdim5 = args[5].dat->size[0];
  const int xdim6 = args[6].dat->size[0];
#endif
  const int ydim4 = args[4].dat->size[1];
  const int zdim4 = args[4].dat->size[2];
  const int ydim5 = args[5].dat->size[1];
  const int zdim5 = args[5].dat->size[2];
  const int ydim6 = args[6].dat->size[1];
  const int zdim6 = args[6].dat->size[2];
#ifdef OPS_BATCHED
  const int bounds_0_l = OPS_BATCHED == 0 ? 0 : start[(OPS_BATCHED > 0) + -1];
  const int bounds_0_u = OPS_BATCHED == 0
                             ? MIN(batch_size, block->count - blockidx_start)
                             : end[(OPS_BATCHED > 0) + -1];
  const int bounds_1_l = OPS_BATCHED == 1 ? 0 : start[(OPS_BATCHED > 1) + 0];
  const int bounds_1_u = OPS_BATCHED == 1
                             ? MIN(batch_size, block->count - blockidx_start)
                             : end[(OPS_BATCHED > 1) + 0];
  const int bounds_2_l = OPS_BATCHED == 2 ? 0 : start[(OPS_BATCHED > 2) + 1];
  const int bounds_2_u = OPS_BATCHED == 2
                             ? MIN(batch_size, block->count - blockidx_start)
                             : end[(OPS_BATCHED > 2) + 1];
  const int bounds_3_l = OPS_BATCHED == 3 ? 0 : start[(OPS_BATCHED > 3) + 2];
  const int bounds_3_u = OPS_BATCHED == 3
                             ? MIN(batch_size, block->count - blockidx_start)
                             : end[(OPS_BATCHED > 3) + 2];
#else
  const int bounds_0_l = start[0];
  const int bounds_0_u = end[0];
  const int bounds_1_l = start[1];
  const int bounds_1_u = end[1];
  const int bounds_2_l = start[2];
  const int bounds_2_u = end[2];
  const int bounds_3_l = 0;
  const int bounds_3_u = blockidx_end - blockidx_start;
#endif

#ifndef OPS_LAZY
  // Halo Exchanges
  ops_H_D_exchanges_host(args, 7);
  ops_halo_exchanges(args, 7, range);
  ops_H_D_exchanges_host(args, 7);
#endif

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timers_core(&__c1, &__t1);
    OPS_instance::getOPSInstance()->OPS_kernels[0].mpi_time += __t1 - __t2;
  }

  // set up initial pointers
  int *__restrict__ dispx = (int *)args[0].data;

  int *__restrict__ dispy = (int *)args[1].data;

  int *__restrict__ dispz = (int *)args[2].data;

  float *__restrict__ rho_p =
      (float *)(args[4].data + args[4].dat->base_offset +
                blockidx_start * args[4].dat->batch_offset);

  float *__restrict__ mu_p =
      (float *)(args[5].data + args[5].dat->base_offset +
                blockidx_start * args[5].dat->batch_offset);

  float *__restrict__ yy_p =
      (float *)(args[6].data + args[6].dat->base_offset +
                blockidx_start * args[6].dat->batch_offset);

#if defined(_OPENMP) && defined(OPS_BATCHED) && !defined(OPS_LAZY)
#pragma omp parallel for
#endif
  for (int n_3 = bounds_3_l; n_3 < bounds_3_u; n_3++) {
#if defined(_OPENMP) && !defined(OPS_BATCHED)
#pragma omp parallel for collapse(2)
#endif
    for (int n_2 = bounds_2_l; n_2 < bounds_2_u; n_2++) {
      for (int n_1 = bounds_1_l; n_1 < bounds_1_u; n_1++) {
#ifdef __INTEL_COMPILER
#pragma loop_count(10000)
#pragma omp simd
#elif defined(__clang__)
#pragma clang loop vectorize(assume_safety)
#elif defined(__GNUC__)
#pragma simd
#pragma GCC ivdep
#else
#pragma simd
#endif
        for (int n_0 = bounds_0_l; n_0 < bounds_0_u; n_0++) {
#if defined(OPS_BATCHED) && OPS_BATCHED == 0
          int idx[] = {arg_idx[0] + n_1, arg_idx[1] + n_2, arg_idx[2] + n_3,
                       blockidx_start + n_0};
#elif OPS_BATCHED == 1
          int idx[] = {arg_idx[0] + n_0, arg_idx[1] + n_2, arg_idx[2] + n_3,
                       blockidx_start + n_1};
#elif OPS_BATCHED == 2
        int idx[] = {arg_idx[0] + n_0, arg_idx[1] + n_1, arg_idx[2] + n_3,
                     blockidx_start + n_2};
#else
        int idx[] = {arg_idx[0] + n_0, arg_idx[1] + n_1, arg_idx[2] + n_2,
                     blockidx_start + n_3};
#endif
          ACC<float> rho(xdim4, ydim4, zdim4,
                         rho_p + n_0 + n_1 * xdim4 + n_2 * xdim4 * ydim4 +
                             n_3 * xdim4 * ydim4 * zdim4);
          ACC<float> mu(xdim5, ydim5, zdim5,
                        mu_p + n_0 + n_1 * xdim5 + n_2 * xdim5 * ydim5 +
                            n_3 * xdim5 * ydim5 * zdim5);
#ifdef OPS_SOA
          ACC<float> yy(6, xdim6, ydim6, zdim6,
                        yy_p + n_0 + n_1 * xdim6 + n_2 * xdim6 * ydim6 +
                            n_3 * xdim6 * ydim6 * zdim6);
#else
          ACC<float> yy(6, xdim6, ydim6, zdim6,
                        yy_p + 6 * (n_0 + n_1 * xdim6 + n_2 * xdim6 * ydim6 +
                                    n_3 * xdim6 * ydim6 * zdim6));
#endif

          float x = 1.0 * ((float)(idx[0] - nx / 2) / nx);
          float y = 1.0 * ((float)(idx[1] - ny / 2) / ny);
          float z = 1.0 * ((float)(idx[2] - nz / 2) / nz);

          const float C = 1.0f;
          const float r0 = 0.001f;
          rho(0, 0, 0) = 1000.0f;
          mu(0, 0, 0) = 0.001f;

          yy(0, 0, 0, 0) = (1. / 3.) * C * exp(-(x * x + y * y + z * z) / r0);
        }
      }
    }
#if OPS_BATCHED == 3 || !defined(OPS_BATCHED)
#endif
  }
  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timers_core(&__c2, &__t2);
    OPS_instance::getOPSInstance()->OPS_kernels[0].time += __t2 - __t1;
  }
#ifndef OPS_LAZY
  ops_set_dirtybit_host(args, 7);
  ops_set_halo_dirtybit3(&args[4], range);
  ops_set_halo_dirtybit3(&args[5], range);
  ops_set_halo_dirtybit3(&args[6], range);
#endif

  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    // Update kernel record
    ops_timers_core(&__c1, &__t1);
    OPS_instance::getOPSInstance()->OPS_kernels[0].mpi_time += __t1 - __t2;
    OPS_instance::getOPSInstance()->OPS_kernels[0].transfer +=
        ops_compute_transfer(dim, start, end, &arg4);
    OPS_instance::getOPSInstance()->OPS_kernels[0].transfer +=
        ops_compute_transfer(dim, start, end, &arg5);
    OPS_instance::getOPSInstance()->OPS_kernels[0].transfer +=
        ops_compute_transfer(dim, start, end, &arg6);
  }
}

#ifdef OPS_LAZY
void ops_par_loop_rtm_kernel_populate(char const *name, ops_block block,
                                      int dim, int *range, ops_arg arg0,
                                      ops_arg arg1, ops_arg arg2, ops_arg arg3,
                                      ops_arg arg4, ops_arg arg5,
                                      ops_arg arg6) {
  ops_kernel_descriptor *desc =
      (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 0;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 0;
  for (int i = 0; i < 6; i++) {
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 7;
  desc->args = (ops_arg *)malloc(7 * sizeof(ops_arg));
  desc->args[0] = arg0;
  char *tmp = (char *)malloc(1 * sizeof(int));
  memcpy(tmp, arg0.data, 1 * sizeof(int));
  desc->args[0].data = tmp;
  desc->args[1] = arg1;
  tmp = (char *)malloc(1 * sizeof(int));
  memcpy(tmp, arg1.data, 1 * sizeof(int));
  desc->args[1].data = tmp;
  desc->args[2] = arg2;
  tmp = (char *)malloc(1 * sizeof(int));
  memcpy(tmp, arg2.data, 1 * sizeof(int));
  desc->args[2].data = tmp;
  desc->args[3] = arg3;
  desc->args[4] = arg4;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg4.dat->index;
  desc->args[5] = arg5;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg5.dat->index;
  desc->args[6] = arg6;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg6.dat->index;
  desc->function = ops_par_loop_rtm_kernel_populate_execute;
  if (OPS_instance::getOPSInstance()->OPS_diags > 1) {
    ops_timing_realloc(0, "rtm_kernel_populate");
  }
  ops_enqueue_kernel(desc);
}
#endif