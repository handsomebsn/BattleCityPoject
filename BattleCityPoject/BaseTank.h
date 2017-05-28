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

	int id;
	int life;
	int armor;
	int face;
	int x;
	int y;
	int gun_face;
	bool gun_lock;
	int gun_step;
	int move;
	int move_step;
	int speed;
	int speed_real;
	int fire_step;
	int fire_speed;
	int bullet_id;
	int bullet_num;
	int bullet_max;
	int bullet_power;
	int bullet_speed;
	int delay;
};

