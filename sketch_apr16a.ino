#include <LiquidCrystal_I2C.h>

#include <Servo.h>

#include <DHT.h>
#include <DHT_U.h>

#include <SoftwareSerial.h>

#include <Adafruit_Sensor.h>
#include "DHT.h" 
LiquidCrystal_I2C lcd(0x27,16,2);
#define DHTPIN 11   
 
#define DHTTYPE DHT11 
SoftwareSerial blue(9,8);
byte dracon[8] = {
 0b01110, 0b11011, 0b11111, 0b11100, 0b11111, 0b01100, 0b10010, 0b11011
};
byte derevo[8] = {
 0b00000, 0b00000, 0b00000, 0b11011, 0b11011, 0b11011, 0b01100, 0b01100
};
byte tas[8] = {
 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b11111
};
byte ptica[8] = {
 0b00100, 0b00101, 0b01111, 0b11111, 0b10100, 0b00100, 0b00000, 0b00000
};
DHT dht(11, DHTTYPE); 
int buttonPin=2;
int buzzer=10;
int x=A2;
int y=A3;
int red=3;
int index=0;
String menu[]={"1.Game","2.Sensors","3.Control","4.Led","5.Buzzer","6.Servo"};
boolean button;
// boolean left;
// boolean right;
// boolean up;
// boolean down;
String * current;
 int tyyy=0;
 int dyyy=0;
 int q=0;
 int jsb=5;
 Servo servo;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  blue.begin(9600);
  dht.begin(); 
  lcd.init();
  lcd.backlight();
 pinMode (buzzer, OUTPUT);
 pinMode (buttonPin, INPUT);     // подключаем кнопку
 analogWrite(buttonPin, LOW);
 pinMode(red,OUTPUT);
 servo.attach(6);
 lcd.setCursor(0,0);
lcd.print("Main menu");
lcd.setCursor(0,1);
lcd.print(menu[index]);
 lcd.createChar(0, dracon);
 lcd.createChar(1, derevo);
 lcd.createChar(2, tas);
 lcd.createChar(3, ptica);
 current=menu;
}

void loop() {
int joy_stick=readJoystick();
if(joy_stick!=jsb){
  if(!button){
    jsb=joy_stick;
    switch(joy_stick){
      case 0 :
        moveup();
      case 1 :
      movedown();
      break;
      default: break;
    }
  }
}
  if(digitalRead(buttonPin)==HIGH){
    lcd.clear();
    button=true;
  }
  if(button){
    if(current[index].equals(menu[0])){
      game();
      delay(300);
    }
    else if(current[index].equals(menu[1])){
      Humidity();
      delay(200);
    }
    else if(current[index].equals(menu[2])){
      WirelessControl();
      
      delay(200);
    }
    else  if(current[index].equals(menu[3])){
      led();
      delay(200);
    }
    else if(current[index].equals(menu[4])){
      zvyk();
      delay(200);
    }
    else if(current[index].equals(menu[5])){
      Servo();
      delay(200);
      button=false;
    }
  }

}
void led(){
  int a=analogRead(y);
  int value=map(y,0,1023,0,255);
  analogWrite(red,value);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Led value:  ");
  lcd.setCursor(0,1);
  lcd.print(value);
}
void zvyk(){
  int a=analogRead(y);
    int value=map(y,0,1023,0,20000);
    tone(buzzer , value);
    lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("Buzzer value:  ");
  lcd.setCursor(0,1);
  lcd.print(value);
}
void Servo(){
    int a=analogRead(y);
    int value=map(y,0,1023,0,180);
    servo.write(value);
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Servo value:  ");
  lcd.setCursor(0,1);
  lcd.print(value);
}
boolean connect=false; 
boolean ledd=false; 
boolean buzzerr=false; 
boolean servoo=false; 
 
void WirelessControl(){  
   if(blue.available()){ 
     connect=true; 
     
     lcd.clear(); 
     lcd.home(); 
     lcd.print("CONNECTED!"); 
  String temp=blue.readString();  
  Serial.println(temp); 
 
  if(temp=="Led"){     
    ledd=true; 
    Serial.println("Haha");
    delay(3000); 
  }else if(temp=="Buzzer"){ 
    buzzerr=true; 
    delay(3000); 
  }else if(temp=="Servo"){ 
    servoo=true; 
delay(3000);     
  } 
   
  if(ledd){ 
    int l=blue.readString().toInt(); 
    analogWrite(red,l);  
    ledd=false;  
    delay(1000);      
  }else if(buzzerr){ 
    int b=blue.readString().toInt(); 
    tone(buzzer,b , 5000); 
    buzzerr=false; 
    delay(1000);     
 
  }else if(servoo){ 
    int val=blue.readString().toInt(); 
    servo.write(val); 
    servoo=false; 
    delay(1000); 
  }   
   }
   else{ 
     lcd.clear(); 
     lcd.home(); 
     lcd.print("NOT CONNECTED!"); 
   }   
}


void moveup(){
  if(index<=0){
    index=sizeof(*current);
  }
  else{
    index--;
  }
  linea(1,current[index]);
  delay(150);
}
void linea(int line , String text){
  lcd.setCursor(0,line);
  lcd.print("       ");
  lcd.setCursor(0,line);
  lcd.print(text);
}
void movedown(){
  if(index>=sizeof(*current)){
    index=0;
  }
  else{
    index++;
  }
  linea(1,current[index]);
  delay(150);
}
    void Humidity(){
      delay(2000);
        float h = dht.readHumidity(); 
          float t = dht.readTemperature(); 
        float f = dht.readTemperature(true);
          if (isnan(h)||isnan(f)||isnan(t)) { 
    Serial.println(F("Failed to read from DHT sensor!")); 
    return; 
  }
    float hif = dht.computeHeatIndex(f, h); 
  float hic = dht.computeHeatIndex(t, h, false); 
          lcd.print("Humidity:");
          lcd.print(h);
          lcd.print("%");
          lcd.setCursor(0,1 );
              lcd.setCursor(0,1 );
  lcd.print("Temperature:");
  lcd.print(t);
  delay(500);
    }
int readJoystick(){
      x=analogRead(A2);
      y=analogRead(A3);
      int SwitchValue = digitalRead(buzzer);   
  SwitchValue = map(SwitchValue, 0, 1, 0, 1);
      int outp=0;
      if(x>=800){
        outp=1;
      }
      else if(x<=150){
        outp=3;
      }
      else if(y>=800){
        outp=2;
      }
      else if(y<=150){
        outp=0;
      }
      return outp;
}
  void game(){
    byte p = 0; 
    int pause = 400;
    int level = 1;
    startgame();
      byte d = 1;
      byte x1 = 15;
      byte y1 = 1;
      //припятствие 
        byte i = random (1, 4);
        if (i == 3) y1 = 0;
        else y1 = 1;
        while (x1 > 0) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(level);
                if (digitalRead(A1) == LOW) d = 1;
    if (digitalRead(A1) == HIGH) d = 0;
    if (p > 3) d = 1;
        lcd.setCursor(4, d);
    lcd.print(char(0));
        lcd.setCursor(x1, y1);
    tone(buzzer, 50);
    lcd.print(char(i));
    noTone(buzzer);
    if (x1 == 4 && y1 == d) {
      delay(400);
      tone(buzzer, 50);
      delay(100);
      noTone(buzzer);
      delay(100);
      tone(buzzer, 20);
      delay(300);
      noTone(10);
      lcd.clear();
      delay(200);
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER!");
      delay(600);
      lcd.clear();
      delay(400);
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER!");
      delay(600);
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("LEVEL: ");
      lcd.print(level);
      delay(400);
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER!");
      delay(3000);
      lcd.clear();
      // начинаем игру заново, обнулив уровень игры
      lcd.setCursor(7, 0);
      lcd.print("GO!");
      delay(400);
      tone(buzzer, 600);
      delay(100);
      noTone(buzzer);
      lcd.clear();
      level = 0;
      pause = 400;
      p = 0;
      y1 = 1;
      x1 = 0;
      break;
    }
    if (d == 0) {
       tone(buzzer, 200);
        delay(100); 
        noTone(buzzer); }
    else { 
      delay(100); }
          delay(pause);
          x = x - 1;
          p = p + 1;
          if (p > 4) p = 0; 
        }
        tone(buzzer, 800);
        delay(20);
        level = level + 1;
        pause = pause - 20;
        if (pause < 0) pause = 0; }
    void startgame(){
      lcd.setCursor(7, 0);
      lcd.print("GO!");
      delay(400);
      tone(buzzer, 600);
      delay(100);
      noTone(buzzer);
      lcd.clear();
    }