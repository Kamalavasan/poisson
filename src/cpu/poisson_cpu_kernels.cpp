//
// auto-generated by ops.py//

//header
#define OPS_2D
#define OPS_API 2
#include "../../ops/c/include/ops_lib_cpp.h"
#ifdef OPS_MPI
#include "ops_mpi_core.h"
#endif
#include "../OpenCL/user_types.h"

// global constants
extern double dx;
extern double dy;

//void ops_init_backend() {}

//user kernel files
//#include "poisson_kernel_populate_cpu_kernel.cpp"
#include "poisson_kernel_update_cpu_kernel.cpp"
#include "poisson_kernel_initialguess_cpu_kernel.cpp"
#include "poisson_kernel_stencil_cpu_kernel.cpp"
#include "poisson_kernel_error_cpu_kernel.cpp"
