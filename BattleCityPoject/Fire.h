#pragma once
#include <Windows.h>

class Fire
{
public:
	Fire(
		int _x=0,
		int _y=0);

	~Fire(void);

	int x;
	int y;
	int step;
	bool end;
	void Draw(HDC &,HDC &);

};

