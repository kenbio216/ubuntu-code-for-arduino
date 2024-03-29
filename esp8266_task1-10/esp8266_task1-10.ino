#include<Adafruit_NeoPixel.h>
#define PIN    12
#define NUMPIXELS      14
Adafruit_NeoPixel pixels=
Adafruit_NeoPixel(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);
int color_index=0;
uint8_t count = 0;

bool running = false;
const int startButtonPin = 4; // 启动按钮
const int stopButtonPin = 5;  // 停止按钮

//the delay
unsigned long previousMillis = 0; 

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pixels.begin();

  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);
}

void set4led(uint8_t r_value,uint8_t g_value,uint8_t b_value,uint8_t bright_value)
{
  pixels.setBrightness(bright_value);
  for(int i =0;i<NUMPIXELS;i++)
  {
    pixels.setPixelColor(i,r_value,g_value,b_value);
    }
    pixels.show();
  }  

void set4led_new(uint8_t r_value,uint8_t g_value,uint8_t b_value,uint8_t select)
{
  switch(select)
  {
    case(1):
  pixels.setPixelColor(0,r_value,g_value,b_value);
  //pixels.setPixelColor(1,g_value,b_value,g_value);
  //pixels.setPixelColor(2,b_value,g_value,r_value);
  //pixels.setPixelColor(3,b_value,r_value,g_value);
  pixels.show();
  break;
    case(2):
  //pixels.setPixelColor(0,r_value,g_value,b_value);
  pixels.setPixelColor(1,g_value,b_value,g_value);
  //pixels.setPixelColor(2,b_value,g_value,r_value);
  //pixels.setPixelColor(3,b_value,r_value,g_value);
  pixels.show();
  break;
    case(3):
  //pixels.setPixelColor(0,r_value,g_value,b_value);
  //pixels.setPixelColor(1,g_value,b_value,g_value);
  pixels.setPixelColor(2,b_value,g_value,r_value);
  //pixels.setPixelColor(3,b_value,r_value,g_value);
  pixels.show();
  break;
    case(4):
  //pixels.setPixelColor(0,r_value,g_value,b_value);
  //pixels.setPixelColor(1,g_value,b_value,g_value);
  //pixels.setPixelColor(2,b_value,g_value,r_value);
  pixels.setPixelColor(3,b_value,r_value,g_value);
  pixels.show();
  break;

    }

  }
void turnOled()
{
  for(int i =0;i<NUMPIXELS;i++)
  {
    pixels.setPixelColor(i,0,0,0);
    }
    pixels.show();
    
  }



void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  int startButtonState = digitalRead(startButtonPin);
  int stopButtonState = digitalRead(stopButtonPin);
  //Serial.println("startButtonPin:");
  //Serial.println(startButtonState);
  //Serial.println("stopButtonPin:");
  //Serial.println(stopButtonState);
  if (startButtonState == HIGH) {
    running = true;
  }

  if (stopButtonState == HIGH) {
    running = false;
  Serial.println("startButtonPin:");
  Serial.println(startButtonState);
  Serial.println("stopButtonPin:");
  Serial.println(stopButtonState);
  Serial.println("许阳，2022030332");
    turnOled();
  }
  //Serial.println("are you running?");
  //Serial.println(running);
  
 // if(running){
 //   set4led_new(100,255,0,count%4+1);

 //   }
  pixels.setBrightness(30);
if(running)
{
if (currentMillis - previousMillis >1000){
  count++;
  //count = count %4;
  Serial.println("startButtonPin:");
  Serial.println(startButtonState);
  Serial.println("stopButtonPin:");
  Serial.println(stopButtonState);
  Serial.println("许阳，2022030332");
  switch(count%4)
  {
      case(0):
    turnOled();
  set4led_new(100,255,0,1);
  delay(500);
  break;
      case(1):
    turnOled();
  set4led_new(0,100,255,2);
  delay(500);
  break;
      case(2):
    turnOled();
  set4led_new(150,220,50,3);
  delay(500);
  break;
      case(3):
    turnOled();
  set4led_new(50,100,150,4);
  delay(500);
  break;
  }

  previousMillis = millis();

  }

}

//
}
