//������һ��
#pragma once
//��ý���
#pragma comment(lib, "winmm")
//API
#include <Windows.h>
//ϵͳʱ��
#include <time.h>
//�ַ�����
#include <string>
//�ļ���
#include <fstream>
//˳������
#include <vector>
#include <list>
//��Դͷ�ļ�
#include "resource.h"
//̹����
#include "BaseTank.h"
//�ӵ���
#include "Bullet.h"
//����
#include "Fire.h"
//�����ռ�
using namespace std;

//��Ϸ���ڿ�͸�
#define GAME_W  960
#define GAME_H  704
#define TANK_WIDE 64
#define TANK_HIGH 64
#define BULLET_WIDE 25
#define BULLET_HIGH 40
//��Ϸ��ʱ��
#define GAME_ID 1
#define GAME_TIME 10
//����ӳ��
#define KEYDOWN(key) GetAsyncKeyState(key)&0x8000
#define KEYUP(key) !(GetAsyncKeyState(key)&0x8000)
//������Ч
#define PLAYA(MUSIC) PlaySound(MAKEINTRESOURCE(MUSIC),GetModuleHandle(NULL),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT)
#define PLAYB(MUSIC) PlaySound(MAKEINTRESOURCE(MUSIC),GetModuleHandle(NULL),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT|SND_NOSTOP)
//��ͼ��
#define FLOOR 0
#define WALL 1
#define WALLMAX 6
#define STONE 7
#define GRASS 8
#define ICE 9
#define BASE 10
#define SAND 11
#define WATER 12
#define WATERMAX 23
//��Ϸ״̬
#define GAME 0
#define PAUSE 1
#define GAMEOVER 2
#define WIN 3
#define NEXTSTAGE 4
#define EDIT 5
//����̹������
#define GREEN 4
#define BLUE 5
#define RED 6
#define GOLD 7
//��������
#define NONE 4
#define LIFE 0
#define POWER 1
#define SPEED 2
#define WEAPON 3
//���̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��Ϸ��ʼ
void Start();
//��Ϸѭ��
void Game();
//�����ж�
void Key();
//������
void DrawBG();
//����Ϸ
void DrawGame();
//���������
void Print();
//����ͼ��
void DrawBlock();
//����ͻ�����ӵ�
void PlayerBullet();
//����
void DrawGrass();
//����ը��
void DrawFire();
//��ʼ��
void Init();
//�ӵ�����ש��
bool BulletHitBlock(Bullet&);
//̹������ש��
bool TankHitBlock(BaseTank&);
//�ӵ�����̹��
bool BulletHitTank(Bullet&,BaseTank&);
//̹������̹��
bool TankHitTank(BaseTank&,BaseTank&);
//�ӵ������ӵ�
bool BulletHitBullet(Bullet&,Bullet&);
//����ͻ�����̹��
void EnemyTank();
//����ͻ������ӵ�
void EnemyBullet();
//����ͻ�����
void Item();
//��ͼ�༭
void Edit();
//��ͼ����
void Save();
//��ͼ����
void Load();
//����Ԥ�õ�ͼ
void LoadMap();
//ȫ��ͼ��ʼ��
void MapInit();

