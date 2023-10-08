#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include<SoftwareSerial.h>
//SoftwareSerial espSerial(13,15);

String switchs[4] = {"0","1","2","3"};
bool state[4] = {0,0,0,0};
#define LED_G D1//define green LED pin
#define LED_R D0 //define red LED
char a = '\0',b='\0';
int pin_select = 0;

//paste the hidden code id: 1
//This part of code is removed for confidential information

//paste the hidden code id: 2
//This part of code is removed for confidential information
// #define wifiName "ALIGATOR 7499"
// #define password "12341234"

bool flag=false;

WiFiClient wifiClient;
 
 void Change(int a){
  int val = Firebase.getInt("switches/"+switchs[a]);
  if(val != state[a]){
      state[a]=val;
      Serial.print("@");
       Serial.print(a+2);
       Serial.println((int)state[a]);
   delay(20);
  }

}

void putvalue(int a,bool b){
  Firebase.setInt("switches/"+switchs[a], b?1:0);
}
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  //Serial.println();
  //espSerial.begin(9600);
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, HIGH);
  WiFi.begin(wifiName, password);
 while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_G, HIGH);
   delay(500);
   //Serial.println("Connecting to WiFi...");
    digitalWrite(LED_G, LOW);
   delay(500);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    if(WiFi.status() == WL_CONNECTED){
     Serial.println("Connected to WiFi and Firebase"); 
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_R, LOW);
    }

}
void loop() 
{
  for(int i=0;i<4;i++)
  Change(i);
  if(Serial.available()>0){
    a=Serial.read();
    while(a=='#')
    {
      //Serial.println("listining to command");
     if(Serial.available()>0){
        b=Serial.read();
        if(b>='2'&&b<='9'){
          pin_select=b-'1';
         // Serial.println("pin selected");
        }
        else if(b=='0'||b=='1'){
          if(pin_select){
            state[pin_select-1]=(b=='1');
            putvalue(pin_select-1,state[pin_select-1]);
            pin_select=0;
            a='\0';
           // Serial.println("finish");
          }
        }
        else{
          pin_select=0;
          a='\0';
         // Serial.println("finish");
        }
      }
    }
  }
  while(WiFi.status() != WL_CONNECTED){
    digitalWrite(LED_R, HIGH);
    //Serial.println("Wifi disconnected");
    flag=true;
    digitalWrite(LED_G, HIGH);
    delay(500);
    digitalWrite(LED_G, LOW);
    delay(500);
  }
  if(WiFi.status() == WL_CONNECTED&&flag){
    Serial.println("Wifi connected");
    flag=false;
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, LOW);
  }
  delay(10);
}
