#include "VI_image.h"
#include "hsv2rgb.h"

#include <math.h>


img_coor_t get_coordinate(float vdata,float idata,float v_max,float i_max){
    img_coor_t coordinate;
    coordinate.x_v=vdata/v_max*N+N;
    coordinate.y_i=idata/i_max*N+N;
    return coordinate;
}

float findmax(float* data, int datasize){
    int num;
    float max=0;
    for(num=0;num<datasize;num++){
        if(fabs(data[num])>max){
            max=fabs(data[num]);
        }
    }
    return max;
}

void generate_VI_image(vi_data_t* data,unsigned char* image){
    int i,j;
    float v_max,i_max,vrms,irms;
    float img[IMAGE_SIZE]={};
    v_max=findmax(data->v, PERIOD_SIZE);
    i_max=findmax(data->i, PERIOD_SIZE);
    vrms=v_max*sqrt(2)*0.5;
    int a,b;
    for(i=0;i<PERIOD_SIZE;i++){
        img_coor_t coor=get_coordinate(data->v[i],data->i[i],v_max,i_max);
        a=coor.x_v;
        b=coor.y_i;
        img[a*32+b]++;
        img[32*32+a*32+b]+=fabs(data->v[i]*data->i[i]);
        img[2*32*32+a*32+b]+=data->i[i]*data->i[i];
    }
    for(i=0;i<2*N;i++){
        for(j=0;j<2*N;j++){
            if(img[i*32+j]!=0){
                float H=(int)(fabs((j+0.5-N)/N)*i_max)%5*72;
                float S=img[32*32+i*32+j]/(v_max*sqrt(img[i*32+j]*img[2*32*32+i*32+j]*0.5));
                float V=50;
                RGBColor rgb = hsv2rgb(H,S,V);
                image[i*32+j]=rgb.b;
                image[32*32+i*32+j]=rgb.g;
                image[2*32*32+i*32+j]=rgb.r;
            }
        }
    }
}
