#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <windows.h>
#include <string.h>

#ifndef SDL
#define SDL
#include <SDL.h>
#endif

#ifndef TTF
#define TTF
#include <SDL_ttf.h>
#endif

#ifndef IMAGE
#define IMAGE
#include <SDL_image.h>
#endif

class Utility
{
public:
    class Point
    {
    public:

        int x, y;
        Point(const int x, const int y)
        {
            this->x = x;
            this->y = y;
        }
        Point(const SDL_Point& point)
        {
            x = point.x;
            y = point.y;
        }

        Point(const Point& point)
        {
            x = point.x;
            y = point.y;
        }

        Point operator=(const Point point)
        {
            x = point.x;
            y = point.y;
        }
    };

    SDL_Rect* Rectangle(int w, int h, int x, int y)
    {
        SDL_Rect* rect = new SDL_Rect();
        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
        return rect;
    }

    //Возвращает, сколько ждать осталось
    static int TimeLeft(int time)
    {
        static int nextTime = 0;
        int nowTime = SDL_GetTicks();
        time *= 1000;
        //std::cout << (nextTime - nowTime) / 1000 << std::endl;
        if (nextTime <= nowTime)
        {
            nextTime = nowTime + time;
            return 0;
        }
        return nextTime - nowTime;
    }

    //Функция очистки потока ввода
    static void ClearInput()
        {
            std::cin.clear();
            std::wcin.clear();
            fflush(stdin);
            std::fflush(stdin);
        }

    //Функция ожидания нажатия
    static void Pause()
        {
            std::cout << std::endl;
            system("Pause");
        }

    //Функция очистки экрана
    static void ClearScreen()
        {
            system("cls");
        }

    //Функция чистки экрана и потока ввода
    static void ClearEth()
        {
            ClearInput();
            ClearScreen();
        }

    //Функция считывания символа; набор символов, которые допустимы для ввода, указываются через булеву функцию
    template<class T>
    static char InputSth(bool (*f)(T))
        {
            T c = 0;
            do
            {
                std::cin >> c;
            } while (!f(c));
        }

    //Циферка?
    static bool IsDigit(const char symbol)
        {
            return symbol > 47 && symbol < 58;
        }

    //В ASCII?
    static bool IsASCII(const char symbol)
        {
            return symbol >= 0 && symbol <= 127;
        }

    //Буква американская?
    static int IsASCIIChar(const char symbol)
        {
            return (symbol > 64 && symbol < 91) || (symbol > 96 && symbol < 123);
        }

    //Пробел и другие невидимые знаки?
    static int IsSpace(const char symbol)
        {
            return symbol == ' ' || symbol == '\t';
        }

    //Какой-то символ странный?
    static int IsDot(const char symbol)
        {
            return (symbol > 32 && symbol < 48) || (symbol > 57 && symbol < 65) || (symbol > 90 && symbol < 97) || (symbol > 122 && symbol <= 127);
        }

    template<class T>
    static bool DataInArray(T data, T* array, int n)
    {
        while (--n >= 0)
        {
            if (data == array[n])
                return true;
        }
        return false;
    }

    //Определяет длину строки
    template<class T>
    static int LenghtOf(const T* arr, const T symbol = '\0')
        {
            int i = 0;
            for (; arr[i] != symbol; i++);
            return i;
        }

    //Копирует массив 2 в массив 1; сделал, т.к. strcpy_s крашит
    template<class T>
    static void CopyArray(T* arr1, const T* const arr2, bool debug = 0)
        {
            int i = 0;
            int lenght = LenghtOf<T>(arr2);
            if (debug)
            {
                for (; i < lenght; i++)
                {
                    arr1[i] = arr2[i];
                    std::cout << "i = " << i << std::endl;
                }
                arr1[i] = '\0';
                std::cout << "Lenght of arr 2 = " << lenght << std::endl;
                Pause();
                ClearEth();
            }
            else
            {
                for (; i < lenght; i++)
                {
                    arr1[i] = arr2[i];
                }

                arr1[i] = '\0';
            }

        }

    //Конкатинирует массив 2 после массива 1
    template<class T>
    static void CatArray(T* arr1, const T* const arr2, const char* divider = "\0", bool debug = 0)
        {
            int i = 0;
            int lenght = LenghtOf<T>(arr2);
            for (; arr1[i] != '\0'; i++);
            if (strcmp(divider, "\0"))
            {
                arr1[i] = divider[0];
                i++;
            }

            if (debug)
            {
                for (int t = 0; t < lenght; i++, t++)
                {
                    arr1[i] = arr2[t];
                    std::cout << "arr1[" << i << "] = " << arr1[i] << std::endl;
                }
                arr1[i] = '\0';
                std::cout << "arr1 = " << arr1 << std::endl;
                Pause();

            }
            else
            {
                for (int t = 0; t < lenght; i++, t++)
                {
                    arr1[i] = arr2[t];
                }
                arr1[i] = '\0';
            }
        }

    //Устанавливает нужную кодировку
    static void SetLocale()
        {
            SetConsoleCP(1251);
            SetConsoleOutputCP(1251);
        }

    //Заполняет массив заданным значением
    template <class T>
    static void MakeVoidArray(T* array, int size, T value = 0)
        {
            if (array != nullptr)
            {
                while (size >= 0)
                {
                    --size;
                    array[size] = value;
                }
            }

        }

    //Отражает массив зеркально
    template<class T>
    static void ReverseArray(T* a, int size)
    {
        T temp;
        for (int i = 0; i < size / 2; i++)
        {
            temp = a[i];
            a[i] = a[size - i - 1];
            a[size - i - 1] = temp;
        }
    }

    static long Random(int from, int to)
    {
        //srand(rand());
        return from + rand() % (to - from + 1);
    }

    static long Random(int to)
    {
        //srand(SDL_GetTicks());
        return rand() % ++to;
    }

    static int WaitFor(int seconds)
    {
        static int i = 0;
        Sleep(seconds * 1000);
        //std::cout << "i = " << i << std::endl;
        return 0;
    }
};

