#pragma once

#include "CommonWindow.h"


class Menu :
    public CommonWindow
{
private:

    //���������� ������ ��������
    SDL_Texture* wally;
    SDL_Texture* texture;

    SDL_Rect tempRect;
    SDL_Rect forWall;

    texts* text = new texts[3];

    static Menu* menu;

    //����� �������� ���-��
    ~Menu() override
    {

    }

    //��������� ������
    void DrawingText(SDL_Renderer* renderer) override
    {
        for (int i = 0; i < 3; i++)
        {
            SDL_RenderCopy(renderer, text[i].texture, NULL, &text[i].rect);
        }
        
    }

    //������ ������ ������
    Menu(const char* name, int w, int h)
    {
        //������ �������
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
        surface = SDL_GetWindowSurface(window);
        if (!window || !surface) printf("������");

        //���������� � ���� ������
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL) printf("������ �������");

        //������������� ������ ���� (�� ������ ������)
        widght = w;
        height = h;
        
        //��������� ������
        AddButton<StartButton>(Utility::Point((widght - 300) / 2 - 50, (height - 100) / 2 + 100), 100, 100, 255, 0, 0);
        AddButton<AuthorButton>(Utility::Point((widght - 100) / 2, (height - 100) / 2 + 100), 100, 100, 0, 255, 0);
        AddButton<LeaderBoardButton>(Utility::Point((widght + 100) / 2 + 50, (height - 100) / 2 + 100), 100, 100, 0, 0, 255);

        //��������� ����� �� ������
        int we = 80;
        int he = FONTSIZE;
        int ye = (height - 100) / 2 + 132;

        Utility::CatArray(text[0].text, (UINT16*)L"������");
        Utility::CatArray(text[1].text, (UINT16*)L"�� ����");
        Utility::CatArray(text[2].text, (UINT16*)L"������");

        text[0].rect.x = (widght - 300) / 2 - 40;
        text[1].rect.x = (widght - 100) / 2 + 12;
        text[2].rect.x = (widght + 100) / 2 + 60;
        text[0].rect.y = text[1].rect.y = text[2].rect.y = ye;
        text[0].rect.w = text[1].rect.w = text[2].rect.w = we;
        text[0].rect.h = text[1].rect.h = text[2].rect.h = he;

        if (font2 == NULL)
        {
            perror(TTF_GetError());
        }

        text[0].surface = TTF_RenderUNICODE_Blended(font2, text[0].text, text[0].color);
        text[1].surface = TTF_RenderUNICODE_Blended(font2, text[1].text, text[1].color);
        text[2].surface = TTF_RenderUNICODE_Blended(font2, text[2].text, text[2].color);

        //���������� ����� � ��������
        text[0].texture = SDL_CreateTextureFromSurface(renderer, text[0].surface);
        text[1].texture = SDL_CreateTextureFromSurface(renderer, text[1].surface);
        text[2].texture = SDL_CreateTextureFromSurface(renderer, text[2].surface);

        //���������, ��� ��� ������ �������
        if (text[0].surface == NULL || text[1].surface == NULL || text[2].surface == NULL)
        {
            perror(TTF_GetError());
        }

        //�������������� ��� ������ � ������� ����� �������
        tempRect.x = 0;
        tempRect.y = 0;
        tempRect.w = widght;
        tempRect.h = height / 2;
        forWall.x = 0;
        forWall.y = 0;
        forWall.w = widght;
        forWall.h = height;

        //���������� ��������
        SDL_Surface* image;
        image = IMG_Load(MENUIMAGE);
        SDL_Surface* wall;
        wall = IMG_Load(WALL);

        if (image == NULL || wall == NULL)
        {
            perror(SDL_GetError());
        }
        
        //� ��� ��� � �������� ���������� ��
        wally = SDL_CreateTextureFromSurface(renderer, wall);
        texture = SDL_CreateTextureFromSurface(renderer, image);

        
    }

public:

    static Menu* Get(const char* name, int w, int h)
    {
        if (!menu)
        {
            menu = new Menu(name, w, h);
        }
        return menu;
    }

    //���������� ������
    void Update() override
    {
        SDL_RenderClear(renderer);
        
        //��������� �������
        SDL_RenderCopy(renderer, wally, NULL, &forWall);

        //��������� ������
        for (int i = 0; i < GetCountOfButtons(); i++)
        {
            GetButtons()[i]->Draw(renderer);
        }
        
        //��������� ������
        SDL_RenderCopy(renderer, texture, NULL, &tempRect);

        //��������� ������
        DrawingText(renderer);
        
        SDL_RenderPresent(renderer);
    }

    void Destroy() override {};

};

Menu* Menu::menu = 0;

