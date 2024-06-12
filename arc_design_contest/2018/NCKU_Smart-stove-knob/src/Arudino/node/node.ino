
#include <Wire.h> // call library
#define ADT7420_Adresse 0x4B // I2C adress of the Pmod TMP2
#define BT1_RxD 8
#define BT1_TxD 9
#define infrared_pin 5
#define smoke_pin 11

//Déclaration d'un port série
#include <SoftwareSerial.h>
#include <Servo.h> 
Servo myservo;
SoftwareSerial BT1(BT1_RxD,BT1_TxD); 

int MSB;
int LSB;
int valeur;
int temperature;
int stove_on_cnt=0;
char stove_condition=0;
int pos = 180;   // variable to store the servo position
char dirction = 0;
int runtime = 0;
int smoke=0;
int human_state;

void setup()
{
 Serial.begin(9600); // initialization of serial communication
 myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
 myservo.write(pos);
 Wire.begin(); // initialization of I2C communication
 Init_ADT7420(); // initialisztion of Pmod AD2 module
 BT1.begin(9600);
}

void loop()
{
  if(runtime<1)
  {
    BT1.begin(9600);
    if(BT1.available()>0)
    {
      dirction = BT1.read();
      if(dirction == 'r')
      {
        for(pos = 180; pos >= 0; pos -= 1) // goes from 0 degrees to 180 degrees 
        {                                  // in steps of 1 degree 
          myservo.write(pos);              // tell servo to go to position in variable 'pos' 
          delay(20);          // waits 15ms for the servo to reach the position 
          runtime++;
        }
        Serial.println("Stove has been turned off");
      }
    }
  }
  Wire.beginTransmission(ADT7420_Adresse); // Launch of the measure
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(ADT7420_Adresse, 2); // Recovery of the two bytes MSB and LSB
  if(Wire.available() <=2)
  {
    MSB = Wire.read();
    LSB = Wire.read();
  }
  valeur=(MSB<<8)|LSB ;
  temperature=valeur/128;
 /*if (((valeur>>15)&1)==0) // If the temperature is positive
 {
  temperature=valeur/128;
 }
 else // If the temperature is negative
 {
  temperature=(valeur-65535)/128;
 }
/* Serial.print("MSB="); // display in serial monitor
 Serial.println(MSB);
 Serial.print("LSB=");
 Serial.println(LSB);
 Serial.print("Valeur=");
 Serial.println(valeur);
*/ Serial.print("Temperature=");
  Serial.println(temperature);
  //delay(100); 
  //if(temperature>=40) 
  //{
    if(stove_on_cnt<1){
      Serial.println("STOVE ON!");
      stove_on_cnt++;
    }
    human_state = digitalRead(infrared_pin);
    smoke = digitalRead(smoke_pin);
    if(smoke == LOW) stove_condition = 's'; 
    else if(human_state == HIGH) stove_condition = 'h';
    else stove_condition = 'n';  
  //}
  //else stove_condition = 'l';
  BT1.write(stove_condition);
  delay(1000);
}
// Initialization of Pmod TMP2 module
void Init_ADT7420(void)//temp
{
// Configuring the ADT7420 in 16 bit mode
 Wire.beginTransmission(ADT7420_Adresse);
 Wire.write(0x03);
 Wire.write(0x80);
 Wire.endTransmission();
}
