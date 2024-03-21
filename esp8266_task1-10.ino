#include<Adafruit_NeoPixel.h>
// LED和按钮的引脚编号
const int redLedPin = 5;
const int yellowLedPin = 4;
const int greenLedPin = 0;
const int startButtonPin = 12; // 启动按钮
const int stopButtonPin = 13;  // 停止按钮

// 控制状态和时间记录变量
bool running = false; 
unsigned long previousMillis = 0; 
unsigned long previousTwoMillis = 0; 
unsigned long lastBlinkMillis = 0;
const long redInterval = 3000;    // 红灯亮的持续时间
const long yellowInterval = 500;  // 黄灯闪烁的间隔
const long greenInterval = 3000;  // 绿灯亮的持续时间
const int yellowBlinkingInterval = 500;   //黄灯闪烁的间隔
int yellowCoununter = 0;  //黄灯闪烁的计数器
int Counter = 0;
// 状态枚举
enum TrafficState {RED, YELLOW_ON, YELLOW_OFF, GREEN};
TrafficState state = RED;

void setup() {
  // 初始化串口通信
  Serial.begin(115200);

  // 设置LED和按钮引脚
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if(yellowCoununter == 0&& Counter == 0)
  {
  previousMillis = currentMillis;
    Counter = 1;
  }
  int startButtonState = digitalRead(startButtonPin);
  int stopButtonState = digitalRead(stopButtonPin);

  if (startButtonState == HIGH) {
    running = true;
  }

  if (stopButtonState == HIGH) {
    running = false;
    turnOffLights();
    state = RED; // 重置状态为红灯，以便下次启动时从红灯开始
  }

  if (running) {
    switch(state) {
      case RED:
        changeLight(redLedPin, "Red");
        if (currentMillis - previousMillis >= redInterval) {
          previousMillis = currentMillis;
          turnOffLights();
          state = YELLOW_ON;
          lastBlinkMillis = millis();
        }
        break;
      case YELLOW_ON:
        if (currentMillis - previousMillis >= yellowInterval) {
          Serial.println("Yellow Blinking");
          if(currentMillis - lastBlinkMillis > yellowBlinkingInterval)
          {
             // 更新上次闪烁时间
            lastBlinkMillis = currentMillis;
            
            // 切换黄灯状态
            digitalWrite(yellowLedPin, !digitalRead(yellowLedPin));
            Serial.println("Yellow Blinking");
            
            // 增加黄灯闪烁计数器
            yellowCoununter++;
          }
          if(yellowCoununter == 6)
          {
          previousMillis = currentMillis;
          state = GREEN;
            yellowCoununter = 0;
          }
        }
        break;
      case YELLOW_OFF:
        if (currentMillis - previousMillis >= yellowInterval) {
          digitalWrite(yellowLedPin, LOW);
          previousMillis = currentMillis;
          state = GREEN;
        }
        break;
      case GREEN:
          changeLight(greenLedPin, "Green");
        if (currentMillis - previousMillis >= greenInterval) {
          previousMillis = currentMillis;
          state = RED;
          Counter = 0;
        }
        break;
    }
  }
}

void changeLight(int ledPin, String color) {
  turnOffLights();
  digitalWrite(ledPin, HIGH);
  Serial.println(color);
  Serial.println("许阳，2022030332");
}

void turnOffLights() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}

