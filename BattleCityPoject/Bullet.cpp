#include "Bullet.h"
#include "tank.h"

Bullet::Bullet(
	int _id,
	int _life,
	int _face,
	int _x,
	int _y,
	int _speed,
	int _power):	
id(_id),
	life(_life),
	face(_face),
	x(_x),
	y(_y),
	speed(_speed),
	power(_power),
	step(0)
{
}

Bullet::~Bullet(void)
{
}

void Bullet::Move()
{
	if ( face == DOWN ) y+=speed;
	else if ( face == LEFT ) x-=speed;
	else if ( face == UP ) y-=speed;
	else x+=speed;
	if(++step>6)step=0;
}

void Bullet::Death()
{
}

void Bullet::Draw(HDC &DC1,HDC &DC2)
{
	int x1;
	int y1;
	int step1;

	if(step<4)step1=step*8;
	else step1=(6-step)*8;

	switch(face){
	case DOWN:
		x1=x;
		y1=y-36+step1;
		break;
	case LEFT:
		x1=x+36-step1;
		y1=y;
		break;
	case UP:
		x1=x;
		y1=y+36-step1;
		break;
	case RIGHT:
		x1=x-36+step1;
		y1=y;
	}
	BitBlt(DC1,x1,y1,40,40,DC2,face*40,4*80+40,SRCPAINT);
	BitBlt(DC1,x1,y1,40,40,DC2,face*40,4*80,SRCAND);
	BitBlt(DC1,x,y,40,40,DC2,face*40,id*80+40,SRCPAINT);
	BitBlt(DC1,x,y,40,40,DC2,face*40,id*80,SRCAND);
}