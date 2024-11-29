#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

const char* ssid = "Prophet";
const char* password = "1mProphet";
const char* mqttServer = "broker.hivemq.com";  // MQTT伺服器位址
const char* mqttUserName = "Prophet_Car";  // 使用者名稱，隨意設定。
const char* mqttPwd = "12345679";  // MQTT密碼
const char* clientID = "Prophet";      // 用戶端ID，隨意設定。
const char* topic = "/AGiLBERSPBCcqU";

unsigned long prevMillis = 0;  // 暫存經過時間（毫秒）
const long interval = 1000;  // 上傳資料的間隔時間，20秒。
String msgStr = "";      // 暫存MQTT訊息字串
String Direction="";
String Car_Front="";
String Available="";
long duration, cm;
long duration2, cm2;




int temp;  // 暫存溫度
int hum;   // 暫存濕度
int DP5;
int DP6;
int DP7;


WiFiClient espClient;
PubSubClient client(espClient);
//dht11 DHT11;

void setup_wifi() {
  delay(10);
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // 等5秒之後再重試
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  digitalWrite(D4, LOW);
  delayMicroseconds(5);
  digitalWrite(D4, HIGH);     // 給 Trig 高電位，持續 10微秒  
  delayMicroseconds(10);
  digitalWrite(D4, LOW);
  duration = pulseIn(D3, HIGH);   // 收到高電位時的時間
  cm = (duration/2) / 29.1; 

  digitalWrite(D6, LOW);
  delayMicroseconds(5);
  digitalWrite(D6, HIGH);
  delayMicroseconds(10);
  digitalWrite(D6, LOW);
  duration2 = pulseIn(D5, HIGH);
  cm2 = (duration2/2) / 29.1;
  Serial.print(cm2);
  Serial.println("cm2");

  //Car_Front
    if(cm<20||cm2<20){
          Car_Front="Block";
          digitalWrite(D2, HIGH);
    }
    else{
         Car_Front="Clean";
         digitalWrite(D2, LOW);
    }

  
  // 等待20秒
  if (millis() - prevMillis > interval) {
    prevMillis = millis();
    DP5=digitalRead(D0);
    DP6=digitalRead(D1);
    DP7=digitalRead(D2);
    //Direction
    if(DP5)
          Direction="Dining Out";
    else
          Direction="Finish";
      //Available
    if(DP6){
          Available="Waiting Commd";
        }
    else
          Available="Useing";

      
 //   // 讀取DHT11的溫濕度資料
//    int chk = DHT11.read(DHT11PIN);
 //   if (chk == 0) {
 //    temp = DHT11.temperature;
 //     hum = DHT11.humidity;
 temp++;
 hum++;
    

    // 組合MQTT訊息；field1填入溫度、field2填入濕度
    msgStr=msgStr+" Direction "+Direction+"  Car_Front "+Car_Front+" Available "+Available;
    
    // 宣告字元陣列
    byte arrSize = msgStr.length() + 1;
    char msg[arrSize];

    Serial.print("Publish message: ");
    Serial.println(msgStr);
    msgStr.toCharArray(msg, arrSize); // 把String字串轉換成字元陣列格式
    client.publish(topic, msg);       // 發布MQTT主題與訊息
    msgStr = "";
  }
}
