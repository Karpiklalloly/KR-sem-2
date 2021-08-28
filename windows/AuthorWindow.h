#pragma once
#include "CommonWindow.h"
class AuthorWindow :
    public CommonWindow
{
    AuthorWindow() {};

    static AuthorWindow* author;

    SDL_Rect frame;
    SDL_Texture* ren;

    SDL_Texture* wally;
    SDL_Rect forWall;

    texts* text;

    ~AuthorWindow() override
    {
        SDL_DestroyWindow(window);
        author = NULL;
        delete[] text;
    }

    AuthorWindow(const char* name, int w, int h)
    {
        if (window != NULL)
        {
            SDL_DestroyWindow(window);
        }
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
        if (surface != NULL)
        {
            SDL_FreeSurface(surface);
        }
        surface = SDL_GetWindowSurface(window);
        if (!window || !surface)
            printf("Ошибка");
        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL) printf("Ошибка рендера");

        widght = w;
        height = h;

        //Задаем размеры 
        forWall.x = 0;
        forWall.y = 0;
        forWall.w = widght;
        forWall.h = height;

        //Создаем кнопки в окне
        AddButton<ButtonToMenu>(Utility::Point(10, 10), 60, 30, 255, 100, 100);

        frame = { 50, 50, widght - 100, height * 8/10};

        //Добавляем текст на кнопку
        int we = 50;
        int he = FONTSIZE/2;
        int xe = 15;
        int ye = 15;
        

        text = new texts[12];
        Utility::CatArray(text[0].text, (UINT16*)L"Меню");
        text[0].rect.x = xe;
        text[0].rect.y = ye;
        text[0].rect.w = we;
        text[0].rect.h = he;
        text[0].surface = TTF_RenderUNICODE_Blended(font2, text[0].text, text[0].color);
        text[0].texture = SDL_CreateTextureFromSurface(renderer, text[0].surface);

        //Добавляем текст
        int yes = 100;
        double koef = 0.8;

        Utility::CatArray(text[1].text, (UINT16*)L"Автор:");
        Utility::CatArray(text[2].text, (UINT16*)L"Студент группы И904Б");
        Utility::CatArray(text[3].text, (UINT16*)L"Карпинский А.А.");
        Utility::CatArray(text[4].text, (UINT16*)L"университета ВОЕНМЕХ");
        Utility::CatArray(text[5].text, (UINT16*)L" ");
        Utility::CatArray(text[6].text, (UINT16*)L"Задача игры - разминировать");
        Utility::CatArray(text[7].text, (UINT16*)L"все мины на поле,");
        Utility::CatArray(text[8].text, (UINT16*)L"помечая флажками ячейки,");
        Utility::CatArray(text[9].text, (UINT16*)L"на которых могут быть мины.");
        Utility::CatArray(text[10].text, (UINT16*)L"Для их обнаружения есть");
        Utility::CatArray(text[11].text, (UINT16*)L"подсказки в виде чисел");
        for (unsigned char i = 1; i < 12; i++)
        {
            text[i].rect.y = yes + i * (FONTSIZE + 5)*koef;
            text[i].rect.x = w / 2 - Utility::LenghtOf(text[i].text)* FONTSIZE/2 * koef;
            text[i].rect.w = Utility::LenghtOf(text[i].text) * FONTSIZE * koef;
            text[i].rect.h = FONTSIZE * koef;
            text[i].color.r = 0;
            text[i].color.g = 50;
            text[i].color.b = 255;
            text[i].color.a = 255;
            text[i].surface = TTF_RenderUNICODE_Blended(font2, text[i].text, text[i].color);
            text[i].texture = SDL_CreateTextureFromSurface(renderer, text[i].surface);
        }

        //Подгружаем картинки всякие и закидываем в текстуры
        SDL_Surface* wall;
        wall = IMG_Load(WALL);
        wally = SDL_CreateTextureFromSurface(renderer, wall);
        SDL_FreeSurface(wall);

        SDL_Surface* sur = IMG_Load(FRAME);
        ren = SDL_CreateTextureFromSurface(renderer, sur);
        SDL_FreeSurface(sur);
        
    }

    //Рисование текста
    void DrawingText(SDL_Renderer* renderer) override
    {
        for (int i = 0; i < 12; i++)
        {
            SDL_RenderCopy(renderer, text[i].texture, NULL, &text[i].rect);
        }
    }
public:

    static AuthorWindow* Get(const char* name, int w, int h)
    {
        if (!author)
        {
            author = new AuthorWindow(name, w, h);
        }
        return author;
    }

    //Уничтожаем окно и задаем NULL static переменной
    void Destroy() override
    {
        SDL_DestroyTexture(ren);
        SDL_DestroyTexture(wally);
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        TTF_CloseFont(font2);
        SDL_DestroyWindow(window);
        delete[] text;
        delete[] buttonStruct.buttons;
        author = NULL;
    }

    void Update() override
    {
        SDL_RenderClear(renderer);

        //Рисование задника
        SDL_RenderCopy(renderer, wally, NULL, &forWall);

        SDL_RenderCopy(renderer, ren, NULL, &frame);

        //Рисование кнопок
        for (int i = 0; i < GetCountOfButtons(); i++)
        {
            GetButtons()[i]->Draw(renderer);
        }

        DrawingText(renderer);

        SDL_RenderPresent(renderer);
    }
};

AuthorWindow* AuthorWindow::author = 0;

