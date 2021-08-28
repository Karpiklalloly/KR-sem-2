#pragma once
#include "CommonWindow.h"
class LeaderWindow :
    public CommonWindow
{
    
    static LeaderWindow* leaders;

    SDL_Texture* wally;
    SDL_Rect forWall;
    SDL_Rect frameMin;
    SDL_Rect frameAvg;
    SDL_Rect frameMax;

    texts* text;
    SDL_Surface*** surf;
    SDL_Texture*** textu;
    int* countToDraw;

    SDL_Texture** ren;

    STH::FileData** datas;

    int IntLen(int number)
    {
        int i = 0;
        for (; number > 0; i++)
        {
            number = number / 10;
        }
        return i;
    }

    ~LeaderWindow() override
    {
        leaders = NULL;
    }

    LeaderWindow() {};

    LeaderWindow(const char* name, int w, int h)
    {
        if (window != NULL)
        {
            SDL_DestroyWindow(window);
        }
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
        if (surface != NULL)
            SDL_FreeSurface(surface);
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

        //Добавляем текст на кнопку
        int we = 50;
        int he = 20;
        int xe = 15;
        int ye = 15;

        text = new texts();
        Utility::CatArray(text[0].text, (UINT16*)L"Меню");
        text[0].rect.x = xe;
        text[0].rect.y = ye;
        text[0].rect.w = we;
        text[0].rect.h = he;
        text[0].surface = TTF_RenderUNICODE_Blended(font2, text[0].text, text[0].color);
        text[0].texture = SDL_CreateTextureFromSurface(renderer, text[0].surface);

        int wewe = widght / 3 - 100;    //w
        int hehe = 2 * height / 3;      //h
        int yeye = 100;                 //y

        frameMin = { 50, yeye, wewe, hehe };
        frameAvg = { widght / 2 - wewe / 2, yeye, wewe, hehe };
        frameMax = { widght - wewe - 50, yeye, wewe, hehe };

        //Подгружаем картинки всякие и закидываем в текстуры
        SDL_Surface* wall;
        wall = IMG_Load(WALL);
        wally = SDL_CreateTextureFromSurface(renderer, wall);
        SDL_FreeSurface(wall);

        SDL_Surface* sur = IMG_Load(FRAME);
        ren = new SDL_Texture * [3];
        ren[0] = ren[1] = ren[2] = SDL_CreateTextureFromSurface(renderer, sur);
        SDL_FreeSurface(sur);

        datas = new STH::FileData * [3];
        for (int i = 0; i < 3; i++)
        {
            datas[i] = new STH::FileData[COUNTOFLEADERS];
        }
        std::fstream f[3];
        f[0] = std::fstream(LEADERMINFILENAME, std::ios::binary | std::ios::in);
        f[1] = std::fstream(LEADERAVGFILENAME, std::ios::binary | std::ios::in);
        f[2] = std::fstream(LEADERMAXFILENAME, std::ios::binary | std::ios::in);
        for (int k = 0; k < 3; k++)
        {
            for (int i = 0; i < COUNTOFLEADERS; i++)
            {
                if (!f[k].read(reinterpret_cast<char*>(&datas[k][i]), sizeof(datas[k][i])))
                {
                    std::cout << "Не считывается i " << i << std::endl;
                    for (; i < COUNTOFLEADERS; i++)
                    {
                        datas[k][i].result = 12321321;
                        datas[k][i].userName[0] = 0;
                    }
                    break;
                }
            }
        }
        int m = 0;
        for (int k = 0; k < 3; k++)
        {
            f[k].close();
            for (int i = 0; i < COUNTOFLEADERS; i++)
            {
                m = 0;
                for (; datas[k][i].userName[m] != '\0' && m < datas[k][i].lenght; m++)
                {

                }
                datas[k][i].userName[m] = '\0';
            }
        }
        
        surf = new SDL_Surface ** [3];
        for (int i = 0; i < 3; i++)
        {
            surf[i] = new SDL_Surface * [COUNTOFLEADERS];
        }
        textu = new SDL_Texture ** [3];
        for (int i = 0; i < 3; i++)
        {
            textu[i] = new SDL_Texture * [COUNTOFLEADERS * 2];
        }
        UINT16* temp = new UINT16[16];
        SDL_Surface* surfff;
        SDL_Color colorr;
        colorr.r = 0;
        colorr.g = 0;
        colorr.b = 0;
        colorr.a = 255;
        int t = 0;
        countToDraw = new int[3];
        countToDraw[0] = countToDraw[1] = countToDraw[2] = COUNTOFLEADERS;

        for (int k = 0; k < 3; k++)
        {
            for (int i = 0; i < COUNTOFLEADERS; i++)
            {
                
                if (datas[k][i].userName[0] == 0)
                {
                    countToDraw[k] = i;
                    break;
                }
                temp[0] = 0;
                surf[k][i] = TTF_RenderUTF8_Blended(font2, (char*)datas[k][i].userName, { 255, 10, 10, 255 });
                textu[k][2 * i] = SDL_CreateTextureFromSurface(renderer, surf[k][i]);
                if (surf[k][i] == NULL || textu[k][2 * i] == NULL)
                {
                    perror(SDL_GetError());
                }
                t = datas[k][i].result;
                int p = 0;
                while (t != 0)
                {
                    int r = t % 10;
                    temp[p++] = (r > 9) ? (r - 10) + 'a' : r + '0';
                    t = t / 10;
                }
                temp[p] = '\0';
                Utility::ReverseArray(temp, p);
                
                if ((surfff = TTF_RenderUNICODE_Blended(font2, temp, colorr)) == NULL)
                {
                    perror(SDL_GetError());
                }
                textu[k][2 * i + 1] = SDL_CreateTextureFromSurface(renderer, surfff);
                SDL_FreeSurface(surfff);
            }
        }
        delete[] temp;
        }

    //Рисование текста
    void DrawingText(SDL_Renderer* renderer) override
    {
        for (int i = 0; i < 1; i++)
        {
            SDL_RenderCopy(renderer, text[i].texture, NULL, &text[i].rect);
        }
        SDL_Rect* rect = new SDL_Rect[3];
        rect[0].x = frameMin.x + 30;
        rect[0].y = frameMin.y + 55;
        rect[0].w = 100;
        rect[0].h = FONTSIZE;

        rect[1].x = frameAvg.x + 30;
        rect[1].y = frameAvg.y + 55;
        rect[1].w = 100;
        rect[1].h = FONTSIZE;

        rect[2].x = frameMax.x + 30;
        rect[2].y = frameMax.y + 55;
        rect[2].w = 100;
        rect[2].h = FONTSIZE;

        SDL_Rect tempRect;
        tempRect.h = rect[0].h;
        
        for (int k = 0; k < 3; k++)
        {
            for (int i = 0; i < countToDraw[k]; i++)
            {
                tempRect.x = rect[k].x;
                tempRect.y = rect[k].y + i * (FONTSIZE + 5);
                tempRect.w = (datas[k][i].trueLen + 1) * (FONTSIZE - 1);
                if (SDL_RenderCopy(renderer, textu[k][2 * i], NULL, &tempRect))
                    perror(SDL_GetError());
                tempRect.x = rect[k].x + MAXUSERNAME * FONTSIZE + 25;
                tempRect.w = IntLen(datas[k][i].result) * FONTSIZE;
                
                if (SDL_RenderCopy(renderer, textu[k][2 * i + 1], NULL, &tempRect))
                    perror(SDL_GetError());
            }
        }
        delete[] rect;
    }

public:
    
    static LeaderWindow* Get(const char* name, int w, int h)
    {
        if (!leaders)
        {
            leaders = new LeaderWindow(name, w, h);
        }
        return leaders;
    }

    //Уничтожаем окно и задаем NULL static переменной
    void Destroy() override
    {
        for (int i = 0; i < 3; i++)
        {
            SDL_DestroyTexture(ren[i]);
            delete[] datas[i];
        }
        for (int i = 0; i < 3; i++)
        {
            for (int k = 0; k < countToDraw[i]; k++)
            {
                SDL_FreeSurface(surf[i][k]);
                SDL_DestroyTexture(textu[i][k]);
            }
        }
        SDL_DestroyTexture(wally);
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        TTF_CloseFont(font2);
        SDL_DestroyWindow(window);
        delete text;
        delete[] buttonStruct.buttons;
        delete[] countToDraw;
        
        leaders = NULL;
    }

    void Update() override
    {
        SDL_RenderClear(renderer);

        //Рисование задника
        SDL_RenderCopy(renderer, wally, NULL, &forWall);

        SDL_RenderCopy(renderer, ren[0], NULL, &frameMin);
        SDL_RenderCopy(renderer, ren[1], NULL, &frameAvg);
        SDL_RenderCopy(renderer, ren[2], NULL, &frameMax);

        //Рисование кнопок
        for (int i = 0; i < GetCountOfButtons(); i++)
        {
            GetButtons()[i]->Draw(renderer);
        }

        DrawingText(renderer);

        SDL_RenderPresent(renderer);
    }
};

LeaderWindow* LeaderWindow::leaders = 0;
