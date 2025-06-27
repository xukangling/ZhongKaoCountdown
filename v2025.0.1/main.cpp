/*Write before 
  2025镇江地理生物小中考
  
  2025/6/25补:地理100, 生物100!!!
  
  2026中考必胜! 
  
  编写过程中借助了AI帮助
  */


#include <windows.h>
#include <ctime>
#include <cstdio>
#include <cmath>

// 中考日期默认值 可自行设置
#define TARGET_YEAR 2026
#define TARGET_MON 6
#define TARGET_DAY 17

// 控制台颜色定义
enum ConsoleColor {
    Black       = 0,
    DarkBlue    = 1,
    DarkGreen   = 2,
    DarkCyan    = 3,
    DarkRed     = 4,
    DarkMagenta = 5,
    DarkYellow  = 6,
    Gray        = 7,
    DarkGray    = 8,
    Blue        = 9,
    Green       = 10,
    Cyan        = 11,
    Red         = 12,
    Magenta     = 13,
    Yellow      = 14,
    White       = 15
};

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;

// ASCII数字模板（5行x5列）
const char* bigNumbers[10][5] = {
    {"#####","#   #","#   #","#   #","#####"}, // 0
    {"  #  "," ##  ","  #  ","  #  "," ### "}, // 1
    {"#### ","    #"," ### ","#    ","#####"}, // 2
    {"#### ","    #"," ####","    #","#### "}, // 3
    {"#   #","#   #","#####","    #","    #"}, // 4
    {"#####","#    ","#####","    #","#####"}, // 5
    {"#####","#    ","#####","#   #","#####"}, // 6
    {"#####","   # ","  #  "," #   ","#    "}, // 7
    {"#####","#   #","#####","#   #","#####"}, // 8
    {"#####","#   #","#####","    #","#####"}  // 9
};

// 设置控制台颜色
void SetColor(ConsoleColor text, ConsoleColor background = Black) {
    SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | text));
}

// 获取剩余时间
void GetRemainingTime(time_t& remaining) {
    tm target_tm = {0};
    target_tm.tm_year = TARGET_YEAR - 1900;
    target_tm.tm_mon = TARGET_MON - 1;
    target_tm.tm_mday = TARGET_DAY;
    target_tm.tm_hour = 9;
    mktime(&target_tm);

    time_t now = time(NULL);
    remaining = difftime(mktime(&target_tm), now);
}

// 绘制动态进度条
void DrawProgressBar(int width, float progress) {
    static int wavePos = 0;
    
    SetColor(DarkGray);
    printf("[");
    for(int i = 0; i < width; ++i) {
        float pos = (float)i/width;
        float wave = 0.5f * sin((pos * 20.0f) - wavePos * 0.5f) + 0.5f;
        
        if(pos < progress) {
            SetColor((ConsoleColor)(Green + (int)(wave * 6)));; // 颜色波动效果
            printf("#");
        } else {
            SetColor(DarkGray);
            printf("-");
        }
    }
    SetColor(DarkGray);
    printf("]");
    wavePos++;
}

// 绘制大号数字
void DrawBigNumber(int num, int x, int y) {
    int digits[4] = {num/1000, (num/100)%10, (num/10)%10, num%10};
    for(int row = 0; row < 5; ++row) {
        for(int i = 0; i < 4; ++i) {
            COORD pos = {(SHORT)(x + i*6), (SHORT)(y + row)};
            SetConsoleCursorPosition(hConsole, pos);
            SetColor(Cyan);
            printf("%s", bigNumbers[digits[i]][row]);
        }
    }
}

int main() {
    // 设置控制台编码
    SetConsoleOutputCP(65001);
    
    // 初始化控制台
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // 设置控制台尺寸
    system("mode con cols=80 lines=30");

    while(true) {
        // 检测ESC键
        if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        // 计算剩余时间
        time_t remaining;
        GetRemainingTime(remaining);
        if(remaining <= 0) {
            system("cls");
            SetColor(Red);
            printf("\n\n  >>> 中考已经开始！ <<< \n");
            break;
        }

        int days = remaining / 86400;
        int hours = (remaining % 86400) / 3600;
        int minutes = (remaining % 3600) / 60;
        int seconds = remaining % 60;

        // 清屏
        SetConsoleCursorPosition(hConsole, {0, 0});

        // 绘制标题
        SetColor(Yellow);
        printf("\n  >> 镇江中考倒计时 << \n\n");

        // 显示大号数字
        DrawBigNumber(days, 10, 3);
        DrawBigNumber(hours, 10, 9);
        DrawBigNumber(minutes, 10, 15);
        DrawBigNumber(seconds, 10, 21);

        // 绘制标签
        const char* labels[] = {"天", "时", "分", "秒"};
        for(int i = 0; i < 4; ++i) {
            COORD pos = {45, (SHORT)(6 + i*6)};
            SetConsoleCursorPosition(hConsole, pos);
            SetColor(Magenta);
            printf("%s", labels[i]);
        }

        // 绘制进度条
        COORD barPos = {10, 27};
        SetConsoleCursorPosition(hConsole, barPos);
        float progress = 1 - remaining / (30.0*86400);
        DrawProgressBar(50, progress);

        // 显示详细时间
        COORD timePos = {10, 29};
        SetConsoleCursorPosition(hConsole, timePos);
        SetColor(White);
        printf(" %02d天 %02d时 %02d分 %02d秒 ", 
              days, hours, minutes, seconds);

        Sleep(100);
    }

    // 恢复设置
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    SetColor(White);
    return 0;
}