#include "Fire.h"

Fire::Fire(
	int _x,
	int _y):
x(_x),
	y(_y),
	step(0),
	end(false)
{
}


Fire::~Fire(void)
{
}

void Fire::Draw(HDC& DC1,HDC& DC2)
{
	if(end==false)
	{
		BitBlt(DC1,x,y,66,66,DC2,step/3*66,66,SRCPAINT);
		BitBlt(DC1,x,y,66,66,DC2,step/3*66,0,SRCAND);
		if(++step==24)end=true;
	}
}
