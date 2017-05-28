#pragma once
#include <Windows.h>

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

class BaseTank
{
public:
	BaseTank(
		int _id=0,
		int _life=20,
		int _armor=0,
		int _face=DOWN,
		int _x=0,
		int _y=0,
		int _gun_face=DOWN,
		bool _gun_lock=false,
		int _move=32,
		int _speed=2,
		int _fire_speed=2,
		int _bullet_id=0,
		int _bullet_max=1,
		int _bullet_power=10,
		int _bullet_speed=5);

	~BaseTank(void);

	void Move();
	void Change(int);
	void Fire();
	void Death();
	void Draw(HDC &,HDC &,bool);

	int id;//坦克种类
	int life;//生命
	int armor;//防御
	int face;//朝向
	int x;//出生坐标
	int y;
	int gun_face;
	bool gun_lock;
	int gun_step;
	int move;//移动
	int move_step;//移动状态
	int speed;//速度
	int speed_real;//实时速度
	int fire_step;//火花状态
	int fire_speed;//火花速度
	int bullet_id;//炮弹种类
	int bullet_num;//炮弹数量
	int bullet_max;
	int bullet_power;//炮弹攻击力
	int bullet_speed;//炮弹速度
	int delay;
};

