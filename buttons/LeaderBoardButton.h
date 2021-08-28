#pragma once
#include "Button.h"
class LeaderBoardButton :
    public Button
{
public:
	LeaderBoardButton(int w = 0, int h = 0, int x = 0, int y = 0, unsigned char r = 0, unsigned char g = 0, unsigned char b = 255)
	{
		type[0] = 0;
		Utility::CatArray(type, "LeaderBoardButton");
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		colors.r = r;
		colors.g = g;
		colors.b = b;
	}

};

