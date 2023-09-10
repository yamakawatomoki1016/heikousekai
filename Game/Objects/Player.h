#pragma once
#include <Vector2.h>
struct Chara {
	Vector2 pos;
	int radius;
	unsigned int color;
};

class Player
{
public:
	void Initialize();
	void Update(char* keys, char* preKeys);
	void Draw();
public:
	Chara BluePlayer = {
		{1,8},
		32,
		0x00ffffff,
	};

	Chara PinkPlayer = {
		{1,15},
		32,
		0xff66ffff,
	};
private:
	bool UpSideGround = false;
	bool DownSideGround = true;
public:
	bool changePlayerFlag = false;

};

