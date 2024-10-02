#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//ArduinoJson 5.13.1
//esp8266 board 2.3.0.
//sudo chmod a+rw /dev/ttyUSB0


String switchs[4] = {"0","1","2","3"};
int buttons[] = {D1,D2,D3,D4};
bool button_state[4] = {1,1,1,1};
bool state[4] = {0,0,0,0};
int realys[] = {D5,D6,D7,D8};
char a = '\0',b='\0';
int pin_select = -1;

#define WIFI_SSID "ARC-WIFI"
#define WIFI_PASSWORD "arc@&$456!"
#define DATABASE_URL "arc-lock-6e5c6-default-rtdb.asia-southeast1.firebasedatabase.app"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


bool flag=false;

 
 void Change(int a){
 long start = millis();
  String val = Firebase.getString(fbdo, "switches/"+switchs[a]) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  long end = millis();
  //Serial.println("reading time: "+String(end-start));
  if(val=="ON"||val=="OFF"){
    if(val != ((state[a])?"ON":"OFF")){
      if(val == "ON")
        state[a] = true;
      else if(val == "OFF")
        state[a] = false;
      digitalWrite(realys[a],state[a]);
      Serial.println("switch "+String(a)+" is "+(state[a]?"ON":"OFF"));
    }
  }

}

void putvalue(int a,bool b){
  long start = millis();
  Firebase.setString(fbdo, "switches/"+switchs[a], b?"ON":"OFF") ? "ok" : fbdo.errorReason().c_str();
  long end = millis();
  Serial.println("writing time: "+String(end-start));
}
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  //Serial.println();
  //espSerial.begin(9600);
  for(int i=0;i<4;i++){
    pinMode(realys[i],OUTPUT);
    pinMode(buttons[i],INPUT_PULLUP);
    digitalWrite(realys[i],LOW);
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   //Serial.println("Connecting to WiFi...");
  }

  config.signer.test_mode = true;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = "EJf6A6wWjYycd06bLoLzD4ziGJjiwcdpHwzOxFYq";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

    if(WiFi.status() == WL_CONNECTED){
     Serial.println("Connected to WiFi and Firebase"); 
    }

}
void loop() 
{
  for(int i=0;i<4;i++)
  Change(i);
  for(int i=0;i<4;i++){
    bool read = digitalRead(buttons[i]);
    if(read!=button_state[i]){
      button_state[i] = !button_state[i];
      state[i] = !state[i];
      digitalWrite(realys[i],state[i]);
      Serial.println("switch "+String(i)+" is "+(state[i]?"ON":"OFF"));
      putvalue(i,state[i]);
    }
  }
  while(WiFi.status() != WL_CONNECTED){
    //Serial.println("Wifi disconnected");
    flag=true;
  }
  if(WiFi.status() == WL_CONNECTED&&flag){
    Serial.println("Wifi connected");
    flag=false;
  }
  delay(10);
}
