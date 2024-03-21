#include<Adafruit_NeoPixel.h>
#define PIN    13
#define NUMPIXELS      14
Adafruit_NeoPixel pixels=
Adafruit_NeoPixel(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);
int color_index=0;

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

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

int input;
input=analogRead(A0);
Serial.println(input);
if (currentMillis - previousMillis >2000){
    previousMillis = millis();
    set4led(0,255,0,input);

  }

}
