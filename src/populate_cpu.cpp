#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "rtm.h"


int copy_grid(float* grid_s, float* grid_d, int grid_size){
    memcpy(grid_d, grid_s, grid_size);
    return 0;
}

int populate_rho_mu_yy(float* grid, struct Grid_d grid_d){
  for(int i = 0; i < grid_d.act_sizez; i++){
    for(int j = 0; j < grid_d.act_sizey; j++){
      for(int k = 0; k < grid_d.act_sizex; k++){

        if(i < ORDER || i >= (grid_d.logical_size_z+ORDER) || j < ORDER || j >= (grid_d.logical_size_y+ORDER) || k < ORDER || k >= (grid_d.logical_size_x+ORDER)){
          for(int p = 0; p < 8; p++){
            grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + p] = 0;
          }
        } else {
          float x = 1.0 * (float ) (k - grid_d.act_sizex/2)/(grid_d.logical_size_x);
          float y = 1.0 * (float ) (j - grid_d.act_sizey/2)/(grid_d.logical_size_y);
          float z = 1.0 * (float ) (i - grid_d.act_sizez/2)/(grid_d.logical_size_z);

          const float C = 1;
          const float r0 = 0.1;

          grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + 0] = 1.0;
          grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + 1] = 1.0;
          grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + 2] = (1.0/3) * C * exp(-(x*x+y*y+z*z)/r0);
          for(int p = 3; p < 8; p++){
            grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + p] = 0;
          }
        }
      }
    }
  }
  return 0;
}

int dump_rho_mu_yy(float* grid, struct Grid_d grid_d, char n_rho[100], char n_mu[100], char n_yy[100]){
  FILE* fp_rho = fopen(n_rho, "w");
  FILE* fp_mu  = fopen(n_mu, "w");
  FILE* fp_yy  = fopen(n_yy, "w");

  for(int i = 0; i < grid_d.act_sizez; i++){
    for(int j = 0; j < grid_d.act_sizey; j++){
      for(int k = 0; k < grid_d.act_sizex; k++){
        fprintf(fp_rho, "%e ", grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + 0]);
        fprintf(fp_mu, "%e ",  grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + 1]);
        for(int p = 2; p < 8; p++){
            fprintf(fp_yy, "%e ",  grid[i * grid_d.grid_size_x * grid_d.grid_size_y * 8 + j * grid_d.grid_size_x * 8 + k*8 + p]);
        }
      }
      fprintf(fp_rho, "\n");
      fprintf(fp_mu, "\n");
      fprintf(fp_yy, "\n");
    }
    fprintf(fp_rho, "\n");
    fprintf(fp_mu, "\n");
    fprintf(fp_yy, "\n");
  }
  fclose(fp_rho);
  fclose(fp_mu);
  fclose(fp_yy);
  return 0;
}


inline int caculate_index(struct Grid_d grid_d, int z, int y, int x, int pt){
  return (z * grid_d.grid_size_x * grid_d.grid_size_y * 8 + y * grid_d.grid_size_x * 8 + x*8 + pt);
}



void fd3d_pml_kernel(float* yy, float* dyy, struct Grid_d grid_d){
  int half = 4;
  int pml_width = 10;
  double dx = 0.005;
  double dy = 0.005;
  double dz = 0.005;

  float coeffs[9][9] = {
    {-2.717857142857143,8.0,-14.0,18.666666666666668,-17.5,11.2,-4.666666666666667,1.1428571428571428,-0.125}, 
    {-0.125,-1.5928571428571427,3.5,-3.5,2.9166666666666665,-1.75,0.7,-0.16666666666666666,0.017857142857142856}, 
    {0.017857142857142856,-0.2857142857142857,-0.95,2.0,-1.25,0.6666666666666666,-0.25,0.05714285714285714,-0.005952380952380952},
    {-0.005952380952380952,0.07142857142857142,-0.5,-0.45,1.25,-0.5,0.16666666666666666,-0.03571428571428571,0.0035714285714285713}, 
    {0.0035714285714285713,-0.0380952380952381,0.2,-0.8,0.0,0.8,-0.2,0.0380952380952381,-0.0035714285714285713}, 
    {-0.0035714285714285713,0.03571428571428571,-0.16666666666666666,0.5,-1.25,0.45,0.5,-0.07142857142857142,0.005952380952380952}, 
    {0.005952380952380952,-0.05714285714285714,0.25,-0.6666666666666666,1.25,-2.0,0.95,0.2857142857142857,-0.017857142857142856},
    {-0.017857142857142856,0.16666666666666666,-0.7,1.75,-2.9166666666666665,3.5,-3.5,1.5928571428571427,0.125}, 
    {0.125,-1.1428571428571428,4.666666666666667,-11.2,17.5,-18.666666666666668,14.0,-8.0,2.717857142857143}
  };


  float* c = &coeffs[half][half];
  float invdx = 1.0 / dx;
  float invdy = 1.0 / dy;
  float invdz = 1.0 / dz;
  int xbeg=half;
  int xend=grid_d.logical_size_x -half;
  int ybeg=half;
  int yend=grid_d.logical_size_y-half;
  int zbeg=half;
  int zend=grid_d.logical_size_z-half;
  int xpmlbeg=xbeg+pml_width;
  int ypmlbeg=ybeg+pml_width;
  int zpmlbeg=zbeg+pml_width;
  int xpmlend=xend-pml_width;
  int ypmlend=yend-pml_width;
  int zpmlend=zend-pml_width;



  for(int i = ORDER; i < grid_d.logical_size_z+ORDER; i++){
    for(int j = ORDER; j < grid_d.logical_size_y+ORDER; j++){
      for(int k = ORDER; k < grid_d.logical_size_x+ORDER; k++){


        float sigma = yy[caculate_index(grid_d,i,j,k,1)] / yy[caculate_index(grid_d,i,j,k,0)];
        float sigmax=0.0;
        float sigmay=0.0;
        float sigmaz=0.0;

        int x = k - ORDER;
        int y = j - ORDER;
        int z = i - ORDER;

        if(x<=xbeg+pml_width){
          sigmax = (xbeg+pml_width-x)*sigma/pml_width;
        }
        if( x >= xend-pml_width){
          sigmax=( x -(xend-pml_width))*sigma/pml_width;
        }
        if(y <=ybeg+pml_width){
          sigmay=(ybeg+pml_width-y)*sigma/pml_width;
        }
        if(y >=yend-pml_width){
          sigmay=(y-(yend-pml_width))*sigma/pml_width;
        }
        if(z <=zbeg+pml_width){
          sigmaz=(zbeg+pml_width- z)*sigma/pml_width;
        }
        if( z >=zend-pml_width){
          sigmaz=( z -(zend-pml_width))*sigma/pml_width;
        }

        //sigmax=0.0;
        //sigmay=0.0;
        
        float px = yy[caculate_index(grid_d,i,j,k,2)];
        float py = yy[caculate_index(grid_d,i,j,k,3)];
        float pz = yy[caculate_index(grid_d,i,j,k,4)];
        
        float vx = yy[caculate_index(grid_d,i,j,k,5)];
        float vy = yy[caculate_index(grid_d,i,j,k,6)];
        float vz = yy[caculate_index(grid_d,i,j,k,7)];
        
        float vxx=0.0;
        float vxy=0.0;
        float vxz=0.0;
        
        float vyx=0.0;
        float vyy=0.0;
        float vyz=0.0;

        float vzx=0.0;
        float vzy=0.0;
        float vzz=0.0;
        
        float pxx=0.0;
        float pxy=0.0;
        float pxz=0.0;
        
        float pyx=0.0;
        float pyy=0.0;
        float pyz=0.0;

        float pzx=0.0;
        float pzy=0.0;
        float pzz=0.0;

        for(int l=-half;l<=half;l++){
          pxx += yy[caculate_index(grid_d,i,j,k+l,2)]*c[l]*invdx;
          pyx += yy[caculate_index(grid_d,i,j,k+l,3)]*c[l]*invdx;
          pzx += yy[caculate_index(grid_d,i,j,k+l,4)]*c[l]*invdx;
          
          vxx += yy[caculate_index(grid_d,i,j,k+l,5)]*c[l]*invdx;
          vyx += yy[caculate_index(grid_d,i,j,k+l,6)]*c[l]*invdx;
          vzx += yy[caculate_index(grid_d,i,j,k+l,7)]*c[l]*invdx;
          
          pxy += yy[caculate_index(grid_d,i,j+l,k,2)]*c[l]*invdy;
          pyy += yy[caculate_index(grid_d,i,j+l,k,3)]*c[l]*invdy;
          pzy += yy[caculate_index(grid_d,i,j+l,k,4)]*c[l]*invdy;
          
          vxy += yy[caculate_index(grid_d,i,j+l,k,5)]*c[l]*invdy;
          vyy += yy[caculate_index(grid_d,i,j+l,k,6)]*c[l]*invdy;
          vzy += yy[caculate_index(grid_d,i,j+l,k,7)]*c[l]*invdy;
          
          pxz += yy[caculate_index(grid_d,i+l,j,k,2)]*c[l]*invdz;
          pyz += yy[caculate_index(grid_d,i+l,j,k,3)]*c[l]*invdz;
          pzz += yy[caculate_index(grid_d,i+l,j,k,4)]*c[l]*invdz;
          
          vxz += yy[caculate_index(grid_d,i+l,j,k,5)]*c[l]*invdz;
          vyz += yy[caculate_index(grid_d,i+l,j,k,6)]*c[l]*invdz;
          vzz += yy[caculate_index(grid_d,i+l,j,k,7)]*c[l]*invdz;
        }
      

        dyy[caculate_index(grid_d,i,j,k,2)]=vxx/yy[caculate_index(grid_d,i,j,k,0)]- sigmax * px;
        dyy[caculate_index(grid_d,i,j,k,5)]=(pxx+pyx+pxz)*yy[caculate_index(grid_d,i,j,k,1)] - sigmax*vx;
        
        dyy[caculate_index(grid_d,i,j,k,3)]=vyy/yy[caculate_index(grid_d,i,j,k,0)]  - sigmay*py;
        dyy[caculate_index(grid_d,i,j,k,6)]=(pxy+pyy+pyz)*yy[caculate_index(grid_d,i,j,k,1)]  - sigmay*vy;
        
        dyy[caculate_index(grid_d,i,j,k,4)]= vzz/yy[caculate_index(grid_d,i,j,k,0)]  - sigmaz*pz;
        dyy[caculate_index(grid_d,i,j,k,7)]=(pxz+pyz+pzz)*yy[caculate_index(grid_d,i,j,k,1)]  - sigmaz*vz;


      }
    }
  }
}