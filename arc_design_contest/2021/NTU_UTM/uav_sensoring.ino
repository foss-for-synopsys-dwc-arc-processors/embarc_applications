#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <sps30.h>
#include<SoftwareSerial.h>
#include <SPI.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"

int8_t answer;
char gps_data[100];
char lon_[10] = "";
char la_[10] = "";
int counter;
bool enableHeater = false;
uint8_t loopCnt = 0;

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

// Set BMP280
Adafruit_BMP280 bmp(BMP_CS,
BMP_MOSI, BMP_MISO, BMP_SCK);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
SoftwareSerial mySerial(7,8);
String Publish = "hsinwei"; //Publish Topic


void setup() {
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;
  Serial.begin(115200);
  mySerial.begin(115200);
  bmp.begin();

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");
  
  sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }

#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }

#ifndef PLOTTER_FORMAT
  Serial.print("measurements started\n");
#endif /* PLOTTER_FORMAT */


  //AT Commands for setting up the client id and Server
  
  Serial.println("Connecting To Server........");
  mySerial.println("AT+CMQTTSTART"); //Establishing MQTT Connection
  delay(2000); 
  mySerial.println("AT+CMQTTACCQ=0,\"david927\""); //Client ID
  delay(2000);
  mySerial.println("AT+CMQTTCONNECT=0,\"tcp://broker.hivemq.com:1883\",90,1"); //MQTT Server Name for connecting this client
  delay(2000);
  mySerial.println("AT+CNETSTART");//Start the internet connection
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }


  // Toggle heater enabled state every 30 seconds
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  if (++loopCnt == 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");

    loopCnt = 0;
  }
  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {
#ifndef PLOTTER_FORMAT
    Serial.print("PM  1.0: ");
    Serial.println(m.mc_1p0);
    Serial.print("PM  2.5: ");
    Serial.println(m.mc_2p5);
    Serial.print("PM  4.0: ");
    Serial.println(m.mc_4p0);
    Serial.print("PM 10.0: ");
    Serial.println(m.mc_10p0);

#ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
    Serial.print("NC  0.5: ");
    Serial.println(m.nc_0p5);
    Serial.print("NC  1.0: ");
    Serial.println(m.nc_1p0);
    Serial.print("NC  2.5: ");
    Serial.println(m.nc_2p5);
    Serial.print("NC  4.0: ");
    Serial.println(m.nc_4p0);
    Serial.print("NC 10.0: ");
    Serial.println(m.nc_10p0);

    Serial.print("Typical partical size: ");
    Serial.println(m.typical_particle_size);
#endif
#else
    // since all values include particles smaller than X, if we want to create buckets we 
    // need to subtract the smaller particle count. 
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

    Serial.print(m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_1p0  - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5  - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0  - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
   


#endif /* PLOTTER_FORMAT */




  }
 answer = sendATcommand("AT+CLBS=1","+CLBS:",1000); // request info from GPS

if (answer == 1)
{
counter = 0;
do{
while(mySerial.available() == 0);
gps_data[counter] = mySerial.read();
counter++;
}
while(counter<22);
gps_data[counter] = '\0';
if(gps_data[0] == ',')
{
Serial.println("No GPS data available"); 
}
else
{
  int len=strlen(gps_data);
  //Ensure that English letters and other non-numeric symbols do not appear
    for(int i=0;i<len+1;i++){
      if(gps_data[i]=='a'){
        gps_data[i]='1';
      }
      if(gps_data[i]=='b'){
        gps_data[i]='2';
      }
      if(gps_data[i]=='c'){
        gps_data[i]='3';
      }
      if(gps_data[i]=='d'){
        gps_data[i]='4';
      }
      if(gps_data[i]=='e'){
        gps_data[i]='5';
      }
      if(gps_data[i]=='f'){
        gps_data[i]='6';
      }
      if(gps_data[i]=='g'){
        gps_data[i]='7';
      }
      if(gps_data[i]=='h'){
        gps_data[i]='8';
      }
      if(gps_data[i]=='i'){
        gps_data[i]='9';
      }
      if(gps_data[i]=='^'){
        gps_data[i]='.';
      }
      if(gps_data[i]=='/'){
        gps_data[i]='.';
      }
      if(gps_data[i]=='`'){
        gps_data[i]='0';
      }
      
    }
Serial.print("GPS data:");
strncpy( la_, gps_data+3  , 9);
    strncpy( lon_, gps_data+13, 9);
    Serial.println(gps_data);
    Serial.println(la_); 
    Serial.println(lon_); 

int hei=bmp.readAltitude(1019.66);
Serial.print(F("Approx altitude = "));
Serial.print(int(hei)); //The "1019.66" is the pressure(hPa) at sea level in day
Serial.println(" m");      
Serial.println();

 
String payload = "{";
payload += t; payload += ",";
payload += h; payload += ",";
payload += m.mc_2p5;payload += ",";
payload +=la_;payload += ",";
payload +=lon_;payload += ",";
payload +=hei;
payload += "}";
// Declare character array
byte arrsize = payload.length()+1;
char attributes[arrsize];
// Convert string to character array format
payload.toCharArray( attributes, arrsize);
Serial.println(attributes);


//PUBLISH MESSAGE  
Serial.println("Publish Temp&Humi&PM2.5&Flight data...");
mySerial.println("AT+CMQTTTOPIC=0,7"); //AT Command for Setting up the Publish Topic Name
delay(1000);
mySerial.println(Publish); //Topic Name
delay(1000);
mySerial.println("AT+CMQTTPAYLOAD=0,41"); //Payload length
delay(1000);
mySerial.println(attributes); //Payload message
delay(1000);
mySerial.println("AT+CMQTTPUB=0,1,60"); //Acknowledgment
  
    delay(6000);
  }
  
}
  
else{
  Serial.println("gps error");
  }
}



//The sendATcommand function
int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

uint8_t x=0, answer=0;
char response[100];
unsigned long previous;

memset(response, '\0', 100); // Initialize the string
delay(100);
while( Serial.available() > 0) Serial.read(); // Clean the input buffer
mySerial.println(ATcommand); // Send the AT command 
x = 0;
previous = millis();


// this loop waits for the answer
do{
if(mySerial.available() != 0){ 
response[x] = mySerial.read();
x++;
// check if the desired answer is in the response of the module
if (strstr(response, expected_answer1) != NULL) 
{
answer = 1;
}
}
// Waits for the asnwer with time out
}
while((answer == 0) && ((millis() - previous) < timeout)); 

 //Serial.println(response); 

return answer;
}