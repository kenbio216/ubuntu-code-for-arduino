#include <U8g2lib.h>
#include <Wire.h>
#include <math.h>
#include "paj7620.h"

// 手势识别模块需要的一些定义
#define GES_REACTION_TIME 100 // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME 100    // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_QUIT_TIME 0
uint8_t btn0;
uint8_t btn1;
uint8_t btn2;

// 消失函数需要的一些定义，指向buf首地址的指针
uint8_t *buf_ptr;
uint16_t buf_len;
// 多级菜单需要的一些定义
uint8_t ui_index, ui_state;
enum // ui_index
{
  M_LOGO,      // 开始界面
  M_SELECT,    // 选择界面
  M_PID,       // PID界面
  M_PID_EDIT,  // pid编辑
  M_ICON,      // icon界面
  M_CHART,     // 图表
  M_TEXT_EDIT, // 文字编辑
  M_VIDEO,     // 视频显示
  M_ABOUT,     // 关于本机
};
enum // ui_state
{
  S_NONE,
  S_DISAPPEAR,
  S_SWITCH,
  S_MENU_TO_MENU,
  S_MENU_TO_PIC,
  S_PIC_TO_MENU,
};

typedef struct
{
  byte val;
  byte last_val;
} KEY_T;
typedef struct
{
  byte id;
  byte press;
  byte res;
} KEY_MSG;

typedef struct
{
  char *str;
  byte len;
} SETTING_LIST;

SETTING_LIST list[] =
    {
        {"AnimeMode", 9},
        {"MoveMode", 8},
        {"About", 5},
};
SETTING_LIST animemode[] =
    {
        {"audio", 5},
        {"video", 5},
};
SETTING_LIST movemode[] =
    {
        {"manual", 6},
        {"automatic", 9},
};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
short x, x_trg; // x当前位置数值,x_trg 目标坐标值
short y = 18, y_trg = 18;

short frame_len, frame_len_trg; // 框框的宽度
short frame_y, frame_y_trg;     // 框框的y

int ui_select = 0;

int state;
KEY_T key[3] = {0};
KEY_MSG key_msg = {0};

byte get_io_val(byte ch)
{
  uint8_t temp0;
  switch (ch)
  {
  case 0:
    temp0 = btn0;
    btn0 = 0;
    return temp0;
    break;
  case 1:
    temp0 = btn1;
    btn1 = 0;
    return temp0;
    break;
  case 2:
    temp0 = btn2;
    btn2 = 0;
    return temp0;
  default:
    break;
  }
}

void key_init(void)
{
  for (int i = 0; i < 3; i++)
  {
    key[i].val = key[i].last_val = get_io_val(i);
  }
}

void key_scan(void)
{
  for (int i = 0; i < 3; i++)
  {
    key[i].val = get_io_val(i);
    if (key[i].val != key[i].last_val)
    {
      key[i].last_val = key[i].val;
      if (key[i].val == 0)
      {
        key_msg.id = i;
        key_msg.press = 1;
      }
    }
  }
}

int ui_run(short *a, short *a_trg, uint8_t step, uint8_t slow_cnt)
{
  uint8_t temp;

  temp = abs(*a_trg - *a) > slow_cnt ? step : 1;
  if (*a < *a_trg)
  {
    *a += temp;
  }
  else if (*a > *a_trg)
  {
    *a -= temp;
  }
  else
  {
    return 0;
  }
  return 1;
}

void setup(void)
{
  // 手势识别的9600波特率通信
  uint8_t error = 0;
  Serial.begin(9600);
  error = paj7620Init(); // initialize Paj7620 registers
  if (error)
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  Serial.println("Please input your gestures:\n");

  key_init();
  u8g2.begin();
  // u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_t0_22_mf); // 设置字体
  // u8g2.setFont(u8g2_font_unifont_t_chinese2);
  frame_len = frame_len_trg = list[ui_select].len * 13;

  // 消失函数有关
  buf_ptr = u8g2.getBufferPtr(); // 拿到buffer首地址
  buf_len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
}

void ui_show(void)
{
  int list_len = sizeof(list) / sizeof(SETTING_LIST);
  u8g2.clearBuffer(); // 清除内部缓冲区
  for (int i = 0; i < list_len; i++)
  {
    u8g2.drawStr(x + 2, y + i * 20, list[i].str); // 第一段输出位置
  }
  u8g2.drawRFrame(x, frame_y, frame_len, 22, 3);
  ui_run(&frame_y, &frame_y_trg, 5, 4);
  ui_run(&frame_len, &frame_len_trg, 10, 5);
  u8g2.sendBuffer();
  // transfer internal memory to the displa
}

void ui_proc() // 总的UI进程
{
  switch (ui_state)
  {
  case S_NONE:
    if (ui_index != M_CHART)
      u8g2.clearBuffer();
    switch (ui_index)
    {
    case M_LOGO:
      logo_proc();
      break;
    case M_SELECT:
      select_proc();
      break;
    case M_PID:
      pid_proc();
      break;
    case M_ICON:
      icon_proc();
      break;
    case M_CHART:
      chart_proc();
      break;
    case M_TEXT_EDIT:
      text_edit_proc();
      break;
    case M_PID_EDIT:
      pid_edit_proc();
      break;
    case M_ABOUT:
      about_proc();
      break;
    default:
      break;
    }
    break;
  case S_DISAPPEAR:
    disappear();
    break;
  default:
    break;
  }
  u8g2.sendBuffer();
}

void select_proc(void) // 选择界面处理
{
  int list_len = sizeof(list) / sizeof(SETTING_LIST);

  if (key_msg.press)
  {
    key_msg.press = 0;
    switch (key_msg.id)
    {
    case 0:
      ui_select--;
      if (ui_select < 0)
      {
        ui_select = 0;
      }
      break;
    case 1:
      ui_select++;
      if (ui_select >= list_len)
      {
        ui_select = list_len - 1;
      }
      break;
    case 2:
      switch (ui_select)
      {
      case 0: // return
        ui_state = S_DISAPPEAR;
        ui_index = M_LOGO;
        break;
      case 1: // pid
        ui_state = S_DISAPPEAR;
        ui_index = M_PID;
        break;
      case 2: // icon
        ui_state = S_DISAPPEAR;
        ui_index = M_ICON;
        break;
      case 3: // chart
        ui_state = S_DISAPPEAR;
        ui_index = M_CHART;
        break;
      case 4: // textedit
        ui_state = S_DISAPPEAR;
        ui_index = M_TEXT_EDIT;
        break;
      case 6: // about
        ui_state = S_DISAPPEAR;
        ui_index = M_ABOUT;
        break;
      default:
        break;
      }
    }
    frame_y_trg = ui_select * 20;
    frame_len_trg = list[ui_select].len * 13;
    ui_show();
  }
  // 手势识别的模块化函数
  void paj_roc(void)
  {
    uint8_t data = 0, data1 = 0, error;
    error = paj7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.
    if (!error)
    {
      switch (data) // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
      {
      case GES_RIGHT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        Serial.println("Right");
        btn2 = 1;
        delay(100);

        break;
      case GES_LEFT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        Serial.println("Left");
        break;
      case GES_UP_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        Serial.println("Up");
        btn0 = 1;
        delay(100);

        break;
      case GES_DOWN_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        Serial.println("Down");
        btn1 = 1;
        delay(100);

        break;
      case GES_FORWARD_FLAG:
        Serial.println("Forward");
        delay(GES_QUIT_TIME);
        break;
      case GES_BACKWARD_FLAG:
        Serial.println("Backward");
        delay(GES_QUIT_TIME);
        break;
      case GES_CLOCKWISE_FLAG:
        Serial.println("Clockwise");
        break;
      case GES_COUNT_CLOCKWISE_FLAG:
        Serial.println("anti-clockwise");
        break;
      default:
        paj7620ReadReg(0x44, 1, &data1);
        if (data1 == GES_WAVE_FLAG)
        {
          Serial.println("wave");
        }
        break;
      }
    }
  }

  // 三级菜单的添加

  void about_ui_show() // about界面
  {
    u8g2.drawStr(2, 12, "MCU : ESP32");
    u8g2.drawStr(2, 28, "FLASH : 4MB");
    u8g2.drawStr(2, 44, "SRAM : 520KB");
    u8g2.drawStr(2, 60, "——kenbio Kes——");
  }
  // 消失函数
  uint8_t disappear_step = 1;

  void disappear()
  {
    switch (disappear_step)
    {
    case 1:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 == 0)
          buf_ptr[i] = buf_ptr[i] & 0x55;
      }
      break;
    case 2:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 != 0)
          buf_ptr[i] = buf_ptr[i] & 0xAA;
      }
      break;
    case 3:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 == 0)
          buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    case 4:
      for (uint16_t i = 0; i < buf_len; ++i)
      {
        if (i % 2 != 0)
          buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    default:
      // ui_state=S_NONE;
      disappear_step = 0;
      break;
    }
    disappear_step++;
  }

  void loop(void)
  {
    key_scan();
    ui_proc();
    paj_roc();
  }