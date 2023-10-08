int Relays[4] = {2,3,4,5};
bool state[4] = {0,0,0,0};
int Switches[4] = {A0,A1,A2,A4};
int s_state[4] = {0,0,0,0};
char a = '\0',b='\0';
int pin_select = 0;
void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<4;i++){
  pinMode(Relays[i],OUTPUT);
  digitalWrite(Relays[i],HIGH);
  pinMode(Switches[i],INPUT_PULLUP);
  s_state[i]=!digitalRead(Switches[i]);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    a=Serial.read();
    while(a=='@')
    {
     // Serial.println("listining to command");
     if(Serial.available()>0){
        b=Serial.read();
        if(b>='2'&&b<='9'){
          pin_select=b-'1';
         // Serial.println("pin selected");
        }
        else if(b=='0'||b=='1'){
          if(pin_select){
            state[pin_select-1]=(b=='1');
            digitalWrite(Relays[pin_select-1],!state[pin_select-1]);
           // Serial.print("Pin: ");
            //Serial.print(Relays[pin_select-1]);
           // Serial.print("\tstate: ");
           // Serial.println(state[pin_select-1]);
            pin_select=0;
            a='\0';
           // Serial.println("finish");
          }
        }
        else{
          pin_select=0;
          a='\0';
        //  Serial.println("finish");
        }
      }
    }
  }
  for(int i=0;i<4;i++){
    bool a=!digitalRead(Switches[i]);
    if(s_state[i]!=a){
      s_state[i]=a;
      state[i]=(!state[i]);
      Serial.print("#");
      Serial.print(i+2);
      Serial.println((int)state[i]);
      digitalWrite(Relays[i],!state[i]);
      delay(20);
    }
  }
}
