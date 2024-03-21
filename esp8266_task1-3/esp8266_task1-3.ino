// LED和按钮的引脚编号
const int redLedPin = 5;
const int yellowLedPin = 4;
const int greenLedPin = 0;
const int redButtonPin = 12;
const int yellowButtonPin = 13;
const int greenButtonPin = 14;

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  
  // 设置LED引脚为输出模式
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  // 设置按钮引脚为输入模式
  pinMode(redButtonPin, INPUT);
  pinMode(yellowButtonPin, INPUT);
  pinMode(greenButtonPin, INPUT);
}

void loop() {
  // 读取每个按钮的状态
  int redButtonState = digitalRead(redButtonPin);
  int yellowButtonState = digitalRead(yellowButtonPin);
  int greenButtonState = digitalRead(greenButtonPin);

  // 根据按钮状态控制LED灯
  if (redButtonState == LOW) {
    digitalWrite(redLedPin, 0);
  } else {
    digitalWrite(redLedPin, 1);
    Serial.println("Red");
    Serial.println("许阳，2022030332");
  }

  if (yellowButtonState == LOW) {
    digitalWrite(yellowLedPin, 0);
  } else {
    digitalWrite(yellowLedPin, 1);
    Serial.println("Yellow");
    Serial.println("许阳，2022030332");
  }

  if (greenButtonState == LOW) {
    digitalWrite(greenLedPin, 0);
  } else {
    digitalWrite(greenLedPin, 1);
    Serial.println("Green");
    Serial.println("许阳，2022030332");
  }

  // 稍作延迟，防止过快刷新
  delay(100);
}

