#include "WiFiEspAT.h"
#include <PubSubClient.h>
#include "SoftwareSerial.h"
#include <SimplePgSQL.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <sps30.h>

#define ESP8266 Serial2            // Define ESP-01 pins
#define Wea Serial3                // Define weather module pins

/*WiFi setting*/
WiFiClient espClient;
PubSubClient client(espClient);
#define ssid "***"                 // Set WiFi ssid
#define password "***"             // Set WiFi password
#define MqttServer "***"           // Set Broker address
#define Clientid "***"             // Set Client name

/*Date and time variables*/
String formattedDate;           
String dayStamp;
String timeStamp;

/*Database setting*/
IPAddress PGIP(***, ***, **, **);
const char user[] = "***";         // Set database user
const char password[] = "***";     // Set database password
const char dbname[] = "***";       // Set datatable
char buffer[1024];
PGconnection conn(&espclient, 0, 2048, buffer);

int status = WL_IDLE_STATUS;
unsigned long lasttime;

const int SLAVE_ADDRESS = 0x3C;    // Set I2C address
char databuffer[35];
double temp;
String messageTemp;
float uav_lat, uav_lon, uav_temperature, uav_humidity, uav_pm25;
int uav_height,uav_speed,uav_signal; 
float a, b, d, e;  
int a1, a2, a3, a4, b1, b2, b3, b4, c, d1, d2, e1, e2, a5, A, B, C;
int I = 0;
String str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, topic1;

void getBuffer() //Get weather status data
{
    int index;
    for (index = 0; index < 35; index++)
    {
        if (Ser1.available())
        {
            databuffer[index] = Ser1.read();

            if (databuffer[0] != 'c')
            {
                index = -1;
            }
        }
        else
        {
            index--;
        }
    }
    
}

int transCharToInt(char *_buffer, int _start, int _stop) //char to int）
{
    int _index;
    int result = 0;
    int num = _stop - _start + 1;
    int _temp[num];
    for (_index = _start; _index <= _stop; _index++)
    {
        _temp[_index - _start] = _buffer[_index] - '0';
        result = 10 * result + _temp[_index - _start];
    }
    return result;
}

int WindDirection()       // Wind Direction
{
    return transCharToInt(databuffer, 1, 3);
}

double WindSpeedAverage() // Air Speed (1 minute)
{
    temp = 0.44704 * transCharToInt(databuffer, 5, 7);
    return temp;
}

double WindSpeedMax()     // Max air speed (5 minutes)
{
    temp = 0.44704 * transCharToInt(databuffer, 9, 11);
    return temp;
}

double Temperature()      // Temperature ("C")
{
    temp = (transCharToInt(databuffer, 13, 15) - 32.00) * 5.00 / 9.00;
    return temp;
}

double RainfallOneHour()  // Rainfall (1 hour)
{
    temp = transCharToInt(databuffer, 17, 19) * 25.40 * 0.01;
    return temp;
}

double RainfallOneDay()   // Rainfall (24 hours)
{
    temp = transCharToInt(databuffer, 21, 23) * 25.40 * 0.01;
    return temp;
}

int Humidity()            // Humidity
{
    return transCharToInt(databuffer, 25, 26);
}

double BarPressure()      // Barometric Pressure
{
    temp = transCharToInt(databuffer, 28, 32);
    return temp / 10.00;
}

void doPg(void)
{
    char *msg;
    int rc;

    /* Data connection*/
    if (!pg_status)
    {
        conn.setDbLogin(PGIP,
                        user,
                        password,
                        dbname, "utf8", 5432);
        pg_status = 1;
        Serial.println("conn.setDbLogin");
        return;
    }
    if (pg_status == 1)
    {
        rc = conn.status();
        if (rc == CONNECTION_BAD || rc == CONNECTION_NEEDED)
        {
            char *c = conn.getMessage();
            if (c)
                Serial.println(c);
            pg_status = -1;
        }
        else if (rc == CONNECTION_OK)
        {
            pg_status = 2;
            Serial.println("Enter query");
        }
        return;
    }
    if (pg_status == 2)
    {
        /* Upload UAV data and weather station data to database*/

        /* Part of UAV data*/
        formattedDate = timeClient.getFormattedDate(); // Immediate time
        String uav_query = "insert into uav_data(timestamp, uav_latlon, uav_height, uav_temp, uav_hum, uav_pm25)values(";
        uav_query += formattedDate;
        uav_query += ",";
        uav_query += "'point(";
        uav_query += uav_lon;
        uav_query += " ";
        uav_query += uav_lat;
        uav_query += ")',";
        uav_query += uav_height;
        uav_query += ",";
        uav_query += uav_temperature;
        uav_query += ",";
        uav_query += uav_humidity;
        uav_query += ",";
        uav_query += uav_pm25;
        uav_query += ");";

        /* Part of weather station data*/
        int wind_direction = WindDirection();
        float av_windspeed = WindSpeedAverage();
        float max_windspeed = WindSpeedMax();
        float rain_hour = RainfallOneHour();
        float rain_day = RainfallOneDay();
        float temperature = Temperature();
        int humidity = Humidity();
        float atm_pressure = BarPressure();
        float pm225 = PM;
        float lon = 121.54733;
        float lat = 25.019341;
        
        String station_query = "insert into station(timestamp, latlon, wind_direction,av_windspeed,max_windspeed,rain_hour,rain_day,temperature,humidity,atm_pressure,pm25) values(";
        station_query += formattedDate;
        station_query += ",";
        station_query += "'point(";
        station_query += lon;
        station_query += " ";
        station_query += lat;
        station_query += ")',";
        station_query += wind_direction;
        station_query += ",";
        station_query += av_windspeed;
        station_query += ",";
        station_query += max_windspeed;
        station_query += ",";
        station_query += rain_hour;
        station_query += ",";
        station_query += rain_day;
        station_query += ",";
        station_query += temperature;
        station_query += ",";
        station_query += humidity;
        station_query += ",";
        station_query += atm_pressure;
        station_query += ",";
        station_query += pm225;
        station_query += ");";

        String fin = uav_query + station_query;

        char fin_char[fin.length() + 1];
        fin.toCharArray(fin_char, fin.length() + 1);
        Serial.println(fin_char);
        conn.execute(fin_char);
        if (conn.execute(fin_char))
        {
            Serial.println("inser uav_data");
        }
        //Serial.println(fin_char);
        pg_status = 0;
        conn.close();
        return;
    }
error:
    msg = conn.getMessage();
    if (msg)
        Serial.println(msg);
    else
        Serial.println("UNKNOWN ERROR");
    if (conn.status() == CONNECTION_BAD)
    {
        Serial.println("Connection is bad");
        pg_status = -1;
    }
    conn.close();
}


void setWiFi()
{
  // Initialize the soft serial port, connect the soft serial port to the ESP-01
  soft.begin(9600);
  // Initialize ESP-01
  WiFi.init(&soft);
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("No WiFi shield");
    while (true);
  }
    Serial.println("[InitWiFi]Connecting to AP ...");
  // Try WiFi connection
  while ( status != WL_CONNECTED)
  {
    Serial.print("[InitWiFi]Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
  // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, password);
    delay(500);
  }
    Serial.println("[InitWiFi]Connected to SSID");
}

void reconnect()
{
  // Keep looping until the MQTT server is connected
 while (!client.connected())
 {
  Serial.print("[reconnect]Connecting to MQTT Server ...");
  // Try to connect
  if ( client.connect(Clientid) )
  {
     Serial.println( "OK" );
     // Subscribe message from Broker
     if (I == 0)
     {
         client.subscribe("***");  // The topic is the same as flight Broadcast APP
     }   
     if (I == 1)
     {
         client.subscribe("***");  // The topic is the same as UAV sensor module
     }
  }
  else 
  {
     Serial.print( "FAILED" );
     Serial.print( client.state() );
     // Wait 5 seconds before retrying
     Serial.println( " : retrying in 5 seconds]" );
     delay( 5000 );
  }
 }
}

 /*Callback function header*/
void callback(char* topic, byte*message, unsigned int length) {
  for (int i = 0; i < length; i++)
     {
        //Serial.print((char)message[i]);
        messageTemp += (char)message[i];
     }
  if (String(topic) == "***")  
  {    
     // Collect data of flight Broadcast APP, wind direction and wind speed
     String uavspeed = messageTemp.substring(0);
     String signallevel = messageTemp.substring(1,3);
     uav_speed = uavspeed.toInt();
     uav_signal = signallevel.toInt();
     topic1 = String(topic);
  }
  else if (String(topic) == "***")
  {
     // Collect data of UAV sensor module
     String tem = messageTemp.substring(1, 6);
     String hum = messageTemp.substring(7, 12);
     String pm25 = messageTemp.substring(13, 17);
     String lat = messageTemp.substring(18, 27);
     String lon = messageTemp.substring(28, 37);
     String height = messageTemp.substring(38, 40);
     uav_lat = lat.toFloat();
     uav_lon = lon.toFloat();
     uav_height = height.toInt();
     uav_temperature = tem.toFloat();
     uav_humidity = hum.toFloat();
     uav_pm25 = pm25.toFloat();
     topic1 = String(topic);
  }   
}

/* Function that executes whenever data is requested by master*/
void requestEvent()  
{
   if (topic1 == "***") // Send data to UAV risk model by I2C
   { 
     Wire.write(e1);
     Wire.write(e2);
     Wire.write(d1);
     Wire.write(uav_speed);
     Wire.write(uav_signal);
   }
   else if (topic1 == "***") // Send data to regulatory algorithm by I2C
   {
     Wire.write(a1);
     Wire.write(a2);
     Wire.write(a3);
     Wire.write(a4);
     Wire.write(b1);
     Wire.write(b2);
     Wire.write(b3);
     Wire.write(b4);
     Wire.write(c);
     Wire.write(d1);
     Wire.write(d2);
   }
}

/*The function that executes whenever data is received from master*/
void receiveEvent(int howMany)
{
  byte counter = 0; 
  while (Wire.available())  
  {
    // Get UAV risk level from UAV risk model by I2C
    if (topic1 == "***")  
    {
      if(counter==0)
      {
        A = Wire.read();
        counter++;    
      }
      else
      {
        a5=Wire.read();
      }
      if (A < 6)
      { 
        // Publish UAV risk level to flight Broadcast APP
        client.publish("***", String(A).c_str(), true);
      }
      I = 1;
      // Unsubscribe topic
      client.unsubscribe("***");
    }
    // Get flight instruction and wind speed level from regulatory algorithm by I2C
    else if (topic1 == "***")
    {
     if(counter==0){
        B = Wire.read();
        counter++;    
     }
     else if(counter==1){
        C = Wire.read();
        counter++;    
     }
     else
     {
        a5=Wire.read();
     }
     if (B < 12)
     { 
       if (B == 1)
       {
          // Publish flight instruction to flight broadcast APP
          client.publish("***", "Warning!! You are now entering Restricted Area. Please fly higher.");
       }
       else if (B == 2)
       {
          client.publish("***", "Stop!! You are now entering Restricted Area. Please leave this area quckly.");
       }
       else if (B == 3)
       {
          client.publish("***", "Warning!! You are now entering Alert Area(UAV Action: R5). Please be careful to fly.");
       }
       else if (B == 4)
       {
          client.publish("***", "Safe area.");
       }
     }
     if (C < 12)
     {
       if (C == 5)
       {
          // Publish wind speed level to flight broadcast APP
          client.publish("***", "The wind is Calm. The wind risk level is 0.");
       }
       else if (C == 6)
       {
          client.publish("***", "The wind is Light Air. The wind risk level is 1.");
       }
       else if (C == 7)
       {
          client.publish("***", "The wind is Light Breeze. The wind risk level is 2.");
       }
       else if (C == 8)
       {
          client.publish("***", "The wind is Gentle Breeze. The wind risk level is 3.");
       }
       else if (C == 9)
       {
          client.publish("***", "The wind is Fresh Breeze. The wind risk level is 4.");
       }
       else if (C == 10)
       {
          client.publish("***", "Warning!! The wind is Fresh Breeze. The wind risk level is 5. Please land or return instantly.");
       }
       else if (C == 11)
       {
          client.publish("***", "The wind is Strong Breeze. The wind risk level is max. Don't fly in the area.");
       }
     }
    }
  }
}

void setup()
{
   int16_t ret;
   Serial.begin(115200);
   Wea.begin(9600);
   setWiFi();
   // Connect to MQTT Broker (Broker port is 1883)
   client.setServer( MqttServer, 1883 );
   // Set the corresponding behavior to be executed after receiving the callback
   client.setCallback(callback);
   Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address
   Wire.onRequest(requestEvent); // register event
   Wire.onReceive(receiveEvent); // register event
   while (sps30_probe() != 0) {
    //Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
  // SPS30 PM2.5 sensor setting
#ifndef PLOTTER_FORMAT
  //Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_start_measurement();
  if (ret < 0) {
    //Serial.print("error starting measurement\n");}

#ifndef PLOTTER_FORMAT
  Serial.print("measurements started\n");
#endif /* PLOTTER_FORMAT */
}

void loop()
{
   struct sps30_measurement m;
   char serial[SPS30_MAX_SERIAL_LEN];
   uint16_t data_ready;
   int16_t ret;
   status = WiFi.status();
   if ( status != WL_CONNECTED)
   {
      while ( status != WL_CONNECTED)
      {
         //Serial.print("[loop()]Attempting to connect to WPA SSID: ");
         //Serial.println(ssid);
         // Connect to WiFi
         status = WiFi.begin(ssid, password);
         delay(500);
      }
      //Serial.println("[loop()]Connected to SSID");
   }
   if ( !client.connected() )
   {
      reconnect();
   }
   client.loop();
   
   // Get weather station value
   getBuffer(); 
   // Split wind direction value 
   e = WindDirection();
   e1 = e * 0.1 ;
   e2 = (e * 0.1 - e1) * 10;
   // Split latitude value
   a = uav_lat;
   a1 = a;
   a2 = (a - a1) * 100;
   a3 = ((a - a1) * 100 - a2) * 100;
   a4 = (((a - a1) * 100 - a2) * 100 - a3)* 100;
   // Split latitude value
   b = uav_lon;
   b1 = b;
   b2 = (b - b1) * 100 ;
   b3 = ((b - b1) * 100 - b2) *100;
   b4 = (((b - b1) * 100 - b2) *100 - b3)*10; 
   c = uav_height;
   // Split maximum wind speed value
   d = WindSpeedMax();
   d1 = d;
   d2 = (d - d1) * 100;
   
   // Start SPS30 to monitor PM2.5 value
   do {
        ret = sps30_read_data_ready(&data_ready);
        if (ret < 0) {
           Serial.print("error reading data-ready flag: ");
           Serial.println(ret);
         }
        else
           break;
           delay(100); /* retry in 100ms */
        } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    //Serial.print("error reading measurement\n");
  } else {

#ifndef PLOTTER_FORMAT
    PM = m.mc_2p5;
#endif
  }
   doPg();
   messageTemp = "";
   delay(10000); 
}
