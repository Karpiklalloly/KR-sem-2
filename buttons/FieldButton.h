#pragma once
#include "Button.h"
class FieldButton :
    public Button
{
    bool hasMine = false;
	bool hasFlag = false;
	bool isOpened = false;
	const char* minePath = MINE;
	const char* flagPath = FLAG;
	const char* nominePath = NOMINE;
	const char* number[8] = {"images\\numbers\\number1.png", "images\\numbers\\number2.png", "images\\numbers\\number3.png", "images\\numbers\\number4.png", "images\\numbers\\number5.png", "images\\numbers\\number6.png", "images\\numbers\\number7.png", "images\\numbers\\number8.png"};
	
	char countOfNeighMines = 0;

	color naturalColor;

public:
    FieldButton(int w = 50, int h = 50, int x = 0, int y = 0, unsigned char r = 100, unsigned char g = 100, unsigned char b = 100)
    {
		type[0] = 0;
		Utility::CatArray(type, "FieldButton");
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		colors.r = r;
		colors.g = g;
		colors.b = b;

		naturalColor.r = r;
		naturalColor.g = g;
		naturalColor.b = b;
    }

	void SetCountMines(char c)
	{
		countOfNeighMines = c;
	}

	char GetCountMines()
	{
		return countOfNeighMines;
	}

	//Закладываем мину
	void MakeMine()
	{
		hasMine = true;
	}

	//Смотрим на присутствие мины
	bool HasMine()
	{
		return hasMine;
	}

	//Есть ли флаг?
	bool HasFlag()
	{
		return hasFlag;
	}

	//Открыта ячейка?
	bool IsOpened()
	{
		return isOpened;
	}

	//Меняем состояние флага
	void SwitchFlag()
	{
		if (image == NULL)
			image = flagPath;
		else
			image = NULL;
		hasFlag = !hasFlag;
	}

	//Открываем; если мина есть, то бабах (false), иначе норм (true)
	bool Open()
	{
		if (!isOpened)
		{
			isOpened = true;
			ChangeColor(colors.r - 40, colors.g - 40, colors.b - 40);
			if (hasMine)
			{
				imageChanged = true;
				image = minePath;
				return false;
			}
			image = NULL;
			if (countOfNeighMines && countOfNeighMines < 9)
			{
				imageChanged = true;
				image = number[countOfNeighMines - 1];
			}
				
			return true;
		}
		
		return true;
	}

	void Refresh()
	{
		if (isOpened)
			ChangeColor(naturalColor.r, naturalColor.g, naturalColor.b);
		image = NULL;
		imageChanged = true;
		hasMine = false;
		hasFlag = false;
		isOpened = false;
		countOfNeighMines = 0;
		
	}

	const char* GetNoMinePath()
	{
		return nominePath;
	}

	const char* GetMinePath()
	{
		return minePath;
	}
};


