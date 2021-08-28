#pragma once
#include "Button.h"
class RefreshButton :
    public Button
{
    const char* smilePath = SMILE;
	const char* unsmilePath = UNSMILE;
	const char* winSmilePath = NICE;

public:
	RefreshButton(int w = 50, int h = 50, int x = 0, int y = 0, unsigned char r = 100, unsigned char g = 100, unsigned char b = 100)
	{
		type[0] = 0;
		Utility::CatArray(type, "RefreshButton");
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		colors.r = r;
		colors.g = g;
		colors.b = b;

		image = smilePath;
	}

	const char* GetSmilePath()
	{
		return smilePath;
	}

	//0 - обычная ситуация; 1 - проиграл; 2 -выиграл
	void SetState(int state)
	{
		switch (state)
		{
		case 0:
		{
			image = smilePath;
			imageChanged = true;
			break;
		}
		case 1:
		{
			image = unsmilePath;
			imageChanged = true;
			break;
		}
		case 2:
		{
			image = winSmilePath;
			imageChanged = true;
			break;
		}

		}
	}

};

