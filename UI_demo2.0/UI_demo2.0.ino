
#include <U8g2lib.h>
#include <Wire.h>
#include "paj7620.h"

// #define ESP32 0

#define SPEED 4 // 16的因数
#define ICON_SPEED 12
#define ICON_SPACE 48 // 图标间隔，speed倍数

// 手势识别模块需要的一些定义
#define GES_REACTION_TIME 100 // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME 100    // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_QUIT_TIME 0
uint8_t btn_up;
uint8_t btn_down;
uint8_t btn_enter;
uint8_t btn_return;                                                             // 手势识别中用于返回的变量
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/22, /* data=*/21); // ESP32 Thing, pure SW emulated I2C

// main界面图片
PROGMEM const uint8_t LOGO[] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x01, 0x24, 0x00, 0x44, 0x04, 0x80, 0x20, 0x00, 0x00, 0x10, 0xF2, 0x3C, 0x20, 0xE0, 0x00,
        0x10, 0x01, 0x24, 0x00, 0xC4, 0x04, 0x80, 0x20, 0x00, 0x00, 0x10, 0x11, 0x24, 0x30, 0x10, 0x01,
        0x10, 0x71, 0x24, 0x0F, 0xC4, 0xE2, 0x99, 0x3C, 0x00, 0x00, 0x20, 0x11, 0x24, 0x20, 0x10, 0x01,
        0xF0, 0x89, 0xA4, 0x10, 0xA8, 0x12, 0x8A, 0x22, 0x00, 0x00, 0x20, 0xF1, 0x1C, 0x20, 0x10, 0x01,
        0x10, 0xF9, 0xA4, 0x10, 0x98, 0x13, 0x8A, 0x22, 0x00, 0x00, 0xA0, 0x10, 0x24, 0x20, 0x10, 0x01,
        0x10, 0x09, 0xA4, 0x10, 0x98, 0x11, 0x8A, 0x22, 0x00, 0x00, 0xA0, 0x10, 0x24, 0x20, 0x10, 0x01,
        0x10, 0xF1, 0x24, 0x0F, 0x10, 0xE1, 0x89, 0x3C, 0x00, 0x00, 0x40, 0xF0, 0x44, 0x20, 0xE2, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xE7, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x0F, 0xFE, 0x99, 0xFF, 0xE4, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xE7, 0xFD, 0xBD, 0xFF, 0xDE, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFE,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0x1F, 0xF0, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0x1F, 0xFE, 0xFF, 0x83, 0xFF, 0xE3, 0xF1,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0xE3, 0xF1, 0xFF, 0x7C, 0xF8, 0xF9, 0xEF,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0xFD, 0xEF, 0x3F, 0xFF, 0xF3, 0xFD, 0xDF,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0x1E, 0xDC, 0xBF, 0x03, 0xF7, 0xE3, 0xE1,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xEF, 0xB9, 0xDF, 0xFB, 0xF0, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xE7, 0xBB, 0xDF, 0xF3, 0xFF, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xF7, 0x7B, 0xDF, 0x0F, 0xFF, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0x0F, 0x7C, 0xBF, 0xFF, 0xFC, 0xEF, 0xFD,
        0xF7, 0xFD, 0x7D, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xFF, 0xBF, 0x7F, 0xFE, 0xF3, 0xEF, 0xFD,
        0xF7, 0xFD, 0xBE, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0x07, 0xC0, 0xFF, 0xE1, 0xF7, 0xEF, 0xFD,
        0xEF, 0xFB, 0xBE, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xBF, 0xEF, 0xFF, 0xFF, 0x9F, 0xEF, 0xEF, 0xFD,
        0xEF, 0x07, 0xBF, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0x7F, 0xEF, 0xC7, 0x3F, 0x3E, 0xEF, 0xEF, 0xFD,
        0xDF, 0xFF, 0xDF, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0x1E, 0xB8, 0xDF, 0xB9, 0xF7, 0xEF, 0xE3,
        0x9F, 0xFF, 0xEF, 0x7F, 0xDF, 0xFF, 0xF7, 0xFD, 0xFF, 0xFC, 0xBF, 0x9F, 0xC7, 0xF7, 0xDF, 0xCF,
        0x7F, 0xFF, 0xF3, 0xFF, 0xDE, 0xFF, 0xEF, 0xFD, 0xFF, 0xFB, 0xCF, 0x3F, 0xFF, 0xF9, 0x9F, 0xEF,
        0xFF, 0x00, 0xFC, 0xFF, 0xE1, 0xFF, 0x1F, 0xFE, 0xFF, 0x07, 0xF0, 0xFF, 0x00, 0xFE, 0x7F, 0xF0,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x40, 0x00, 0x02, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x60, 0x00, 0x02, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x88, 0x9C, 0x24, 0xC7, 0x1C, 0x0F, 0x68, 0x22, 0xA0, 0xCC, 0x23, 0x91, 0xC7, 0x01,
        0x00, 0xFE, 0x88, 0xA2, 0xAA, 0x48, 0xA2, 0x08, 0x98, 0x14, 0xB0, 0x24, 0x22, 0x91, 0x24, 0x02,
        0x00, 0x00, 0x78, 0xA2, 0xAB, 0x4F, 0xBE, 0x08, 0x88, 0x14, 0xF0, 0x24, 0x22, 0x91, 0x24, 0x02,
        0x00, 0x00, 0x08, 0x22, 0x9B, 0x40, 0x82, 0x08, 0x88, 0x0C, 0x08, 0x25, 0x22, 0x91, 0x24, 0x02,
        0x00, 0x00, 0x08, 0x1C, 0x11, 0x4F, 0x3C, 0x0F, 0x78, 0x08, 0x08, 0xC5, 0xE3, 0x91, 0xC4, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t disappear_step = 1;

// 实时坐标
uint8_t chart_x;
bool frame_is_drawed = false;

// 指向buf首地址的指针
uint8_t *buf_ptr;
uint16_t buf_len;

// 选择界面变量
uint8_t x;
int16_t y, y_trg;                 // 目标和当前
uint8_t line_y, line_y_trg;       // 线的位置
uint8_t box_width, box_width_trg; // 框的宽度
int16_t box_y, box_y_trg;         // 框的当前值和目标值
int8_t ui_select;                 // 当前选中那一栏

// 状态机变量
uint8_t ui_index, ui_state;

enum // ui_index
{
    M_LOGO,      // 开始界面
    M_SELECT,    // 选择界面
    M_ANIMEMODE, // 播放表情界面
    M_MOVEMODE,  // 移动操作界面
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

// 菜单结构体
typedef struct
{
    char *select;
} SELECT_LIST;

// ——————————————————————三级菜单的示例——————————————————————————————
//  SELECT_LIST pid[]{
//      {"-Proportion"},
//      {"-Integral"},
//      {"-Derivative"},
//      {"Return"},
//  };
//  uint8_t pid_num = sizeof(pid) / sizeof(SELECT_LIST); // PID选项数量

SELECT_LIST list[]{
    {"MainUI"},
    // {"+PID Editor"},
    {"-anime mode"},
    {"-move mode"},
    {"-about"},
};

uint8_t list_num = sizeof(list) / sizeof(SELECT_LIST); // 选择界面数量
uint8_t single_line_length = 63 / list_num;
uint8_t total_line_length = single_line_length * list_num + 1;

// 按键变量
typedef struct
{
    bool val;
    bool last_val;
} KEY;

KEY key[4] = {false};

// 按键信息

typedef struct
{
    uint8_t id;
    bool pressed;
} KEY_MSG;

KEY_MSG key_msg = {0};

bool get_key_val(uint8_t ch)
{
    uint8_t temp0;
    switch (ch)
    {
    case 0:
        temp0 = btn_up;
        btn_up = 0;
        return temp0;
        break;
    case 1:
        temp0 = btn_down;
        btn_down = 0;
        return temp0;
        break;
    case 2:
        temp0 = btn_enter;
        btn_enter = 0;
        return temp0;
    case 3:
        temp0 = btn_return;
        btn_return = 0;
        return temp0;
    default:
        break;
    }
}

void key_init()
{
    for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i)
    {
        key[i].val = key[i].last_val = get_key_val(i);
    }
}

void key_scan()
{
    for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i)
    {
        key[i].val = get_key_val(i);       // 获取键值
        if (key[i].last_val != key[i].val) // 发生改变
        {
            key[i].last_val = key[i].val; // 更新状态
            if (key[i].val == LOW)
            {
                key_msg.id = i;
                key_msg.pressed = true;
            }
        }
    }
}

// 移动函数
bool move(int16_t *a, int16_t *a_trg)
{
    if (*a < *a_trg)
    {
        *a += SPEED;
        if (*a > *a_trg)
            *a = *a_trg; // 加完超过
    }
    else if (*a > *a_trg)
    {
        *a -= SPEED;
        if (*a < *a_trg)
            *a = *a_trg; // 减完不足
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 移动函数
bool move_icon(int16_t *a, int16_t *a_trg)
{
    if (*a < *a_trg)
    {
        *a += ICON_SPEED;
        if (*a > *a_trg)
            *a = *a_trg; // 加完超过
    }
    else if (*a > *a_trg)
    {
        *a -= ICON_SPEED;
        if (*a < *a_trg)
            *a = *a_trg; // 减完不足
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 宽度移动函数
bool move_width(uint8_t *a, uint8_t *a_trg, uint8_t select, uint8_t id)
{
    if (*a < *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t len;
        if (ui_index == M_SELECT)
        {
            len = abs(u8g2.getStrWidth(list[select].select) - u8g2.getStrWidth(list[id == 0 ? select + 1 : select - 1].select));
        }

        uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1)); // 计算步长
        *a += width_speed;
        if (*a > *a_trg)
            *a = *a_trg; //
    }
    else if (*a > *a_trg)
    {
        uint8_t step = 16 / SPEED; // 判断步数
        uint8_t len;
        if (ui_index == M_SELECT)
        {
            len = abs(u8g2.getStrWidth(list[select].select) - u8g2.getStrWidth(list[id == 0 ? select + 1 : select - 1].select));
        }

        uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1)); // 计算步长
        *a -= width_speed;
        if (*a < *a_trg)
            *a = *a_trg;
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 进度条移动函数
bool move_bar(uint8_t *a, uint8_t *a_trg)
{
    if (*a < *a_trg)
    {
        uint8_t step = 16 / SPEED;                                                                                                // 判断步数
        uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step) : (single_line_length / step + 1)); // 计算步长
        *a += width_speed;
        if (*a > *a_trg)
            *a = *a_trg; //
    }
    else if (*a > *a_trg)
    {
        uint8_t step = 16 / SPEED;                                                                                                // 判断步数
        uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step) : (single_line_length / step + 1)); // 计算步长
        *a -= width_speed;
        if (*a < *a_trg)
            *a = *a_trg;
    }
    else
    {
        return true; // 到达目标
    }
    return false; // 未到达
}

// 消失函数
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
        ui_state = S_NONE;
        disappear_step = 0;
        break;
    }
    disappear_step++;
}

/**************************手势识别*******************************/

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
            btn_enter = 1;
            delay(100);

            break;
        case GES_LEFT_FLAG:
            delay(GES_ENTRY_TIME);
            paj7620ReadReg(0x43, 1, &data);
            Serial.println("Left");
            btn_return = 1;
            break;
        case GES_UP_FLAG:
            delay(GES_ENTRY_TIME);
            paj7620ReadReg(0x43, 1, &data);
            Serial.println("Up");
            btn_up = 1;
            delay(100);

            break;
        case GES_DOWN_FLAG:
            delay(GES_ENTRY_TIME);
            paj7620ReadReg(0x43, 1, &data);
            Serial.println("Down");
            btn_down = 1;
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

/**************************界面显示*******************************/

void logo_ui_show() // 显示logo
{
    u8g2.drawXBMP(0, 0, 128, 64, LOGO);

    // for (uint16_t i = 0; i < buf_len; ++i)
    // {
    //     if (i % 4 == 0 || i % 4 == 1)
    //     {
    //         buf_ptr[i] = buf_ptr[i] & 0x33;
    //     }
    //     else
    //     {
    //         buf_ptr[i] = buf_ptr[i] & 0xCC;
    //     }
    // }
}

void select_ui_show() // 选择界面
{
    move_bar(&line_y, &line_y_trg);
    move(&y, &y_trg);
    move(&box_y, &box_y_trg);
    move_width(&box_width, &box_width_trg, ui_select, key_msg.id);
    u8g2.drawVLine(126, 0, total_line_length);
    u8g2.drawPixel(125, 0);
    u8g2.drawPixel(127, 0);
    for (uint8_t i = 0; i < list_num; ++i)
    {
        u8g2.drawStr(x, 16 * i + y + 12, list[i].select); // 第一段输出位置
        u8g2.drawPixel(125, single_line_length * (i + 1));
        u8g2.drawPixel(127, single_line_length * (i + 1));
    }
    u8g2.drawVLine(125, line_y, single_line_length - 1);
    u8g2.drawVLine(127, line_y, single_line_length - 1);
    u8g2.setDrawColor(2);
    u8g2.drawRBox(0, box_y, box_width, 16, 1);
    u8g2.setDrawColor(1);
}

void about_ui_show() // about界面
{
    u8g2.drawStr(2, 12, "control : ESP32");
    u8g2.drawStr(2, 28, "name : SoidaBot ");
    u8g2.drawStr(2, 44, "time : 2023.7.4");
    u8g2.drawStr(2, 60, "*******kenbio*******");
}

void animemode_ui_show() // animemode界面
{
    u8g2.drawStr(2, 28, "this is waifu++! ");
}

void movemode_ui_show() // movemode界面
{

    u8g2.drawStr(2, 28, "I`m moving !Sir! ");
    u8g2.drawStr(2, 44, "我正在移动！先生！");
    u8g2.drawStr(2, 60, "I`m moving !Sir! ");
}
/**************************界面处理*******************************/

void logo_proc() // logo界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    logo_ui_show();
}

void select_proc(void) // 选择界面处理
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        switch (key_msg.id)
        {
        case 0:
            if (ui_select < 1)
                break;
            ui_select -= 1;
            line_y_trg -= single_line_length;
            if (ui_select < -(y / 16))
            {
                y_trg += 16;
            }
            else
            {
                box_y_trg -= 16;
            }

            break;
        case 1:
            if ((ui_select + 2) > (sizeof(list) / sizeof(SELECT_LIST)))
                break;
            ui_select += 1;
            line_y_trg += single_line_length;
            if ((ui_select + 1) > (4 - y / 16))
            {
                y_trg -= 16;
            }
            else
            {
                box_y_trg += 16;
            }

            break;
        case 2:
            switch (ui_select)
            {
            case 0: // return
                ui_state = S_DISAPPEAR;
                ui_index = M_LOGO;
                break;
            case 1: // animemode
                ui_state = S_DISAPPEAR;
                ui_index = M_ANIMEMODE;
                break;
            case 2: // movemode
                ui_state = S_DISAPPEAR;
                ui_index = M_MOVEMODE;
                break;
            case 3: // about
                ui_state = S_DISAPPEAR;
                ui_index = M_ABOUT;
                break;
            default:
                break;
            }
            break;
        case 3:
            if (ui_index == M_ANIMEMODE || ui_index == M_ABOUT || ui_index == M_MOVEMODE)
                ui_index = M_SELECT;
            else
                ui_index = M_LOGO;

            break;

        default:
            break;
        }
        box_width_trg = u8g2.getStrWidth(list[ui_select].select) + x * 2;
    }
    select_ui_show();
}

void about_proc() // about界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    about_ui_show();
}
void animemode_proc() // animemode界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    animemode_ui_show();
}
void movemode_proc() // movemode界面处理函数
{
    if (key_msg.pressed)
    {
        key_msg.pressed = false;
        ui_state = S_DISAPPEAR;
        ui_index = M_SELECT;
    }
    movemode_ui_show();
}
/********************************总的UI显示************************************/

void ui_proc() // 总的UI进程
{
    switch (ui_state)
    {
    case S_NONE:
        u8g2.clearBuffer();
        switch (ui_index)
        {
        case M_LOGO:
            logo_proc();
            break;
        case M_SELECT:
            select_proc();
            break;
        case M_ANIMEMODE:
            animemode_proc();
            break;
        case M_MOVEMODE:
            movemode_proc();
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

void setup()
{
    Serial.begin(9600);
    key_init();
    u8g2.setBusClock(800000);
    u8g2.begin();
    u8g2.setFont(u8g2_font_wqy12_t_chinese1);
    // u8g2.setContrast(10);

    buf_ptr = u8g2.getBufferPtr(); // 拿到buffer首地址
    buf_len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();

    x = 4;
    y = y_trg = 0;
    line_y = line_y_trg = 1;
    ui_select = 0;

    box_width = box_width_trg = u8g2.getStrWidth(list[ui_select].select) + x * 2; // 两边各多2

    ui_index = M_LOGO;
    ui_state = S_NONE;
}

void loop()
{
    key_scan();
    ui_proc();
    paj_roc();
}