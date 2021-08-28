#pragma once
#include "Button.h"
class DifficultyButton :
    public Button
{
	bool checked = false;
public:
	DifficultyButton(int w = 0, int h = 0, int x = 0, int y = 0, unsigned char r = 0, unsigned char g = 255, unsigned char b = 0)
	{
		type[0] = 0;
		Utility::CatArray(type, "DifficultyButton");
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		colors.r = r;
		colors.g = g;
		colors.b = b;
	}

	bool GetState()
	{
		return checked;
	}

	void SetState(bool state)
	{
		checked = state;
	}
};

