#include<Adafruit_NeoPixel.h>
#define PIN    12
#define NUMPIXELS      14
Adafruit_NeoPixel pixels=
Adafruit_NeoPixel(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);
int color_index=0;
uint8_t count = 0;

//the delay
unsigned long previousMillis = 0; 

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pixels.begin();
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

void set4led_new(uint8_t r_value,uint8_t g_value,uint8_t b_value)
{
  pixels.setPixelColor(0,r_value,g_value,b_value);
  pixels.setPixelColor(1,g_value,b_value,g_value);
  pixels.setPixelColor(2,b_value,g_value,r_value);
  pixels.setPixelColor(3,b_value,r_value,g_value);
  pixels.show();

  }


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

int input;
input=analogRead(A0);
pixels.setBrightness(input);
if (currentMillis - previousMillis >2000){
  count++;
Serial.println(count);
  //count = count %4;
  switch(count%4)
  {
      case(0):
  set4led_new(100,255,0);
  break;
      case(1):
  set4led_new(0,100,255);
  break;
      case(2):
  set4led_new(150,220,50);
  break;
      case(3):
  set4led_new(50,100,150);
  break;
  }

  previousMillis = millis();

  }

}
