/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "main_functions.h"
#include "model_settings.h"
#include "model.h"
#include "test_samples.h"
#include "hx_drv_tflm.h"
#include "synopsys_wei_delay.h"
#include "synopsys_wei_i2c_oled1306.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

// sin計算
double sind(double d){
    double v = sin(d*(PI / 180));
    return v;
}

// cos計算
double cosd(double d){
    double v = cos(d*(PI / 180));
    return v;
}

// 極角
double atanp(double d1, double d2){
    double v = atan2(d1,d2)*(180/PI);
    return v;
}

// 禁航區
double restricted(double Lat, double Lon){
   double R = -0.092391304 * Lat + 123.8594428717 - Lon;  
   return R;
}
// 警戒區
double alert(double Lat, double Lon){
   double P = -0.3606557377 * Lat + 130.5710695081 - Lon;
   return P;
}

// 低空限航區
uint8_t hight_area(double d){
    if (d <= 0){
      hx_drv_uart_print("Warning!! You are now entering Restricted Area. Please fly higher. \n");
      int m = 1;
      return m; 
    }
    else{
      int m = 12;
      return m; 
    }   
}

// 方位角
double atana(double d1, double d2, double D, double Lat, double Lon){
    double v = atan2(d1,d2)*(180/PI);
    if (v == 0){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-E \n");}}
      return (-(v-90));
    }
    else if (v < 90 && v > 0){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-1 \n");}}
      return (-(v-90));
    }
    else if (v == 90){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-N \n");}}
      return (v-90);
    }
    else if (v < 180 && v > 90){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-4 \n");}}
      return (180-v+270);
    }
    else if (v == 180){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-W \n");}}
      return (v+90);
    }
    else if (v < -90 && v > -180){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-3 \n");}}
      return (-v+90);
    }
    else if (v == -90){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-S \n");}}
      return (-v+90);
    }
    else if (v < 0 && v > -90){
      double R =  restricted(Lat,Lon); 
      if (R <= 0){}
      else
      {
        double P = alert(Lat,Lon); 
        if (P <= 0){}
        else{hx_drv_uart_print("-2 \n");}}
      return (-v+90);
    }
}
//利用冪律公式推算該高度風速
double Wind_Height(double W, double H){
  double V = W*(pow((H/3), 0.14));
  return V;
}

//計算無人機目前位置
uint8_t radius_area(double d, double H, double W, double Lat, double Lon){
    double w = Wind_Height(W, H);
    double R = restricted(Lat,Lon); 
    if (R <= 0){
        hx_drv_uart_print("Stop!! You are now entering Restricted Area. Please leave this area quckly. \n");
        int m = 2;
        return m; 
      }
    else
    {
        double P = alert(Lat,Lon); 
        if (P <= 0){
          hx_drv_uart_print("Warning!! You are now entering Alert Area(UAV Action: R5). Please be careful to fly. \n");
          int m = 3;
          return m; 
        }
        else{ 
              if (H > 0){
                if (w <= 8.0){
                  hx_drv_uart_print("Safe area. \n");
                  if (d > 80){
                    hx_drv_uart_print("UAV Action:: Other area ");
                  }
                  else if (d <= 80 && d > 60){
                    hx_drv_uart_print("UAV Action:: R1");
                  }
                  else if (d <= 60 && d > 40){
                    hx_drv_uart_print("UAV Action:: R2");
                  }
                  else if (d <= 40 && d > 20){
                    hx_drv_uart_print("UAV Action: R3");
                  }
                  else if (d <= 20){
                    hx_drv_uart_print("UAV Action: R4");
                  }
                  int m = 4;
                  return m; 
                }
                else{
                  if (d > 80){
                    hx_drv_uart_print("UAV Action:: Other area ");
                  }
                  else if (d <= 80 && d > 60){
                    hx_drv_uart_print("UAV Action:: R1");
                  }
                  else if (d <= 60 && d > 40){
                    hx_drv_uart_print("UAV Action:: R2");
                  }
                  else if (d <= 40 && d > 20){
                    hx_drv_uart_print("UAV Action: R3");
                  }
                  else if (d <= 20){
                    hx_drv_uart_print("UAV Action: R4");
                  }
                  int m = 0;
                  return m; 
                }
              }
              else{
                if (d > 80){
                  hx_drv_uart_print("UAV Action:: Other area ");
                }
                else if (d <= 80 && d > 60){
                  hx_drv_uart_print("UAV Action:: R1");
                }
                else if (d <= 60 && d > 40){
                  hx_drv_uart_print("UAV Action:: R2");
                }
                else if (d <= 40 && d > 20){
                  hx_drv_uart_print("UAV Action: R3");
                }
                else if (d <= 20){
                  hx_drv_uart_print("UAV Action: R4");
                }
                int m = 0;
                return m; 
              }  
            }
    }
}
//判斷塔台感測到之風速大小給予無人機警示
double wind_risk(double W,double H){
    double V = Wind_Height(W,H);
    if (H > 0){
        if (V > 10.7){
          hx_drv_uart_print("Stop!! The wind is Strong Breeze. The wind speed level is max. Don't fly in the area. \n");
          int w = 11;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
        else if (V <= 10.7 && V >= 8.0){
          hx_drv_uart_print("Warning!! The wind is Fresh Breeze. The wind speed level is 5. Please land or return instantly .  \n");
          int w = 10;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
        else if (V <= 7.9 && V >= 5.5){
          hx_drv_uart_print("The wind is Fresh Breeze. The wind speed level is 4.  \n"); 
          int w = 9;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
        else if (V <= 5.4 && V >= 3.4){
          hx_drv_uart_print("The wind is Gentle Breeze. The wind speed level is 3.  \n");
          int w = 8;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
        else if (V <= 3.3 && V >= 1.6){
          hx_drv_uart_print("The wind is Light Breeze. The wind speed level is 2.  \n");
          int w = 7;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }  
        else if (V <= 1.5 && V >= 0.3){
          hx_drv_uart_print("The wind is Light Air. The wind speed level is 1.  \n");
          int w = 6;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
        else if (V <0.3){
          hx_drv_uart_print("The wind is Calm. The wind speed level is 0.  \n"); 
          int w = 5;
          hx_drv_uart_print("w: %d\n ", w);
          return w;
        }
    }
    else{
      int m = 0;
      return m; 
    }   
}

uint8_t UAV(double lat, double lon, int H, double W){
 int Re = 6371137;  // 地球半徑
 double o[] = {25.01934, 121.54733}; // 塔台經緯度
 double u[] = {lat, lon}; // UAV經緯度
 double O[] = {Re*sind(90-o[0])*cosd(o[1]), Re*sind(90-o[0])*sind(o[1]), Re*cosd(90-o[0])}; // 塔台(XYZ)
 double U[] = {Re*sind(90-u[0])*cosd(u[1]), Re*sind(90-u[0])*sind(u[1]), Re*cosd(90-u[0])}; // UAV(XYZ)
 double diff[] = {U[0]-O[0], U[1]-O[1], U[2]-O[2]}; // 塔台與UAV間差距
 double e[] = {cosd(90+o[1]), sind(90+o[1]), 0}; // 座標轉換(北)
 double n[] = {cosd(90-o[0])*cosd(o[1]+180), cosd(90-o[0])*sind(o[1]+180), sind(90-o[0])}; //座標轉換(東)
 double x = diff[0]*e[0]+diff[1]*e[1]; // 新座標X
 double y = diff[0]*n[0]+diff[1]*n[1]+diff[2]*n[2]; // 新座標Y
 double D = sqrt(pow(diff[0],2)+pow(diff[1],2)+pow(diff[2],2)); // 平面距離
 int din = sqrt(pow(diff[0],2)+pow(diff[1],2)+pow(diff[2],2)); // 平面距離
  hx_drv_uart_print("Line distance(to ATC): %d\n ", din);
  //double z = sqrt(pow(D,2)-pow(x,2)-pow(y,2)); // 新座標Z
  //hight_area(H);
  hx_drv_uart_print("Height(to ATC): %d\n ", H);
  int R = sqrt(pow(D,2)+pow(H,2));  // 三維距離(r)
  //radius_area(R, lat, lon);
  uint8_t Hight = hight_area(H);
  uint8_t Fly = radius_area(R, H, W, lat, lon);
  uint16_t P = atanp(D,H); // 極角(φ)
  uint16_t T = atana(y,x,R, lat, lon); // 方位角(θ)
  hx_drv_uart_print("UAV coordinate: %d, %d, %d\n ", R, T, P);
  
  if (Hight == 1){
    hx_drv_uart_print("H: %d\n ", Hight);
    return Hight;
  }
  else if (Fly != 0){
    hx_drv_uart_print("F: %d\n ", Fly);
    return Fly;
  }
}


// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;


// An area of memory to use for input, output, and intermediate arrays.
constexpr int kTensorArenaSize = 50 * 2048;
static uint8_t tensor_arena[kTensorArenaSize] = {0};
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  
  static tflite::MicroMutableOpResolver<3> micro_op_resolver;
  //micro_op_resolver.AddConv2D();
  //micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddFullyConnected();
  //micro_op_resolver.AddReshape();
  //micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddRelu();
  micro_op_resolver.AddLogistic();

  // Build an interpreter to run the model with.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);
  output = interpreter->output(0);
}

// The name of this function is important for Arduino compatibility.
void loop()
{
  int32_t function_code = 0;

  while (true) 
  {
    //int main(int argc, char* argv[]);
    hx_drv_uart_initial(UART_BR_115200);
    hx_drv_share_switch(SHARE_MODE_I2CM);
    uint8_t data_write[2];
    uint8_t data_write1[9];
    uint8_t data_read[9];
      
    hx_drv_i2cm_get_data(SSD1306_ADDRESS, data_read, 9, data_write1, 9);
    const int wind_dir  =90;
    const int wind_speed=2;
    const int UAV_speed=1;
    const int Satellite_signal=11;

    int j=1;
    
    TF_LITE_REPORT_ERROR(error_reporter, "Test sample[%d] Start Reading\n", j);
    // Write  input data
    input->data.int8[0] = wind_dir;   
    input->data.int8[1] = wind_speed;
    input->data.int8[2] = UAV_speed;
    input->data.int8[3] =Satellite_signal;
    for(int i=0; i<4; i++){
      hx_drv_uart_print("input %d\n",input->data.int8[i]);
    }
    
    TF_LITE_REPORT_ERROR(error_reporter, "Test sample[%d] Start Invoking\n", j);
    // Run the model on this input and make sure it succeeds.
    if (kTfLiteOk != interpreter->Invoke()) {
      TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
      break;

    }
    //輸出
    int out1 = (output->data.f[0]) *10000;
    int out2 = (output->data.f[1]) *10000;
    int out3 = (output->data.f[2]) *10000;
    int out4 = (output->data.f[3]) *10000;

    int out_arr[4] = {out1, out2, out3, out4};
    int max_value = out_arr[0];
    int max_index = 0;
    for(int i = 1; i < 4; ++i){
        if(out_arr[i] > max_value){
            max_value = out_arr[i];
            max_index = i;
        }
    }
    hx_drv_uart_print("risk level %d\n",max_index);
    data_write[0] = max_index;
    hx_drv_i2cm_set_data(SSD1306_ADDRESS, data_read, 0, data_write, 1);
    function_code=1
  }
  
   if(function_code==1) 
  {  
    hx_drv_i2cm_get_data(SSD1306_ADDRESS, data_read, 11, data_write1, 11);
    double lat = data_write1[0] + data_write1[1] * 0.01 + data_write1[2] * 0.0001 + data_write1[3] * 0.000001;
    double lon = data_write1[4] + data_write1[5] * 0.01 + data_write1[6] * 0.0001 + data_write1[7] * 0.00001;
    int h = data_write1[8];
    int H = h-1;
    double W = data_write1[9] + data_write1[10] * 0.01;
    hal_delay_ms(5000);

    data_write[0] = UAV(lat, lon, H, W);
    data_write[1] = wind_risk(W, H);
    hx_drv_i2cm_set_data(SSD1306_ADDRESS, data_read, 0, data_write, 2);
}
