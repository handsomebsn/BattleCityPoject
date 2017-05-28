//仅编译一次
#pragma once
//多媒体库
#pragma comment(lib, "winmm")
//API
#include <Windows.h>
//系统时间
#include <time.h>
//字符串库
#include <string>
//文件流
#include <fstream>
//顺序容器
#include <vector>
#include <list>
//资源头文件
#include "resource.h"
//坦克类
#include "BaseTank.h"
//子弹类
#include "Bullet.h"
//火花类
#include "Fire.h"
//命名空间
using namespace std;

//游戏窗口宽和高
#define GAME_W  960
#define GAME_H  704
#define TANK_WIDE 64
#define TANK_HIGH 64
#define BULLET_WIDE 25
#define BULLET_HIGH 40
//游戏定时器
#define GAME_ID 1
#define GAME_TIME 10
//按键映射
#define KEYDOWN(key) GetAsyncKeyState(key)&0x8000
#define KEYUP(key) !(GetAsyncKeyState(key)&0x8000)
//播放音效
#define PLAYA(MUSIC) PlaySound(MAKEINTRESOURCE(MUSIC),GetModuleHandle(NULL),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT)
#define PLAYB(MUSIC) PlaySound(MAKEINTRESOURCE(MUSIC),GetModuleHandle(NULL),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT|SND_NOSTOP)
//地图块
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
//游戏状态
#define GAME 0
#define PAUSE 1
#define GAMEOVER 2
#define WIN 3
#define NEXTSTAGE 4
#define EDIT 5
//敌人坦克种类
#define GREEN 4
#define BLUE 5
#define RED 6
#define GOLD 7
//道具种类
#define NONE 4
#define LIFE 0
#define POWER 1
#define SPEED 2
#define WEAPON 3
//过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//游戏开始
void Start();
//游戏循环
void Game();
//按键判断
void Key();
//画背景
void DrawBG();
//画游戏
void DrawGame();
//输出到窗口
void Print();
//画地图块
void DrawBlock();
//处理和画玩家子弹
void PlayerBullet();
//画草
void DrawGrass();
//画爆炸火花
void DrawFire();
//初始化
void Init();
//子弹击中砖块
bool BulletHitBlock(Bullet&);
//坦克碰到砖块
bool TankHitBlock(BaseTank&);
//子弹击中坦克
bool BulletHitTank(Bullet&,BaseTank&);
//坦克碰到坦克
bool TankHitTank(BaseTank&,BaseTank&);
//子弹击中子弹
bool BulletHitBullet(Bullet&,Bullet&);
//处理和画敌人坦克
void EnemyTank();
//处理和画敌人子弹
void EnemyBullet();
//处理和画道具
void Item();
//地图编辑
void Edit();
//地图保存
void Save();
//地图载入
void Load();
//载入预置地图
void LoadMap();
//全地图初始化
void MapInit();

