#define _CRT_SECURE_NO_WARNINGS
#define UTILITY
#include "utils/Utility.h"  //Утилитарный класс
#define STHSTH
#include "utils/STH.h"      //Тут есть всякое полезное
#define SDL_MAIN_HANDLED
#ifndef SDL
#define SDL
#endif
#ifndef TTF
#define TTF
#endif
#ifndef IMAGE
#define IMAGE
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

//Сами окна подгружаем
#include "windows/Menu.h"
#include "windows/GameWindow.h"
#include "windows/LeaderWindow.h"
#include "windows/AuthorWindow.h"

//Окошки
CommonWindow* window = NULL;    //Для того, чтобы двигаться по разным окнам, меняя только указатель
Menu* menu = NULL;              //Непосредственно меню
GameWindow* game = NULL;        //Игровое окно
LeaderWindow* leaders = NULL;   //Окно с лидерами
AuthorWindow* author = NULL;    //Окно "Об игре"


bool needRedraw = true;

//Проверка, что координаты в кнопке
bool InButton(Button** button, int countOfButtons, Utility::Point point, Button** but)
{
    //Сразу в нулл задаем, чтобы если не попали в кноку, то и указателя на кнопку тоже не было
    *but = NULL;
    //Смотрим на все кнопки в поле зрения
    for (int i = 0; i < countOfButtons; i++)
    {
        //Ну и смотрим по координатам
        if (button[i]->IsInButton(point))
        {
            //Если попали в кнопку, то она наша
            *but = button[i];
            return true;
        }
    }
    return false;
}

//Действие на левую кнопку мыши
void ButtonActionLeft(Button* button)
{
    //Если кнопка на поле
    if (!strcmp(button->GetType(), "FieldButton"))
    {
        if (game != NULL)
        {
            if (!game->Started())
            {
                game->SetStart();
                game->SetMines(button);
            }
            if (!((FieldButton*)button)->HasFlag() && !((FieldButton*)button)->IsOpened() && game->CanChange())
            {
#if CHEATS==1
                //Неуязвимость
                if (((FieldButton*)button)->HasMine())
                {
                    std::cout << game->GetCountToUnclock() << std::endl;
                    return;
                }
#endif
                if (((FieldButton*)button)->HasMine())
                {
                    game->Open(button);
                    std::cout << "Бабах!" << std::endl;
                    game->GameLost(button);
                }
                else
                {
                    game->Open(button);
                    game->OpenNeigh(button);
                    if (game->GetCountToUnclock() <= 0)
                        game->GameWon();
                }
            }

            needRedraw = true;
        }
    }
    //Если просто кнопка какая-то
    else if (!strcmp(button->GetType(), "Button"))
    {
        std::cout << "Кнопочка обыкновенная" << std::endl;
    }
    //Если кнопка старта
    else if (!strcmp(button->GetType(), "StartButton"))
    {
        game = game->Get(u8"Сапер", WIDTH, HEIGHT);
        window = game;
        SDL_HideWindow(menu->window);
        needRedraw = true;
    }
    //Если кнопка автора
    else if (!strcmp(button->GetType(), "AuthorButton"))
    {
        author = author->Get(u8"Об игре", WIDTH, HEIGHT);
        window = author;
        SDL_HideWindow(menu->window);
        needRedraw = true;
    }
    //Если кнопка лидеров
    else if (!strcmp(button->GetType(), "LeaderBoardButton"))
    {
        leaders = leaders->Get(u8"Лидеры", 1280, 720);
        window = leaders;
        SDL_HideWindow(menu->window);
        needRedraw = true;
    }
    //Если кнопка обратно в меню
    else if (!strcmp(button->GetType(), "ButtonToMenu"))
    {
        window->Destroy();
        window = menu;
        SDL_ShowWindow(menu->window);
        needRedraw = true;
    }
    //Если кнопка обновления игрового поля
    else if (!strcmp(button->GetType(), "RefreshButton"))
    {
        if (game != NULL)
        {
            game->Refresh();
            needRedraw = true;
        }
    }
    else if (!strcmp(button->GetType(), "DifficultyButton"))
    {
        if (game != NULL && !game->Started())
        {
            game->RefreshDif(button);
            needRedraw = true;
        }
    }
}

//Действие на правую кнопку мыши
void ButtonActionRight(Button* button)
{
    //Если кнопка на поле
    if (game != NULL && !strcmp(button->GetType(), "FieldButton") && game->CanChange())
    {
        if (game->Started())
        {
            if (!((FieldButton*)button)->IsOpened())
            {
                ((FieldButton*)button)->SwitchFlag();
                if (((FieldButton*)button)->HasMine()) std::cout << "МИНА" << std::endl;
                needRedraw = true;
            }
        }
    }

}

//Инициализируем всякое
bool Init()
{
    //Графику
    SDL_Init(SDL_INIT_VIDEO);
    //ПНГ
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        perror(IMG_GetError());
        return false;
    }
    //ЖПГ
    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
    {
        perror(IMG_GetError());

        return false;
    }
    //ТТФ
    if (!TTF_WasInit() && TTF_Init() == -1)
    {
        perror(TTF_GetError());

        return false;
    }
    //Создаем меню сразу
    menu = menu->Get(u8"Меню", WIDTH, HEIGHT);
    if (!menu)
    {
        perror("Память не выделилась");
        return false;
    }
    //Ну и проверка, что создалось
    if (menu->window == NULL || menu->surface == NULL)
    {
        perror("Ошибка создания окна");
        return false;
    }
    atexit(TTF_Quit);
    atexit(IMG_Quit);
    atexit(SDL_Quit);
    
    return true;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    Utility::SetLocale();

    //Для запоминания, какие кнопки нажаты
    Button* currentButtonLeft = NULL;
    Button* downedButtonLeft = NULL;
    Button* currentButtonRight = NULL;
    Button* downedButtonRight = NULL;



    //Смотрим, проинициализировалось ли
    if (!Init())
    {
        SDL_Quit();
        return -1;
    }
    //Начальное окно - меню
    window = menu;

    do
    {
        //Обновление экрана, если требуется
        if (needRedraw)
        {
            needRedraw = false;
            window->Update();
        }

        //Обновляем таймер
        if (window == game && !Utility::TimeLeft(1))
        {
            if (game->Started() && game->CanChange())
            {
                needRedraw = true;
                game->UpdateTimer();
            }
        }

        window->event.type = SDL_FIRSTEVENT;
        //Ждем события
        SDL_PollEvent(&window->event);
        //Если на крест нажали
        if (window->event.type == SDL_QUIT)
            break;

        //Если нажали
        if (window->event.type == SDL_MOUSEBUTTONDOWN)
        {
            //Если левой кнопкой мыши нажали, то просто запоминаем на что именно
            if (window->event.button.button == SDL_BUTTON_LEFT)
            {
                InButton(window->GetButtons(), window->GetCountOfButtons(), Utility::Point(window->event.button.x, window->event.button.y), &downedButtonLeft);//Просто запоминаем кнопку, на которую нажали
            }

            //Если правой кнопкой мыши нажали, то просто запоминаем на что именно
            if (window->event.button.button == SDL_BUTTON_RIGHT)
            {
                InButton(window->GetButtons(), window->GetCountOfButtons(), Utility::Point(window->event.button.x, window->event.button.y), &downedButtonRight);//Просто запоминаем кнопку, на которую нажали
            }
        }
        //Если отжали
        if (window->event.type == SDL_MOUSEBUTTONUP)
        {
            //Если левую
            if (window->event.button.button == SDL_BUTTON_LEFT)
            {
                if (InButton(window->GetButtons(), window->GetCountOfButtons(), Utility::Point(window->event.button.x, window->event.button.y), &currentButtonLeft))
                {
                    if (currentButtonLeft == downedButtonLeft)//Если та же кнопка, что и была
                    {
                        ButtonActionLeft(currentButtonLeft);
                    }
                }
            }

            //Если правую
            if (window->event.button.button == SDL_BUTTON_RIGHT)
            {
                if (InButton(window->GetButtons(), window->GetCountOfButtons(), Utility::Point(window->event.button.x, window->event.button.y), &currentButtonRight))
                {
                    if (currentButtonRight == downedButtonRight)//Если та же кнопка, что и была
                    {
                        ButtonActionRight(currentButtonRight);
                    }
                }
            }
        }

    } while (1);
    return 0;
}
