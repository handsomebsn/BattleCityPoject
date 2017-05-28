#include "BaseTank.h"
#include "tank.h"

BaseTank::BaseTank(
	int _id,
	int _life,
	int _armor,
	int _face,
	int _x,
	int _y,
	int _gun_face,
	bool _gun_lock,
	int _move,
	int _speed,
	int _fire_speed,
	int _bullet_id,
	int _bullet_max,
	int _bullet_power,
	int _bullet_speed):	
id(_id),
	life(_life),
	armor(_armor),
	face(_face),
	x(_x),
	y(_y),
	gun_face(_gun_face),
	gun_lock(_gun_lock),
	gun_step(0),
	move(_move),
	move_step(0),
	speed(_speed),
	speed_real(speed),
	fire_step(0),
	fire_speed(_fire_speed),
	bullet_id(_bullet_id),
	bullet_num(0),
	bullet_max(_bullet_max),
	bullet_power(_bullet_power),
	bullet_speed(_bullet_speed),
	delay(0)
{	
}


BaseTank::~BaseTank(void)
{
}

void BaseTank::Move()
{
	if ( face == DOWN ) y+=speed_real;
	else if ( face == LEFT ) x-=speed_real;
	else if ( face == UP ) y-=speed_real;
	else x+=speed_real;
	if (++move_step>2) move_step=0;
}

void BaseTank::Change(int _face)
{
	face=_face;
	if(gun_lock==false)gun_face=face;
	if (++move_step>2) move_step=0;
}

void BaseTank::Fire()
{
	if(!gun_step)
	{
		gun_step=100;
		fire_step=3;
	}

}

void BaseTank::Death()
{
}

void BaseTank::Draw(HDC &DC1,HDC &DC2,bool fire)
{
	int gun_x;
	int gun_y;
	int fire_x;
	int fire_y;
	int _gun_step;
	_gun_step=gun_step/20;
	switch(gun_face)
	{
	case DOWN:
		gun_x=x;
		gun_y=y-_gun_step*(bullet_power/10);
		fire_x=gun_x;
		fire_y=gun_y+48;
		break;
	case LEFT:
		gun_x=x+_gun_step*(bullet_power/10);
		gun_y=y;
		fire_x=gun_x-48;
		fire_y=gun_y;
		break;
	case UP:
		gun_x=x;
		gun_y=y+_gun_step*(bullet_power/10);
		fire_x=gun_x;
		fire_y=gun_y-48;
		break;
	case RIGHT:
		gun_x=x-_gun_step*(bullet_power/10);
		gun_y=y;
		fire_x=gun_x+48;
		fire_y=gun_y;
	}
	if(fire==false)
	{
	//Ì¹¿Ëµ×ÅÌ
	BitBlt(DC1,x,y,64,64,DC2,face*64,id*1280+move_step*128+64,SRCPAINT);
	BitBlt(DC1,x,y,64,64,DC2,face*64,id*1280+move_step*128,SRCAND);
	//Ì¹¿ËÅÚÌ¨
	BitBlt(DC1,gun_x,gun_y,64,64,DC2,gun_face*64,id*1280+(5-_gun_step)*128+128*3+64,SRCPAINT);
	BitBlt(DC1,gun_x,gun_y,64,64,DC2,gun_face*64,id*1280+(5-_gun_step)*128+128*3,SRCAND);
	}
	else
	{
		//Ì¹¿Ë»ğ»¨
		if(fire_step>0)
		{
			--fire_step;
			BitBlt(DC1,fire_x,fire_y,64,64,DC2,gun_face*64,id*1280+9*128+64,SRCPAINT);
			BitBlt(DC1,fire_x,fire_y,64,64,DC2,gun_face*64,id*1280+9*128,SRCAND);
		}
		if(gun_step>0)
		{
			gun_step-=fire_speed;
			if(gun_step<0)gun_step=0;
		}
	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      