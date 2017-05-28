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
//�Ƿ񲥷���Ч
bool sound = false;
//��굱ǰ��ͼ������
int mouse_id = WALL;
int mouse_x;
int mouse_y;
//��ͼ�ļ����
string map_name("map.txt");
fstream map_file;
//�����ڴ��豸
HDC winDC;
HDC winlogin_DC;
HDC writeDC;
HDC backgroundDC;
HDC background_loginDC;
HDC player_tankDC;
HDC bulletDC;
HDC blockDC;
HDC fireDC;
//����λͼ
HBITMAP writeBMP;
HBITMAP backgroundBMP;
HBITMAP backgroundBMP_login;
HBITMAP player_tankBMP;
HBITMAP bulletBMP;
HBITMAP blockBMP;
HBITMAP fireBMP;
//��Ϸ��ʱ��
UINT game_timer;
//��Ϸ״̬
int game_state;
//BOSSģʽ
bool boss_mode = false;
//���̹��
BaseTank player_tank;
//����ӵ�
list<Bullet*> player_bullet;
//������
list<Fire*> fire;
//��������
list<BaseTank*> enemy_tank;
//�����ڵ�����
list<list<Bullet*>*> enemy_bullet;
//��ͼ����
vector<int> block;
//���õ�ͼ����
vector<int> blockmap;
//L������
bool keyup_L = true;
//�ؿ�
int wintime;
const int wintimemax = 200;
int stage;
const int stage_max = map_max;
//����
int base_time;
const int base_timemax = 100;
//���������
bool player_death = false;
const int player_max = 3;
int player_num = player_max;
int player_time;
const int player_timemax = 100;
//�����������
const int max_life = 40;
const int max_speed = 8;
const int max_fire_speed = 16;
const int max_num = 2;
const int max_bullet_speed = 24;
//��������
int enemy_num;//��ǰ��Ļ�о�����
int enemy_rest;
const int enemy_maxscr = 4;
int enemy_time = 0;    //�жԳ���ʱ���ӳ�
const int enemy_timemax = 100;
int enemy_total = 1;//�о�����
//����
int item;
int item_x;
int item_y;
int invin_time;
const int invin_timemax = 1000;
//ש��ѭ������
const int wall_min = 4;
const int wall_max = 6;
const int wall1_min = wall_min + 100;
const int wall1_max = wall_max + 100;
const int wall_timemax = 5;
int wall_time;
//ˮ��ѭ������
const int water_min = 12;
const int water_max = 23;
const int water1_min = water_min + 100;
const int water1_max = water_max + 100;
const int water_timemax = 3;
int water_time;

bool start_flag = false;
//��Ϸ���ھ��
HWND hwnd;
HINSTANCE appInstance;
HINSTANCE hinst;
//�ؼ����
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
int clientORserver = 0;//1�����������2����ͻ���

LPCTSTR str_password = _T("���룺");
LPCTSTR str_username = _T("�˺ţ�");
HBITMAP Hbmp;
int grade = 0;//�÷�
char username[128];//�ؼ�����û���
char password[128];//�ؼ��������
char nickname[128];//����ǳ�
int  grade_db;
bool Admit_Login = false;//׼���¼
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