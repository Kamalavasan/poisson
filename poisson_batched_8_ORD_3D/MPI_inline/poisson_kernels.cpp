//
// auto-generated by ops.py
//

#include "./MPI_inline/poisson_common.h"

void ops_init_backend() {}

void ops_decl_const_char2(int dim, char const *type, int size, char *dat,
                          char const *name) {
  if (!strcmp(name, "dx")) {
    dx = *(double *)dat;
  } else if (!strcmp(name, "dy")) {
    dy = *(double *)dat;
  } else if (!strcmp(name, "dz")) {
    dz = *(double *)dat;
  } else {
    printf("error: unknown const name\n");
    exit(1);
  }
}

// user kernel files
#include "poisson_kernel_error_mpiinline_kernel.cpp"
#include "poisson_kernel_initialguess_mpiinline_kernel.cpp"
#include "poisson_kernel_populate_mpiinline_kernel.cpp"
#include "poisson_kernel_stencil_mpiinline_kernel.cpp"
#include "poisson_kernel_update_mpiinline_kernel.cpp"
