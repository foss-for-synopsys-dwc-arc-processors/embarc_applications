#include <SoftwareSerial.h>
#define book_ID_length 2
#define extension_bit 2
#define User_ID_length 5
#define command_length 9
#define end_of_cmd_index command_length - 1

/*define function code*/
#define Search 'A'
#define Borrow 'B'
#define GiveBack 'C'
#define New_Reservation 'D'
#define Cancel_Reservation 'E'

/*define error message*/
#define E_ARC_OK "M11Z"    /*!< ok */
#define E_ARC_Trans "M22Z" /*!< invalid cmd */
#define E_MPU_OK "M33Z"    /*!< ok */
#define E_MPU_Trans "M44Z" /*!< invalid cmd */
#define E_MPU_NoRes "M55Z"

int LED = 13;
const byte rxPin = 10;
const byte txPin = 11;

// set up a new serial object ->communivate with arc_emsk
SoftwareSerial mySerial(rxPin, txPin);

void setup()
{
  
  Serial.begin(9600);   /*PC baudrate*/
  Serial1.begin(57600); /*linkit 7688 duo mpu baud rate*/
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600); /*arc baud rate*/
  pinMode(LED, OUTPUT);
  //mySerial.listen();
}
void loop()
{
  if (mySerial.available() == command_length)
  {
    /*Read Data from ARC EMSK*/
    int strlength = mySerial.available();
    Serial.println(strlength);
    char uart_text[command_length] = {0};
    for (int i = 0; i < command_length; i++)
    {
      uart_text[i] = ' ';
    }
    for (int index = 0; index < command_length; index++)
    {
      uart_text[index] = mySerial.read();
    }
    /*Verify the gotten*/
    if (uart_text[end_of_cmd_index] == 'Z')
    {
      switch (uart_text[0])
      {
      case Search:
      case GiveBack:
      case Borrow:
      case New_Reservation:
      case Cancel_Reservation:
      {
        Serial.print(uart_text);
        /*send data to linkit 7688 MPU*/
        Serial1.write(uart_text, command_length);
        /*Wait and GET RESPONSE FORM LINKIT 7688 MPU*/
        char rev[6] = {0};
        int count = 0;
        unsigned long current_time = millis();
        while (millis() - current_time < 5000)
        {
          if (Serial1.available())
          {
            delay(10);
            /*Z is end of data from mySQL*/
            rev[count] = Serial1.read();
            if (rev[0] == '0')//mySQL server no response
            {
              count = -1; //indicate end of transmissting
              Serial.println(rev);
              delay(10);
              mySerial.write(E_MPU_Trans);
              break;
            }
            if (rev[count] == 'Z')
            {
              Serial1.flush();
              count = -1; //indicateend of transmissting
              delay(10);
              mySerial.write(rev);
              Serial.println(rev);
              break;
            }
            count++;
          }
        }
        if (count != -1)
        {
          delay(10);
          mySerial.write(E_MPU_NoRes);
        }
        break;
      }
      }
    }
    else
    {
      mySerial.write(E_ARC_Trans);
      mySerial.flush();
    }
  }
}
