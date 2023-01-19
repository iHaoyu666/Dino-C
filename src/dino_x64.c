#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#define maphigh 29 // 游戏画面尺寸
#define mapwide 100
#define Jumphigh 10      // 跳跃高度
HANDLE hOutput, hOutBuf; // 控制台屏幕缓冲区句柄
HANDLE *houtpoint;
COORD coord = {1, 0};
DWORD bytes = 0;
HANDLE consoleHandle;
bool switchover = true;
long int score = 0;
int jumpspeed = 4, fallspeed = 1, runspeed = 1;
int p = 1, q = 1, flag = 0, flag1 = 0, m = 0, jump_flag = 0, fall = 0, s_count = 0, downflag = 0, unstop_flag = 0; 
int _hOutput = 0;                                                                                                 
char temp;
char map[maphigh + 10][mapwide + 10] = {};
void double_buff_init(void)
{
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_WRITE,    // 定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE, // 定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
    hOutput = CreateConsoleScreenBuffer(
        GENERIC_WRITE,    // 定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE, // 定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
    SMALL_RECT rc = {0, 0, mapwide + 10, maphigh + 4}; // 设置窗口大小，宽度和高度
    SetConsoleWindowInfo(hOutput, 1, &rc);
    SetConsoleWindowInfo(hOutBuf, 1, &rc);
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);
}
void gotoxy(int x, int y)
{
    COORD co = (COORD){.X = x, .Y = y};
    SetConsoleCursorPosition(houtpoint, co); // 设置光标位置
}
void dinoprint(int i) // i 为跳起的高度 max = 3 直腿恐龙
{
    SetConsoleOutputCP(437);
    char dino[8][12] = {
        {32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32},
        {32, 32, 32, 32, 206, 181, 32, 64, 32, 32, 32, 179},
        {32, 179, 32, 32, 206, 181, 32, 32, 32, 200, 205, 181},
        {47, 179, 32, 32, 206, 181, 32, 32, 32, 218, 196, 217},
        {92, 192, 208, 208, 208, 217, 32, 32, 32, 198, 203, 187},
        {32, 92, 95, 95, 95, 95, 95, 95, 47, 32, 32, 32},
        {32, 32, 32, 32, 179, 32, 32, 179, 32, 32, 32, 32},
        {32, 32, 32, 32, 207, 205, 32, 207, 205, 32, 32, 32}};
    for (int j = 0; j < 12; j++)
    {
        for (int m = 0; m < 8; m++)
            map[maphigh - 8 + m - i][j] = dino[m][j];
    }
    return;
}

void updatemap() // 更新背景 (树 鸟）
{
    map[maphigh - 2][mapwide - p] = '|';
    map[maphigh - 1][mapwide - p] = '|';
    map[maphigh - 2][mapwide - p - 1] = '/';
    map[maphigh - 2][mapwide - p + 1] = '\\';
    p += 3;
    p = p % mapwide;
    if (p >= (mapwide / 2) || flag == 1)
    {
        map[maphigh - 8][mapwide - q] = '<';
        map[maphigh - 8][mapwide - q + 1] = '@';
        map[maphigh - 8][mapwide - q + 2] = '[';
        map[maphigh - 8][mapwide - q + 3] = ']';
        map[maphigh - 8][mapwide - q + 4] = '<';
        map[maphigh - 7][mapwide - q + 2] = '#';
        map[maphigh - 9][mapwide - q + 2] = '#';
        q += 3;
        flag = 1;
        if (q >= mapwide)
            flag = 0;
        q = q % mapwide;
    }
}
void jump()
{
    dinoprint(m);
    if (m >= Jumphigh || fall == 1)
    {
        fall = 1;
        m = m - fallspeed;
        fallspeed++;
        if (m <= -1)
        {
            jump_flag = 0;
            fall = 0;
            m = 0;
            fallspeed = 1;
            jumpspeed = 4;
        }
        return;
    }
    if (m >= 0 && m < Jumphigh)
    {
        m = m + jumpspeed;
        jumpspeed--;
    }
}

void drawmap()
{
    for (int i = 1; i <= mapwide; i++)
    {

        map[maphigh][i] = '_';
    }
}
void dinoprint1() // 抬右脚
{
    SetConsoleOutputCP(437);
    char dino[8][12] = {
        {32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32},
        {32, 32, 32, 32, 206, 181, 32, 64, 32, 32, 32, 179},
        {32, 179, 32, 32, 206, 181, 32, 32, 32, 200, 205, 181},
        {47, 179, 32, 32, 206, 181, 32, 32, 32, 218, 196, 217},
        {92, 192, 208, 208, 208, 217, 32, 32, 32, 198, 203, 187},
        {32, 92, 95, 95, 95, 95, 95, 95, 47, 32, 32, 32},
        {32, 32, 32, 32, 179, 32, 32, 207, 205, 32, 32, 32},
        {32, 32, 32, 32, 207, 205, 32, 32, 32, 32, 32, 32}};
    for (int j = 0; j < 12; j++)
    {
        for (int m = 0; m < 8; m++)
            map[maphigh - 8 + m][j] = dino[m][j];
    }
    return;
}
void dinoprint2() // 抬左脚
{
    SetConsoleOutputCP(437);
    char dino[8][12] = {
        {32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32},
        {32, 32, 32, 32, 206, 181, 32, 64, 32, 32, 32, 179},
        {32, 179, 32, 32, 206, 181, 32, 32, 32, 200, 205, 181},
        {47, 179, 32, 32, 206, 181, 32, 32, 32, 218, 196, 217},
        {92, 192, 208, 208, 208, 217, 32, 32, 32, 198, 203, 187},
        {32, 92, 95, 95, 95, 95, 95, 95, 47, 32, 32, 32},
        {32, 32, 32, 32, 207, 205, 32, 179, 32, 32, 32, 32},
        {32, 32, 32, 32, 32, 32, 32, 207, 205, 32, 32, 32}};
    for (int j = 0; j < 12; j++)
    {
        for (int m = 0; m < 8; m++)
            map[maphigh - 8 + m][j] = dino[m][j];
    }
    return;
}
void dinoprint3() // 下蹲恐龙
{
    SetConsoleOutputCP(437);
    char dino[8][12] = {
        {32, 32, 32, 32, 32, 32, 95, 95, 95, 95, 95, 32},
        {32, 32, 32, 32, 206, 181, 32, 64, 32, 32, 32, 179},
        {32, 179, 32, 32, 206, 181, 32, 32, 32, 200, 205, 181},
        {47, 179, 32, 32, 206, 181, 32, 32, 32, 218, 196, 217},
        {92, 192, 208, 208, 208, 217, 32, 32, 32, 198, 203, 187},
        {32, 92, 95, 95, 95, 95, 95, 95, 47, 32, 32, 32},
    };
    for (int j = 0; j < 12; j++)
    {
        for (int m = 0; m < 8; m++)
            map[maphigh - 6 + m][j] = dino[m][j];
    }

    return;
}
void keydown()
{
    if (GetKeyState(32) < 0)
    {
        while (getch() != 'p')
        {
        }
    }
    if (jump_flag == 1 && (GetKeyState(83) >= 0))
    {
        jump();
        s_count = 0;
        downflag = 0;
        return;
    }
    if (GetKeyState(87) < 0)
    {
        jump_flag = 1;
        s_count = 0;
        downflag = 0;
    }
    else if (GetKeyState(83) < 0)
    {

        dinoprint3();
        downflag = 1;

        jump_flag = 0;
        m = 0;
        fallspeed = 1;
        jumpspeed = 4;
        return;
    }

    if (jump_flag != 1) // 正常跑动恐龙
    {
        if (p % 2 == 1)
            dinoprint1();
        else
            dinoprint2();

        s_count = 0;
        downflag = 0;
    }
    if (jump_flag == 1)
    {
        jump();
        s_count = 0;
        downflag = 0;
        return;
    }

    return;
}
bool check()
{
    int tree = mapwide - p;
    int bird = mapwide - q;
    if ((tree >= 3 && tree <= 9 && m <= 3) || (bird >= 4 && bird < 12 && downflag != 1))
        return false;
    return true;
}
void reset() // 重置 重新开始游戏
{
    p = 1;
    q = 1;
    flag = 0;
    score = 0;
    fallspeed = 1;
    jumpspeed = 4;
    m = 0;
}
void clear() // 清屏
{
    if (check())
        for (int i = 0; i <= maphigh + 3; i++)
            for (int j = 0; j < (mapwide + 2); j++)
            {
                map[i][j] = ' ';
            }
    else
        for (int j = 0; j < (mapwide + 2); j++)
        {
            map[maphigh - 1][j] = ' ';
            map[maphigh - 2][j] = ' ';
            map[maphigh - 8][j] = ' ';
            map[maphigh - 8][j] = ' ';
            map[maphigh - 9][j] = ' ';
            map[maphigh - 10][j] = ' ';
            map[maphigh - 11][j] = ' ';
            map[maphigh - 12][j] = ' ';
            map[maphigh - 13][j] = ' ';
        }

    return;
}
void updatescore()
{
    char str[] = {' '};
    map[4][mapwide - 15] = 'Y';
    map[4][mapwide - 14] = 'o';
    map[4][mapwide - 13] = 'u';
    map[4][mapwide - 12] = 'r';
    map[4][mapwide - 10] = 'S';
    map[4][mapwide - 9] = 'c';
    map[4][mapwide - 8] = 'o';
    map[4][mapwide - 7] = 'r';
    map[4][mapwide - 6] = 'e';
    map[4][mapwide - 5] = ':';
    long int m;
    m = score;
    ltoa(m, str, 10);
    map[4][mapwide - 4] = str[0];
    map[4][mapwide - 3] = str[1];
    map[4][mapwide - 2] = str[2];
    map[4][mapwide - 1] = str[3];
    map[4][mapwide] = str[4];
    map[4][mapwide + 1] = str[5];
}
void draw()
{
    clear();
    updatemap(); // 画障碍物
    updatescore();
    keydown();
    drawmap();
    _hOutput = !_hOutput;
    if (!_hOutput)
    {
        houtpoint = &hOutput;
    }
    else
    {
        houtpoint = &hOutBuf;
    }
    if (switchover == true)
    {
        for (int i = 0; i < maphigh + 2; i++)
        {
            coord.Y = i;
            WriteConsoleOutputCharacterA(hOutBuf, map[i], mapwide, coord, &bytes);
        }
        // 设置新的缓冲区为活动显示缓冲
        SetConsoleActiveScreenBuffer(hOutBuf);
    }
    else
    {
        for (int i = 0; i < maphigh + 2; i++)
        {
            coord.Y = i;
            WriteConsoleOutputCharacterA(hOutput, map[i], mapwide, coord, &bytes);
        }
        // 设置新的缓冲区为活动显示缓冲
        SetConsoleActiveScreenBuffer(hOutput);
    }
    if (score < 100)
        Sleep(90);
    else if (score < 200)
        Sleep(80);
    else if (score < 300)
        Sleep(70);
    else if (score < 500)
        Sleep(60);
    else
        Sleep(50);
    switchover = !switchover;
}
void gameover()
{
    map[maphigh / 2][mapwide / 2] = 'G';
    map[maphigh / 2][mapwide / 2 + 1] = 'A';
    map[maphigh / 2][mapwide / 2 + 2] = 'M';
    map[maphigh / 2][mapwide / 2 + 3] = 'E';
    map[maphigh / 2][mapwide / 2 + 5] = 'O';
    map[maphigh / 2][mapwide / 2 + 6] = 'V';
    map[maphigh / 2][mapwide / 2 + 7] = 'E';
    map[maphigh / 2][mapwide / 2 + 8] = 'R';
    map[maphigh / 2][mapwide / 2 + 9] = '!';
    map[maphigh / 2][mapwide / 2 + 10] = '!';
    map[maphigh / 2 + 1][mapwide / 2 + 1] = 'R';
    map[maphigh / 2 + 1][mapwide / 2 + 2] = 'e';
    map[maphigh / 2 + 1][mapwide / 2 + 3] = 'p';
    map[maphigh / 2 + 1][mapwide / 2 + 4] = 'l';
    map[maphigh / 2 + 1][mapwide / 2 + 5] = 'a';
    map[maphigh / 2 + 1][mapwide / 2 + 6] = 'y';
    map[maphigh / 2 + 1][mapwide / 2 + 7] = ':';
    map[maphigh / 2 + 1][mapwide / 2 + 8] = 'R';
    map[maphigh / 2 + 1][mapwide / 2 + 9] = '~';
    map[maphigh / 2 + 1][mapwide / 2 + 10] = '~';
    draw();
}
int main()
{

    while (1)
    {
        double_buff_init();
        draw();
        if (!check()) // 碰撞检测
        {
            gameover();
            Sleep(3000);
            if (GetKeyState(82) < 0)
            {
                reset();
                continue;
            }
            break;
        }
        score++;
    }
}