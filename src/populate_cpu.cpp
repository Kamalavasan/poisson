#include <stdio.h>
#include "rtm.h"

double dx = 0.005;
double dy = 0.005;
double dz = 0.005;

int copy_grid(float* grid_s, float* grid_d, int grid_size){
    memcpy(grid_d, grid_s, grid_size);
    return 0;
}

int populate_rho_mu_yy(float* grid, struct Grid_d grid_d){
  for(int i = 0; i < grid_d.act_sizez; i++){
    for(int j = 0; j < grid_d.act_sizey; j++){
      for(int k = 0; k < grid_d.act_sizex; k++){


        float x = 1.0 * (float ) (i - grid_d.act_sizex/2)/(grid_d.logical_size_x);
        float y = 1.0 * (float ) (j - grid_d.act_sizey/2)/(grid_d.logical_size_y);
        float z = 1.0 * (float ) (k - grid_d.act_sizez/2)/(grid_d.logical_size_z);

        const float C = 1;
        const float r0 = 0.001;

        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        grid[i*grid_size_x*grid_size_y*6 + j * grid_size_x*6 + k*6+0] = 1.0;
        grid[i*grid_size_x*grid_size_y*6 + j * grid_size_x*6 + k*6+1] = 1.0;
        for(int p = 2; p < 8; p++){
          grid[i*grid_size_x*grid_size_y*6 + j * grid_size_x*6 + k*6+p] = (1.0/3) * C * exp(-(x*x+y*y+z*z)/r0);
        }
      }
    }
  }
  return 0;
}


int caculate_index(struct Grid_d grid_d, int z, int y, int x, int pt){
  return (x*grid_size_x*grid_size_y*6 + y * grid_size_x*6 + z*6+pt);
}



void fd3d_pml_kernel(float* yy, float* dyy, struct Grid_d grid_d){

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



  for(int i = ORDER; i < grid_d.logical_size_z; i++){
    for(int j = ORDER; j < grid_d.logical_size_y; j++){
      for(int k = ORDER; k < grid_d.logical_size_x; k++){


        float sigma = grid[caculate_index(grid_d,i,j,k,1)] / grid[caculate_index(grid_d,i,j,k,0)];
        float sigmax=0.0;
        float sigmay=0.0;
        float sigmaz=0.0;
        if(k<=xbeg+pml_width){
          sigmax = (xbeg+pml_width-k)*sigma/pml_width;
        }
        if( k >= xend-pml_width){
          sigmax=( k -(xend-pml_width))*sigma/pml_width;
        }
        if(j <=ybeg+pml_width){
          sigmay=(ybeg+pml_width-j)*sigma/pml_width;
        }
        if(j >=yend-pml_width){
          sigmay=(j-(yend-pml_width))*sigma/pml_width;
        }
        if(i <=zbeg+pml_width){
          sigmaz=(zbeg+pml_width- i)*sigma/pml_width;
        }
        if( i >=zend-pml_width){
          sigmaz=( i -(zend-pml_width))*sigma/pml_width;
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
          
          pxy += yy[caculate_index(grid_d,i,j+l,k,2)]*c[l]*invdx;
          pyy += yy[caculate_index(grid_d,i,j+l,k,3)]*c[l]*invdx;
          pzy += yy[caculate_index(grid_d,i,j+l,k,4)]*c[l]*invdx;
          
          vxy += yy[caculate_index(grid_d,i,j+l,k,5)]*c[l]*invdx;
          vyy += yy[caculate_index(grid_d,i,j+l,k,6)]*c[l]*invdx;
          vzy += yy[caculate_index(grid_d,i,j+l,k,7)]*c[l]*invdx;
          
          pxz += yy[caculate_index(grid_d,i+l,j,k,2)]*c[l]*invdx;
          pyz += yy[caculate_index(grid_d,i+l,j,k,3)]*c[l]*invdx;
          pzz += yy[caculate_index(grid_d,i+l,j,k,4)]*c[l]*invdx;
          
          vxz += yy[caculate_index(grid_d,i+l,j,k,5)]*c[l]*invdx;
          vyz += yy[caculate_index(grid_d,i+l,j,k,6)]*c[l]*invdx;
          vzz += yy[caculate_index(grid_d,i+l,j,k,7)]*c[l]*invdx;
        }
      

        dyy[caculate_index(grid_d,i+l,j,k,2)]=vxx/yy[caculate_index(grid_d,i+l,j,k,0)]- sigmax*px;
        dyy[caculate_index(grid_d,i+l,j,k,5)]=(pxx+pyx+pxz)*yy[caculate_index(grid_d,i+l,j,k,1)] - sigmax*vx;
        
        dyy[caculate_index(grid_d,i+l,j,k,3)]=vyy/yy[caculate_index(grid_d,i+l,j,k,0)]  - sigmay*py;
        dyy[caculate_index(grid_d,i+l,j,k,6)]=(pxy+pyy+pyz)*yy[caculate_index(grid_d,i+l,j,k,1)]  - sigmay*vy;
        
        dyy[caculate_index(grid_d,i+l,j,k,4)]=vzz/yy[caculate_index(grid_d,i+l,j,k,0)]  - sigmaz*pz;
        dyy[caculate_index(grid_d,i+l,j,k,7)]=(pxz+pyz+pzz)*yy[caculate_index(grid_d,i+l,j,k,1)]  - sigmaz*vz;


      }
    }
  }
}