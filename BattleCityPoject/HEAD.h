#pragma once

#include "tank.h"
#include <stdlib.h>
#include <tchar.h>
#include "MapData.h"
#include <mysql.h>
#include "md5.h"  
#include "database.h"
#include "socket.h"
//MENU  ID
#define Btn_login 520
#define Btn_Web 523
#define Btn_Music 524
#define Btn_About 525
#define Btn_Quit 526
#define Btn_MakePeople 527
#define Btn_Service 528
#define Edt_username 521
#define Edt_password 522
#define Btn_Single 530
#define Btn_Multi 533
#define Btn_Goon 531
#define Btn_Newgame 532
#define Btn_Createroom 540
#define Btn_Joinroom 541
//是否播放音效
bool sound = false;
//鼠标当前地图块属性
int mouse_id = WALL;
int mouse_x;
int mouse_y;
//地图文件句柄
string map_name("map.txt");
fstream map_file;
//兼容内存设备
HDC winDC;
HDC winlogin_DC;
HDC writeDC;
HDC backgroundDC;
HDC background_loginDC;
HDC player_tankDC;
HDC bulletDC;
HDC blockDC;
HDC fireDC;
//兼容位图
HBITMAP writeBMP;
HBITMAP backgroundBMP;
HBITMAP backgroundBMP_login;
HBITMAP player_tankBMP;
HBITMAP bulletBMP;
HBITMAP blockBMP;
HBITMAP fireBMP;
//游戏定时器
UINT game_timer;
//游戏状态
int game_state;
//BOSS模式
bool boss_mode = false;
//玩家坦克
BaseTank player_tank;
//玩家子弹
list<Bullet*> player_bullet;
//火花容器
list<Fire*> fire;
//敌人容器
list<BaseTank*> enemy_tank;
//敌人炮弹容器
list<list<Bullet*>*> enemy_bullet;
//地图容器
vector<int> block;
//备用地图容器
vector<int> blockmap;
//L键按下
bool keyup_L = true;
//关卡
int wintime;
const int wintimemax = 200;
int stage;
const int stage_max = map_max;
//基地
int base_time;
const int base_timemax = 100;
//玩家生命数
bool player_death = false;
const int player_max = 3;
int player_num = player_max;
int player_time;
const int player_timemax = 100;
//玩家属性上限
const int max_life = 40;
const int max_speed = 8;
const int max_fire_speed = 16;
const int max_num = 2;
const int max_bullet_speed = 24;
//敌人数量
int enemy_num;//当前屏幕敌军数量
int enemy_rest;
const int enemy_maxscr = 4;
int enemy_time = 0;    //敌对出现时间延迟
const int enemy_timemax = 100;
int enemy_total = 1;//敌军总数
//道具
int item;
int item_x;
int item_y;
int invin_time;
const int invin_timemax = 1000;
//砖块循环数据
const int wall_min = 4;
const int wall_max = 6;
const int wall1_min = wall_min + 100;
const int wall1_max = wall_max + 100;
const int wall_timemax = 5;
int wall_time;
//水块循环数据
const int water_min = 12;
const int water_max = 23;
const int water1_min = water_min + 100;
const int water1_max = water_max + 100;
const int water_timemax = 3;
int water_time;

bool start_flag = false;
//游戏窗口句柄
HWND hwnd;
HINSTANCE appInstance;
HINSTANCE hinst;
//控件句柄
HWND hButton_login = NULL;
HWND hButton_Web = NULL;
HWND hButton_Music = NULL;
HWND hButton_About = NULL;
HWND hButton_MakePeople = NULL;
HWND hButton_Service = NULL;
HWND hButton_Quit = NULL;
HWND hEdit_username = NULL;
HWND hEdit_password = NULL;
HWND hLabUsername = NULL;
HWND hButton_Single = NULL;
HWND hButton_Multi = NULL;
HWND hButton_Goon = NULL;
HWND hButton_Newgame = NULL;
HWND hButton_Createroom = NULL;
HWND hButton_Joinroon = NULL;
int clientORserver = 0;//1代表服务器，2代表客户端

LPCTSTR str_password = _T("密码：");
LPCTSTR str_username = _T("账号：");
HBITMAP Hbmp;
int grade = 0;//得分
char username[128];//控件获得用户名
char password[128];//控件获得密码
char nickname[128];//玩家昵称
int  grade_db;
bool Admit_Login = false;//准许登录
wchar_t *wchar = NULL;
wchar_t *chr = NULL;
bool first = true;
void charTowchar(const char *chr, wchar_t *wchar, int size)
{
	MultiByteToWideChar(CP_ACP, 0, chr,
		(int)strlen(chr) + 1, wchar, size / sizeof(wchar[0]));
}
void wcharTochar(const wchar_t *wchar, char *chr, int length)
{
	WideCharToMultiByte(CP_ACP, 0, wchar, -1,
		chr, length, NULL, NULL);
}