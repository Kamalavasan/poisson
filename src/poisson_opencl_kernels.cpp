//
// auto-generated by ops.py//

//header
#define OPS_API 2
#define OPS_2D
#include "stdlib.h"
#include "stdio.h"
//#include "ops_lib_cpp.h"
#include "../ops/c/include/ops_lib_cpp.h"
//#include "ops_opencl_rt_support.h"
#include "../ops/c/include/ops_opencl_rt_support.h"
#include "OpenCL/user_types.h"
#include "xcl2.hpp"

#ifdef OCL_FMA_SWITCH_ON
#define OCL_FMA 1
#else
#define OCL_FMA 0
#endif
//global constants
extern float dx;
extern float dy;

#define x_len 16
#define y_len 1

extern ops_opencl_core OPS_opencl_core;

void ops_release_program(){
	clReleaseCommandQueue(OPS_opencl_core.command_queue);
	clReleaseContext(OPS_opencl_core.context);
	clReleaseDevice(OPS_opencl_core.device_id);
//	//clReleaseKernel(*OPS_opencl_core.kernel);
	clReleaseKernel(OPS_opencl_core.kernel[0]);
//
	clReleaseProgram(OPS_opencl_core.program);
//
	free((OPS_opencl_core.platform_id));
	free((OPS_opencl_core.devices));
	free((OPS_opencl_core.kernel));
	free((OPS_opencl_core.constant));
}

int load_file_to_memory(const char *filename, char **result)
{
	uint size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		*result = NULL;
		return -1; // -1 means file opening fail
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)malloc(size+1);
	if (size != fread(*result, sizeof(char), size, f)) {
		free(*result);
		return -2; // -2 means file reading fail
	}
	fclose(f);
	(*result)[size] = 0;
	return size;
}

void ops_init_backend() {
	cl_int ret;

	char *fpga_bin;
	size_t fpga_bin_size;
	fpga_bin_size = load_file_to_memory("../ops_poisson_kernels.xclbin", &fpga_bin);

	OPS_opencl_core.program = clCreateProgramWithBinary(OPS_opencl_core.context, 1, (const cl_device_id*) &OPS_opencl_core.device_id, &fpga_bin_size, (const unsigned char**) &fpga_bin, NULL, &ret);

	char buildOpts[] = " ";

	ret = clBuildProgram(OPS_opencl_core.program, 1, &OPS_opencl_core.device_id, buildOpts, NULL, NULL);

//	size_t w_size;
//	size_t w_size_ret;
//	clGetDeviceInfo( 	OPS_opencl_core.device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &w_size, &w_size_ret);
//	printf("\nCL_DEVICE_MAX_WORK_GROUP_SIZE is %d ret size %d\n", w_size, w_size_ret);

	if(ret != CL_SUCCESS) {
		char* build_log;
		size_t log_size;
		clSafeCall( clGetProgramBuildInfo(OPS_opencl_core.program, OPS_opencl_core.device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size) );
		build_log = (char*) malloc(log_size+1);
		clSafeCall( clGetProgramBuildInfo(OPS_opencl_core.program, OPS_opencl_core.device_id, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL) );
		build_log[log_size] = '\0';
		fprintf(stderr, "=============== OpenCL Program Build Info ================\n\n%s", build_log);
		fprintf(stderr, "\n========================================================= \n");
		free(build_log);
		exit(EXIT_FAILURE);
	}
}

//this needs to be a platform specific copy symbol to device function
void ops_decl_const_char( int dim, char const * type, int typeSize, char * dat, char const * name ) {
	cl_int ret = 0;
	if (OPS_opencl_core.constant == NULL) {
		OPS_opencl_core.constant = (cl_mem*) malloc((2)*sizeof(cl_mem));
		for ( int i=0; i<2; i++ ){
			OPS_opencl_core.constant[i] = NULL;
		}
	}
	if (!strcmp(name,"dx")) {
		if (OPS_opencl_core.constant[0] == NULL) {
			/*cl_mem_ext_ptr_t d_bank0_ext;
    d_bank0_ext.flags = 4 | XCL_MEM_TOPOLOGY;
    d_bank0_ext.obj = NULL;
    d_bank0_ext.param = 0;*/

			OPS_opencl_core.constant[0] = clCreateBuffer(OPS_opencl_core.context,CL_MEM_READ_WRITE, dim*typeSize, nullptr, &ret);
			clSafeCall( ret );
		}
		//Write the new constant to the memory of the device
		clSafeCall( clEnqueueWriteBuffer(OPS_opencl_core.command_queue, OPS_opencl_core.constant[0], CL_TRUE, 0, dim*typeSize, (void*) dat, 0, NULL, NULL) );
		clSafeCall( clFlush(OPS_opencl_core.command_queue) );
		clSafeCall( clFinish(OPS_opencl_core.command_queue) );

	}
	else
		if (!strcmp(name,"dy")) {
			if (OPS_opencl_core.constant[1] == NULL) {
				/*cl_mem_ext_ptr_t d_bank1_ext;
    	d_bank1_ext.flags = 1 | XCL_MEM_TOPOLOGY;
    	    d_bank1_ext.obj = NULL;
    	    d_bank1_ext.param = 0;*/

				OPS_opencl_core.constant[1] = clCreateBuffer(OPS_opencl_core.context,  CL_MEM_READ_WRITE, dim*typeSize, nullptr, &ret);
				clSafeCall( ret );
			}
			//Write the new constant to the memory of the device
			clSafeCall( clEnqueueWriteBuffer(OPS_opencl_core.command_queue, OPS_opencl_core.constant[1], CL_TRUE, 0, dim*typeSize, (void*) dat, 0, NULL, NULL) );
			clSafeCall( clFlush(OPS_opencl_core.command_queue) );
			clSafeCall( clFinish(OPS_opencl_core.command_queue) );
		}
		else
		{
			printf("error: unknown const name\n"); exit(1);
		}
}


void buildOpenCLKernels() {
	static bool isbuilt = false;

	if(!isbuilt) {
		//clSafeCall( clUnloadCompiler() );

		OPS_opencl_core.n_kernels = 1;
		OPS_opencl_core.kernel = (cl_kernel*) malloc(1*sizeof(cl_kernel));
	}
	isbuilt = true;
}

//user kernel files
//#include "poisson_kernel_error_opencl_kernel.cpp"
static bool isbuilt_poisson_kernel_error = false;

void buildOpenCLKernels_poisson_kernel_error(int xdim0, int xdim1) {
	if(!isbuilt_poisson_kernel_error) {
		buildOpenCLKernels();
		//clSafeCall( clUnloadCompiler() );
		cl_int ret;

		// Create the OpenCL kernel
		OPS_opencl_core.kernel[0] = clCreateKernel(OPS_opencl_core.program, "ops_poisson_kernel", &ret);
		clSafeCall( ret );

		isbuilt_poisson_kernel_error = true;
	}
}


// host stub function
void ops_par_loop_poisson_kernel_error(char const *name, ops_block block, int dim, int* range,
		ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3, ops_arg arg4, const int n_iters) {

	//Timing
	double t1,t2,c1,c2;

	ops_arg args[5] = { arg0, arg1, arg2, arg3, arg4};


#ifdef CHECKPOINTING
	if (!ops_checkpointing_before(args,3,range,4)) return;
#endif

	if (OPS_diags > 1) {
		ops_timing_realloc(0,"poisson_kernel_error");
		OPS_kernels[0].count++;
		ops_timers_core(&c1,&t1);
	}

	//compute locally allocated range for the sub-block
	int start[2];
	int end[2];
#ifdef OPS_MPI
	sub_block_list sb = OPS_sub_block_list[block->index];
	if (!sb->owned) return;
	for ( int n=0; n<2; n++ ){
		start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
		if (start[n] >= range[2*n]) {
			start[n] = 0;
		}
		else {
			start[n] = range[2*n] - start[n];
		}
		if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
		if (end[n] >= range[2*n+1]) {
			end[n] = range[2*n+1] - sb->decomp_disp[n];
		}
		else {
			end[n] = sb->decomp_size[n];
		}
		if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
			end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
	}
#else
	for ( int n=0; n<2; n++ ){
		start[n] = range[2*n];end[n] = range[2*n+1];
	}
#endif


	int arg_idx[2];
	#ifdef OPS_MPI
		arg_idx[0] = sb->decomp_disp[0]+start[0];
		arg_idx[1] = sb->decomp_disp[1]+start[1];
	#else
		arg_idx[0] = start[0] - 1;
		arg_idx[1] = start[1] - 1 ;
	#endif


	int x_size = MAX(0,end[0]-start[0]);
	int y_size = MAX(0,end[1]-start[1]);


	int xdim0 = args[0].dat->size[0];
	int xdim1 = args[1].dat->size[0];
	//build opencl kernel if not already built

	buildOpenCLKernels_poisson_kernel_error(
			xdim0,xdim1);


	size_t globalWorkSize[3] = {1,1,1};
	size_t localWorkSize[3] =  {1,1,1};

#ifdef OPS_MPI
	float *arg2h = (float *)(((ops_reduction)args[2].data)->data + ((ops_reduction)args[2].data)->size * block->index);
#else
	float *arg2h = (float *)(((ops_reduction)args[2].data)->data);
#endif

	int nblocks = ((x_size-1)/OPS_block_size_x+ 1)*((y_size-1)/OPS_block_size_y + 1);
	int maxblocks = nblocks;
	int reduct_bytes = 0;

	reduct_bytes += ROUND_UP(maxblocks*1*sizeof(float));

	reallocReductArrays(reduct_bytes);
	reduct_bytes = 0;

	int r_bytes2 = reduct_bytes/sizeof(float);
	arg2.data = OPS_reduct_h + reduct_bytes;
	arg2.data_d = OPS_reduct_d;// + reduct_bytes;
	for (int b=0; b<maxblocks; b++)
		for (int d=0; d<1; d++) ((float *)arg2.data)[d+b*1] = ZERO_float;
	reduct_bytes += ROUND_UP(maxblocks*1*sizeof(float));


	mvReductArraysToDevice(reduct_bytes);

	//set up initial pointers
	int d_m[OPS_MAX_DIM];
#ifdef OPS_MPI
	for (int d = 0; d < dim; d++) d_m[d] = args[0].dat->d_m[d] + OPS_sub_dat_list[args[0].dat->index]->d_im[d];
#else
	for (int d = 0; d < dim; d++) d_m[d] = args[0].dat->d_m[d];
#endif
	int base0 = 1 *1*
			(start[0] * args[0].stencil->stride[0] - args[0].dat->base[0] - d_m[0]);
	base0 = base0 + args[0].dat->size[0] *1*
			(start[1] * args[0].stencil->stride[1] - args[0].dat->base[1] - d_m[1]);

#ifdef OPS_MPI
	for (int d = 0; d < dim; d++) d_m[d] = args[1].dat->d_m[d] + OPS_sub_dat_list[args[1].dat->index]->d_im[d];
#else
	for (int d = 0; d < dim; d++) d_m[d] = args[1].dat->d_m[d];
#endif
	int base1 = 1 *1*
			(start[0] * args[1].stencil->stride[0] - args[1].dat->base[0] - d_m[0]);
	base1 = base1 + args[1].dat->size[0] *1*
			(start[1] * args[1].stencil->stride[1] - args[1].dat->base[1] - d_m[1]);


	ops_H_D_exchanges_device(args, 3);
	ops_halo_exchanges(args,3,range);
	ops_H_D_exchanges_device(args, 3);

	if (OPS_diags > 1) {
		ops_timers_core(&c2,&t2);
		OPS_kernels[0].mpi_time += t2-t1;
	}

	int nthread = OPS_block_size_x*OPS_block_size_y*OPS_block_size_z;




	if (globalWorkSize[0]>0 && globalWorkSize[1]>0 && globalWorkSize[2]>0) {
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 0, sizeof(cl_int), (void*) arg3.data));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 1, sizeof(cl_int), (void*) arg4.data));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 2, sizeof(cl_float), (void*) &dx));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 3, sizeof(cl_float), (void*) &dy));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 4, sizeof(cl_int), (void*) &arg_idx[0]));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 5, sizeof(cl_int), (void*) &arg_idx[1]));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 6, sizeof(cl_mem), (void*) &arg2.data_d));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 7, nthread*sizeof(float), NULL));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 8, sizeof(cl_int), (void*) &r_bytes2));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 9, sizeof(cl_int), (void*) &base0));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 10, sizeof(cl_int), (void*) &base1));

		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 11, sizeof(cl_int), (void*) &x_size));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 12, sizeof(cl_int), (void*) &y_size));
		clSafeCall( clSetKernelArg(OPS_opencl_core.kernel[0], 13, sizeof(cl_int), (void*) &n_iters ));
		//call/enque opencl kernel wrapper function
		clSafeCall( clEnqueueNDRangeKernel(OPS_opencl_core.command_queue, OPS_opencl_core.kernel[0], 3, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL) );
	}

	if (OPS_diags>1) {
		clSafeCall( clFinish(OPS_opencl_core.command_queue) );
	}

	if (OPS_diags > 1) {
		ops_timers_core(&c1,&t1);
		OPS_kernels[0].time += t1-t2;
	}

	mvReductArraysToHost(reduct_bytes);
	for ( int b=0; b<maxblocks; b++ ){
		for ( int d=0; d<1; d++ ){
			arg2h[d] = arg2h[d] + ((float *)arg2.data)[d+b*1];
		}
	}
	arg2.data = (char *)arg2h;

	ops_set_dirtybit_device(args, 3);

	if (OPS_diags > 1) {
		//Update kernel record
		ops_timers_core(&c2,&t2);
		OPS_kernels[0].mpi_time += t2-t1;
		OPS_kernels[0].transfer += ops_compute_transfer(dim, start, end, &arg0);
		OPS_kernels[0].transfer += ops_compute_transfer(dim, start, end, &arg1);
	}
}





