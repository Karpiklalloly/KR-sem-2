#pragma once

#include <SDL.h>
#include <stdio.h>
#include "..\\buttons\\Button.h"
#include "..\\buttons\\AuthorButton.h"
#include "..\\buttons\\ButtonToMenu.h"
#include "..\\buttons\\DifficultyButton.h"
#include "..\\buttons\\FieldButton.h"
#include "..\\buttons\\LeaderBoardButton.h"
#include "..\\buttons\\RefreshButton.h"
#include "..\\buttons\\StartButton.h"
#ifndef STHSTH
#define STHSTH
#include "..\\utils\\STH.h"
#endif
#ifndef UTILITY
#define UTILITY
#include "..\\utils\\Utility.h"
#endif

//Класс самого обычного окна
class CommonWindow
{
public:
	SDL_Window* window = NULL;			//Окно
	SDL_Event event;					//Здесь событие окна записывается
	SDL_Surface* surface = NULL;		//Поверхность окна

protected:
	int widght = 0;					//Ширина
	int height = 0;					//Высота
	SDL_Renderer* renderer = NULL;	//Сюда закидываем всю картинку
	TTF_Font* font2 = TTF_OpenFont(FONT, FONTSIZE * 3);	//Шрифт

	
	struct but
	{
		Button** buttons = NULL;
		int count = 0;
		~but()
		{
			for (int i = 0; i < count; i++)
			{
				delete buttons[i];
			}
		}
	} buttonStruct;

	struct texts
	{
		UINT16 text[30];
		SDL_Color color;
		SDL_Rect rect{};
		SDL_Surface* surface = NULL;
		SDL_Texture* texture = NULL;

		texts()
		{
			text[0] = '\0';
			color.r = 0;
			color.g = 0;
			color.b = 0;
			color.a = 255;
		}

		texts(const UINT16* t)
		{
			int i = 0;
			for (; t[i] != '\0'; i++)
			{
				text[i] = t[i];
			}
			text[i] = t[i];
			color.r = 0;
			color.g = 0;
			color.b = 0;
			color.a = 255;
		}
		~texts()
		{
			if (surface != NULL)
				SDL_FreeSurface(surface);
			if (texture != NULL)
				SDL_DestroyTexture(texture);
		}
	};

	//Добавление кнопки
	template<class bu>
	void AddButton(Utility::Point coord, int w, int h, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
	{
		bu* temp = new bu(w, h, coord.x, coord.y, r, g, b);
		Button** temp2 = buttonStruct.buttons;

		buttonStruct.buttons = new Button * [buttonStruct.count + 1];
		for (int i = 0; i < buttonStruct.count; i++)
		{
			buttonStruct.buttons[i] = temp2[i];
		}
		buttonStruct.buttons[buttonStruct.count] = temp;
		delete[] temp2;
		buttonStruct.count++;
	}

	
	virtual void DrawingText(SDL_Renderer* renderer) {};

	CommonWindow() {};

public:
	CommonWindow(char* name, int w, int h)
	{
		window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
		surface = SDL_GetWindowSurface(window);
		if (!window || !surface) printf("Ошибка");
		widght = w;
		height = h;
		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	CommonWindow(const char* name, int w, int h) : CommonWindow((char*)name, w, h) {};

	//Обновляем окно
	virtual void Update()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, NULL, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	//Указатель на кнопки
	Button** GetButtons()
	{
		return buttonStruct.buttons;
	}

	//Количество кнопок
	int GetCountOfButtons()
	{
		return buttonStruct.count;
	}

	virtual ~CommonWindow()
	{
	}

	virtual void Destroy() = 0;
};




