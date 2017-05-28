#pragma once
#include <Windows.h>

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

class Bullet
{
public:
	Bullet(
		int _id=0,
		int _life=100,
		int _face=DOWN,
		int _x=0,
		int _y=0,
		int _speed=10,
		int _power=10
		);
	~Bullet(void);

	void Move();
	void Draw(HDC &,HDC &);
	void Death();


	int id;
	int life;
	int face;
	int x;
	int y;
	int speed;
	int power;
	int step;

};

