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
#include "xcl2.hpp"
#include <chrono>
#include "omp.h"

int stencil_computation(float* current, float* next, int act_sizex, int act_sizey, int act_sizez, int grid_size_x, int grid_size_y, int grid_size_z){
    for(int i = 0; i < act_sizez; i++){
      for(int j = 0; j < act_sizey; j++){
        for(int k = 0; k < act_sizex; k++){
          if(i == 0 || j == 0 || k ==0 || i == act_sizez -1  || j==act_sizey-1 || k == act_sizex -1){
            next[i*grid_size_x*grid_size_y + j*grid_size_x + k] = current[i*grid_size_x*grid_size_y + j*grid_size_x + k] ;
          } else {
            next[i*grid_size_x*grid_size_y + j*grid_size_x + k] =     current[(i-1)*grid_size_x*grid_size_y + (j)*grid_size_x + (k)] * (0.01)  + \
                                          current[(i+1)*grid_size_x*grid_size_y + (j)*grid_size_x + (k)] * (0.02)  + \
                                          current[(i)*grid_size_x*grid_size_y + (j-1)*grid_size_x + (k)] * (0.03)  + \
                                          current[(i)*grid_size_x*grid_size_y + (j+1)*grid_size_x + (k)] * (0.04)  + \
                                          current[(i)*grid_size_x*grid_size_y + (j)*grid_size_x + (k-1)] * (0.05)  + \
                                          current[(i)*grid_size_x*grid_size_y + (j)*grid_size_x + (k+1)] * (0.06)  + \
                                          current[(i)*grid_size_x*grid_size_y + (j)*grid_size_x + (k)] * (0.79) ;
          }
        }
      }
    }
    return 0;
}

double square_error(float* current, float* next, int act_sizex, int act_sizey, int act_sizez, int grid_size_x, int grid_size_y, int grid_size_z){
    double sum = 0;
    for(int i = 0; i < act_sizez; i++){
      for(int j = 0; j < act_sizey; j++){
        for(int k = 0; k < act_sizex; k++){
          float val1 = next[i*grid_size_x*grid_size_y + j*grid_size_x+k]*next[i*grid_size_x + j];
          float val2 = current[i*grid_size_x*grid_size_y + j*grid_size_x+k]*next[i*grid_size_x + j];
          sum +=  val1*val1 - val2*val2;
        }
      }
    }
    return sum;
}

int copy_grid(float* grid_s, float* grid_d, int grid_size){
    memcpy(grid_d, grid_s, grid_size);
    return 0;
}


int initialise_grid(float* grid, int act_sizex, int act_sizey, int act_sizez, int grid_size_x, int grid_size_y, int grid_size_z){
  for(int i = 0; i < act_sizez; i++){
    for(int j = 0; j < act_sizey; j++){
      for(int k = 0; k < act_sizex; k++){
        if(i == 0 || j == 0 || k == 0 || i == act_sizez -1  || j==act_sizey-1 || k == act_sizex-1 ){
          float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
          grid[i*grid_size_x*grid_size_y + j * grid_size_x + k] = r;
        } else {
          grid[i*grid_size_x*grid_size_y + j * grid_size_x + k] = 0;
        }
      }
    }
  }
  return 0;
}


// // OPS header file
// #define OPS_2D
// #include "ops_seq_v2.h"
// #include "user_types.h"
// #include "poisson_kernel.h"

/******************************************************************************
* Main program
*******************************************************************************/
int main(int argc, char **argv)
{
  /**-------------------------- Initialisation --------------------------**/

  // OPS initialisation
  // ops_init(argc,argv,1);


  //Mesh
  int logical_size_x = 20;
  int logical_size_y = 20;
  int logical_size_z = 20;
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
    pch = strstr(argv[n], "-non-copy");
    if(pch != NULL) {
      non_copy = 1; continue;
    }
  }

  printf("Grid: %dx%dx%d in %dx%d blocks, %d iterations, %d tile height\n",logical_size_x,logical_size_y,logical_size_z, ngrid_x,ngrid_y,n_iter,itertile);

  int act_sizex = logical_size_x + 2;
  int act_sizey = logical_size_y + 2;
  int act_sizez = logical_size_z + 2;

  int grid_size_x = (act_sizex % 16) != 0 ? (act_sizex/16 +1) * 16 : act_sizex;
  int grid_size_y = act_sizey;
  int grid_size_z = act_sizez;

  int data_size_bytes = grid_size_x * grid_size_y * grid_size_z* sizeof(float);
  float * grid_u1 = (float*)aligned_alloc(4096, data_size_bytes);
  float * grid_u2 = (float*)aligned_alloc(4096, data_size_bytes);

  float * grid_u1_d = (float*)aligned_alloc(4096, data_size_bytes);
  float * grid_u2_d = (float*)aligned_alloc(4096, data_size_bytes);

  initialise_grid(grid_u1, act_sizex, act_sizey, act_sizez, grid_size_x, grid_size_y, grid_size_z);
  copy_grid(grid_u1, grid_u1_d, data_size_bytes);
  // stencil computation

  //OPENCL HOST CODE AREA START

    auto binaryFile = argv[1];
    cl_int err;
    cl::Event event;

    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(
        err,
        cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    OCL_CHECK(err,
              std::string device_name = device.getInfo<CL_DEVICE_NAME>(&err));



    //Create Program and Kernel
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    devices.resize(1);
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
    OCL_CHECK(err, cl::Kernel krnl_stencil(program, "stencil_SLR0", &err));



    //Allocate Buffer in Global Memory
    OCL_CHECK(err,
              cl::Buffer buffer_input(context,
                                      CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                      data_size_bytes,
                                      grid_u1_d,
                                      &err));
    OCL_CHECK(err,
              cl::Buffer buffer_output(context,
                                       CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                       data_size_bytes,
                                       grid_u2_d,
                                       &err));

    //Set the Kernel Arguments
    int narg = 0;
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, buffer_input));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, buffer_output));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, logical_size_x));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, logical_size_y));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, logical_size_z));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, grid_size_x));
    OCL_CHECK(err, err = krnl_stencil.setArg(narg++, n_iter));

    //Copy input data to device global memory
    OCL_CHECK(err,
              err = q.enqueueMigrateMemObjects({buffer_input},
                                               0 /* 0 means from host*/));

    uint64_t wtime = 0;
    uint64_t nstimestart, nstimeend;
    auto start = std::chrono::high_resolution_clock::now();


	OCL_CHECK(err, err = q.enqueueTask(krnl_stencil));
	q.finish();


    q.finish();
    auto finish = std::chrono::high_resolution_clock::now();
    //Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err,
              err = q.enqueueMigrateMemObjects({buffer_input},
                                               CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err,
                 err = q.enqueueMigrateMemObjects({buffer_output},
                                                  CL_MIGRATE_MEM_OBJECT_HOST));

    q.finish();
//    auto finish = std::chrono::high_resolution_clock::now();


  for(int itr = 0; itr < n_iter*1; itr++){
      stencil_computation(grid_u1, grid_u2, act_sizex, act_sizey, act_sizez, grid_size_x, grid_size_y, grid_size_z);
      stencil_computation(grid_u2, grid_u1, act_sizex, act_sizey, act_sizez, grid_size_x, grid_size_y, grid_size_z);
  }

    std::chrono::duration<double> elapsed = finish - start;


  printf("Runtime on FPGA is %f seconds\n", elapsed.count());
  double error = square_error(grid_u1, grid_u1_d, act_sizex, act_sizey, act_sizez, grid_size_x, grid_size_y, grid_size_z);
  float bandwidth = (data_size_bytes * 4.0 * n_iter)/(elapsed.count() * 1000 * 1000 * 1000);
  printf("\nMean Square error is  %f\n\n", error/(logical_size_x * logical_size_y));
  printf("\nBandwidth is %f\n", bandwidth);

//  act_sizez = 2;
//  for(int i = 0; i < act_sizez; i++){
//    for(int j = 0; j < act_sizey; j++){
//  	  for(int k = 0; k < act_sizex; k++){
//  		  printf("%f ", grid_u2_d[i*grid_size_x*grid_size_y+j*grid_size_x + k]);
//  	  }printf("\n");
//    }printf("\n");
//  }
//
// printf("\ngolden\n\n");
// for(int i = 0; i < act_sizez; i++){
//    for(int j = 0; j < act_sizey; j++){
//      for(int k = 0; k < act_sizex; k++){
//        printf("%f ", grid_u2[i*grid_size_x*grid_size_y+j*grid_size_x + k]);
//      }printf("\n");
//    }printf("\n");
//  }
  return 0;
}
