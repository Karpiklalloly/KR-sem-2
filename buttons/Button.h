#ifndef UTILITY
#define UTILITY
#include "utils\\Utility.cpp"
#endif


#pragma once
class Button
{
private:
	void DrawBorder(SDL_Renderer* renderer, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, rect.x,			rect.y,				rect.x,				rect.y + rect.h);	//����
		SDL_RenderDrawLine(renderer, rect.x,			rect.y + rect.h,	rect.x + rect.w,	rect.y + rect.h);	//���
		SDL_RenderDrawLine(renderer, rect.x + rect.w,	rect.y + rect.h,	rect.x + rect.w,	rect.y);			//�����
		SDL_RenderDrawLine(renderer, rect.x + rect.w,	rect.y,				rect.x,				rect.y);			//����
	}

	void DrawColor(SDL_Renderer* renderer, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rect);
	}

	void DrawImages(SDL_Renderer* renderer)
	{
		if (image != NULL)
		{
			if (imageChanged)
			{
				imageChanged = false;
				texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(image));
			}
			SDL_RenderCopy(renderer, texture, NULL, GetRect());
		}
	}

protected:
	SDL_Rect rect;
	struct color
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} colors;
	char* type = NULL;

	const char* image = NULL;
	SDL_Texture* texture = NULL;

	bool imageChanged = true;
public:
	//�����������; ����������� ������ ������ ��������
	Button(int w = 0, int h = 0, int x = 0, int y = 0, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
	{
		type = new char[16];
		type[0] = 0;
		Utility::CatArray(type, "Button");
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		colors.r = r;
		colors.g = g;
		colors.b = b;
	}

	//������������� ��������
	SDL_Rect* GetRect() 
	{
		return &rect;
	}

	//�������� ���������� (x;y)
	SDL_Point GetCoordinates()
	{
		SDL_Point temp{};
		temp.x = rect.x;
		temp.y = rect.y;
		return temp;
	}

#pragma region SetCoordinates
	//������ ���������� (x;y)
	void SetCoordinates(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	//������ ���������� (x;y)
	void SetCoordinates(Utility::Point point)
	{
		rect.x = point.x;
		rect.y = point.y;
	}

	//������ ���������� (x;y)
	void SetCoordinates(SDL_Point point)
	{
		rect.x = point.x;
		rect.y = point.y;
	}
#pragma endregion

#pragma region IsInButton
	//��������, ��� ����� ��������� � ������
	bool IsInButton(Utility::Point point)
	{
		SDL_Point p{};
		p.x = point.x;
		p.y = point.y;
		return SDL_PointInRect(&p, &rect);
	}

	//��������, ��� ����� ��������� � ������
	bool IsInButton(SDL_Point point)
	{
		return SDL_PointInRect(&point, &rect);
	}

	//��������, ��� ����� ��������� � ������
	bool IsInButton(int x, int y)
	{
		SDL_Point p{};
		p.x = x;
		p.y = y;
		return SDL_PointInRect(&p, &rect);
	}
#pragma endregion

	//�������� �����
	color GetColor()
	{
		return colors;
	}

	//�������� ���� �� ���� �����
	void ChangeColor(Uint8 r, Uint8 g, Uint8 b)
	{
		colors.r = r;
		colors.g = g;
		colors.b = b;
	}

	//�������� ��� ������
	char* GetType()
	{
		return type;
	}

	//�������� ���� �������
	void Draw(SDL_Renderer* renderer, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255)
	{
		DrawColor(renderer, colors.r, colors.g, colors.b, 255);
		DrawBorder(renderer, r, g, b, a);
		DrawImages(renderer);
	}

	//������ �������� ����� � ���������
	void SetPath(const char* path)
	{
		if (path != NULL)
		{
			imageChanged = true;
			image = path;
		}
	}

	~Button()
	{
		delete[] type;
	}
};

