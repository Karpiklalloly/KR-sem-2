#pragma once
#include "CommonWindow.h"



class GameWindow :
    public CommonWindow
{
private:
    static GameWindow* game;

    SDL_Texture* ren; //����� ��������
    SDL_Texture* inputName;
    SDL_Rect frame;
    SDL_Rect inputtingText;
    SDL_Texture* wally;
    SDL_Rect forWall;
    SDL_Rect forMark;
    SDL_Texture* mark;

    //������ �������� ��� ���� �� �������
    const char* timerPath = TIMERBASEPATH;
    SDL_Surface** timerSurface = new SDL_Surface*[10];
    SDL_Texture** timerTexture = new SDL_Texture*[10];

    texts* text;

    //��������� �������� ������� 0, ���
    int timer = 0;

    //���������� ������� ������ �� ����, ����� ������ �� ��� ����������� � ��������� �������
    int startOfFieledButtons;
    int endOfFieldButtons;

    int startOfDif;
    int endOfDif;

    //��� ���������
    int countOfMines = MAXMINES;
    int countOfMinesAtStart = MAXMINES;
    const int sizeOfFieldButton = 24;
    const int countPerLine = 30;
    const int countPerColumn = 16;
    const int deltaX = 40;
    const int deltaY = 100;

    //����� ���� ����������, ����� ����� ��� 0
    int countNeedToUnlock = 0;

    //��������� ����������, ����� ��������, ����� ����� ��������, � ����� ������ ���������
    bool canChange = true;
    bool started = false;

    bool showInput = false;

    GameWindow() {};

    //���������� ����; ����� �� ������ ������
    GameWindow(const char* name, int w, int h)
    {
        //������� ���� � ��������� � ���
        if (window != NULL)
        {
            SDL_DestroyWindow(window);
        }
            
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
        if (surface != NULL)
            SDL_FreeSurface(surface);
        surface = SDL_GetWindowSurface(window);
        if (!window || !surface)
            printf("������");
        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL) printf("������ �������");

        //������� ����
        widght = w;
        height = h;

        //������ ������� 
        forWall.x = 0;
        forWall.y = 0;
        forWall.w = widght;
        forWall.h = height;

        inputtingText = { widght / 2 - 160, height / 2 - 30, 0, 60 };
        frame = { widght / 2 - 280, height / 2 - 90, 560, 180 };

        //������� ������ � ����
        AddButton<ButtonToMenu>(Utility::Point(10, 10), 60, 30, 255, 100, 100);
        AddButton<RefreshButton>(Utility::Point(widght / 2 - sizeOfFieldButton / 2, 50), sizeOfFieldButton, sizeOfFieldButton, 120, 120, 120);
        AddDifButtons();
        AddFieldButtons();

        //��������� ����� �� ������
        int we = 50;
        int he = 20;
        int xe = 15;
        int ye = 15;

        text = new texts();
        Utility::CatArray(text[0].text, (UINT16*)L"����");
        text[0].rect.x = xe;
        text[0].rect.y = ye;
        text[0].rect.w = we;
        text[0].rect.h = he;
        text[0].surface = TTF_RenderUNICODE_Blended(font2, text[0].text, text[0].color);
        text[0].texture = SDL_CreateTextureFromSurface(renderer, text[0].surface);

        //���������, ��� ��� ������ �������
        if (text[0].surface == NULL)
        {
            perror(TTF_GetError());
        }

        //���������� �������� ������ � ���������� � ��������
        SDL_Surface* wall;
        wall = IMG_Load(WALL);
        wally = SDL_CreateTextureFromSurface(renderer, wall);
        SDL_FreeSurface(wall);

        //���������� � �������� �������� � ��������� ��� �������
        char* path = new char[16];
        for (int i = 0; i < 10; i++)
        {
            path[0] = '\0';
            Utility::CatArray(path, timerPath);
            _itoa(i, path + Utility::LenghtOf(timerPath), 10);
            Utility::CatArray(path, ".png");
            timerSurface[i] = IMG_Load(path);
            if (timerSurface[i] == NULL)
            {
                perror("�������� � �������� �� ������������\n");
            }
            timerTexture[i] = SDL_CreateTextureFromSurface(renderer, timerSurface[i]);
            if (timerTexture[i] == NULL)
            {
                perror("������ �� �����������!\n");
            }
        }

        //���������� �������� ��� ����������
        SDL_Surface* marky;
        marky = IMG_Load(MINE);
        mark = SDL_CreateTextureFromSurface(renderer, marky);
        SDL_FreeSurface(marky);
        if (mark == NULL)
        {
            perror(SDL_GetError());
        }

        SDL_Surface* sur = IMG_Load(FRAME);
        ren = SDL_CreateTextureFromSurface(renderer, sur);
        SDL_FreeSurface(sur);
    }

    ~GameWindow() override
    {
        game = NULL;
    }

    //������, ����� ���������� �� ���������
    void AddFieldButtons()
    {
        startOfFieledButtons = GetCountOfButtons();
        for (int i = 0; i < countPerLine; i++)
        {
            for (int j = 0; j < countPerColumn; j++)
            {
                AddButton<FieldButton>(Utility::Point(deltaX + sizeOfFieldButton * i, deltaY + sizeOfFieldButton * j), sizeOfFieldButton, sizeOfFieldButton, 100, 100, 100);
            }
        }
        endOfFieldButtons = GetCountOfButtons() - 1;
    }

    //������, ����� ���������� �� ���������
    void AddDifButtons()
    {
        startOfDif = GetCountOfButtons();
        AddButton<DifficultyButton>(Utility::Point(widght - 16 * sizeOfFieldButton / 2, 50), sizeOfFieldButton, sizeOfFieldButton, 0, 255, 0);
        AddButton<DifficultyButton>(Utility::Point(widght - 12 * sizeOfFieldButton / 2, 50), sizeOfFieldButton, sizeOfFieldButton, 255, 255, 100);
        AddButton<DifficultyButton>(Utility::Point(widght - 8 * sizeOfFieldButton / 2, 50), sizeOfFieldButton, sizeOfFieldButton, 255, 0, 0);
        endOfDif = GetCountOfButtons();
        RefreshDif(GetButtons()[startOfDif + 1]);//�� ��������� ������� ������� ���������
    }

    //������������ ������
    void DrawTimer(SDL_Renderer* renderer)
    {
        //��������� �������������, �.�. �� ��� �������
        SDL_Rect rect{};
        rect.x = 250;
        rect.y = 50;
        rect.w = 21;
        rect.h = 21;
        int tempTimer = timer;
        int i = 4;
        while (tempTimer > 0 || i>0)
        {
            SDL_RenderCopy(renderer, timerTexture[tempTimer % 10], NULL, &rect);
            rect.x -= 11;
            tempTimer = tempTimer / 10;
            i--;
        }
    }

    //��������� ������
    void DrawingText(SDL_Renderer* renderer) override
    {
        for (int i = 0; i < 1; i++)
        {
            SDL_RenderCopy(renderer, text[i].texture, NULL, &text[i].rect);
        }

    }

    //������ ���, � ������ � ��������� ������
    void InputName(SDL_Renderer* renderer)
    {
        STH::FileData data;
        int leng = 0;

        bool ascii[MAXUSERNAME+1]{};
        for (int k = 0; k < MAXUSERNAME+1; k++)
        {
            ascii[k] = true;
        }
        data.result = timer;
        inputtingText.w = 0;
        std::basic_string<UINT8> name;
        while (name.length() > 0)
        {
            name.pop_back();
        }

        showInput = true;
        bool needR = true;
        bool needToBreak = false;
        SDL_StartTextInput();
        while (1)
        {
            //���� �������
            while (SDL_PollEvent(&event))
            {
                //���� ������ �� ��������
                if (event.type == SDL_TEXTINPUT)
                {
                    if (leng < MAXUSERNAME)
                    {
                        if (Utility::LenghtOf(event.text.text) == 1)
                        {
                            leng++;
                            name += (UINT8)event.text.text[0];
                        }
                        else if (Utility::LenghtOf(event.text.text) == 2)
                        {
                         
                            leng ++;
                            name += (UINT8)event.text.text[0];
                            name += (UINT8)event.text.text[1];
                            ascii[leng] = false;
                        }
                        
                        inputtingText.w += FONTSIZE;
                        needR = true;
                    }
                }

                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        if (leng > 0)
                        {
                            if (!ascii[leng])
                            {
                                ascii[leng] = true;
                                name.pop_back();
                            }
                            leng--;
                            name.pop_back();
                            inputtingText.w -= FONTSIZE;
                            needR = true;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        std::cout << "Enter" << std::endl;
                        if (leng > 0)
                        {
                            needToBreak = true;
                            break;
                        }
                        
                    }
                }
                
                    
            }
            if (needToBreak)
                break;
            //std::cout << name.length() << std::endl;
            if (needR)
            {
                inputName = SDL_CreateTextureFromSurface(renderer, TTF_RenderUTF8_Blended(font2, (char*)name.c_str(), text[0].color));
                Update();
                needR = false;
            }
            
        }
        SDL_StopTextInput();
        showInput = false;
        unsigned int i = 0;
        for (; i < name.length(); i++)
        {
            data.userName[i] = name[i];
        }
        data.lenght = name.length();
        data.trueLen = leng;
        std::cout << "leng " << (int)data.lenght << std::endl;
        std::fstream f(countOfMinesAtStart==MAXMINES? LEADERMAXFILENAME : countOfMinesAtStart == MINMINES? LEADERMINFILENAME : LEADERAVGFILENAME, std::ios::binary | std::ios::in);
        STH::FileData* datas = new STH::FileData[COUNTOFLEADERS];
        i = 0;
        //���������
        for (; i < COUNTOFLEADERS; i++)
        {
            if (!f.read(reinterpret_cast<char*>(&datas[i]), sizeof(datas[i])))
            {
                for (; i < COUNTOFLEADERS; i++)
                {
                    datas[i].result = 12321321;
                    datas[i].userName[0] = 0;
                    datas[i].lenght = 0;
                }
            }
        }
        //���������
        for (int t = 0; t < COUNTOFLEADERS; t++)
        {
            if (data < datas[t])
            {
                for (int k = COUNTOFLEADERS-1; k > t; k--)
                {
                    datas[k] = datas[k - 1];
                }
                datas[t] = data;
                break;
            }
        }
        f.close();

        f.open(countOfMinesAtStart <= MINMINES ? LEADERMINFILENAME : countOfMinesAtStart <= AVGMINES ? LEADERAVGFILENAME : LEADERMAXFILENAME, std::ios::binary | std::ios::out);
        i = 0;
        //����������
        for (; i < COUNTOFLEADERS; i++)
        {
            if (!f.write(reinterpret_cast<char*>(&datas[i]), sizeof(datas[i])))
            {
                for (; i < COUNTOFLEADERS; i++)
                {
                    datas[i].result = 12321321;
                    datas[i].userName[0] = 0;
                    datas[i].lenght = 0;
                }
            }
        }
        f.close();
        delete[] datas;
    }

    //��������� �� ������ ������ ���������
    void DrawMark(SDL_Renderer* renderer)
    {
        for (int i = startOfDif; i < endOfDif; i++)
        {
            if (((DifficultyButton*)GetButtons()[i])->GetState())
            {
                SDL_RenderCopy(renderer, mark, NULL, &forMark);
                //std::cout << countOfMines << std::endl;
                break;
            }
        }
    }

public:
    static GameWindow* Get(const char* name, int w, int h)
    {
        if (!game)
        {
            game = new GameWindow(u8"�����", w, h);
        }
        return game;
    }

    virtual void Update() override
    {
        SDL_RenderClear(renderer);

        //��������� �������
        SDL_RenderCopy(renderer, wally, NULL, &forWall);

        //��������� �������
        DrawTimer(renderer);

        //��������� ������
        for (int i = 0; i < GetCountOfButtons(); i++)
        {
            GetButtons()[i]->Draw(renderer);
        }

        //��������� �������
        DrawMark(renderer);

        //��������� ������
        DrawingText(renderer);

        if (showInput)
        {
            SDL_RenderCopy(renderer, ren, NULL, &frame);
            SDL_RenderCopy(renderer, inputName, NULL, &inputtingText);
        }

        SDL_RenderPresent(renderer);
    }

    //����������� �������� ������� �� 1
    void UpdateTimer()
    {
        timer++;
    }

    //���������� ���� � ������ NULL static ����������
    void Destroy() override
    {
        SDL_DestroyTexture(ren);
        SDL_DestroyTexture(wally);
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        TTF_CloseFont(font2);
        SDL_DestroyWindow(window);
        for (int i = 0; i < 10; i++)
        {
            SDL_FreeSurface(timerSurface[i]);
            SDL_DestroyTexture(timerTexture[i]);
        }
        delete text;
        delete[] buttonStruct.buttons;
        timer = 0;
        game = NULL;
    }

    //��������� ���
    void SetMines(Button* button)
    {
        int number = 0;
        countOfMinesAtStart = countOfMines;
        int t = 0;
        for (int i = 0; i < countOfMines; i++)
        {
            do
            {
                //��������, ��� ���� �����
                number = Utility::Random(startOfFieledButtons, endOfFieldButtons);
            } while (GetButtons()[number] == button || ((FieldButton*)GetButtons()[number])->HasMine());
            ((FieldButton*)GetButtons()[number])->MakeMine();
            t++;
        }
        countNeedToUnlock = 1;
        
        for (int i = startOfFieledButtons; i < endOfFieldButtons; i++)
        {
            if (!((FieldButton*)GetButtons()[i])->HasMine())
            {
                countNeedToUnlock++;
            }
            else t--;
        }
        std::cout << "t = " << t << std::endl;
        for (; t > 0; t--)
        {
            countNeedToUnlock--;
        }
        //������� �������� ������ �� ������� ���
        int countOfNeighMines = 0;
        bool leftChecked = false;
        bool rightChecked = false;
        for (int i = 0; i < countPerLine; i++)
        {
            for (int j = 0; j < countPerColumn; j++)
            {
                countOfNeighMines = 0;
                leftChecked = false;
                rightChecked = false;
                if (!((FieldButton*)GetButtons()[startOfFieledButtons + i * countPerColumn + j])->HasMine())
                {
                    if (j > 0)//�� � ������� ����
                    {
                        if (((FieldButton*)GetButtons()[startOfFieledButtons + i * countPerColumn + j - 1])->HasMine())//������ �� ������
                        {
                            countOfNeighMines++;
                        }
                        if (i > 0)//�� � ����� ����
                        {
                            
                            if (((FieldButton*)GetButtons()[startOfFieledButtons + (i - 1) * countPerColumn + j])->HasMine())//����� �� ������
                            {
                                countOfNeighMines++;
                                leftChecked = true;
                            }
                            if (((FieldButton*)GetButtons()[startOfFieledButtons + (i - 1) * countPerColumn + j - 1])->HasMine())//������ ����� �� ������
                            {
                                countOfNeighMines++;
                            }
                        }
                        if (i < countPerLine - 1)//�� � ������ ����
                        {
                            if (((FieldButton*)GetButtons()[startOfFieledButtons + (i + 1) * countPerColumn + j])->HasMine())//������ �� ������
                            {
                                countOfNeighMines++;
                                rightChecked = true;
                            }
                            if (((FieldButton*)GetButtons()[startOfFieledButtons + (i + 1) * countPerColumn + j - 1])->HasMine())//������ ������ �� ������
                            {
                                countOfNeighMines++;
                            }
                        }
                    }
                    if (j < countPerColumn - 1)//�� � ������ ����
                    {
                        if ((((FieldButton*)GetButtons()[startOfFieledButtons + i * countPerColumn + j + 1])->HasMine()))//����� �� ������
                        {
                            countOfNeighMines++;
                        }
                        if (i > 0)//�� � ����� ����
                        {
                            if (!leftChecked && ((FieldButton*)GetButtons()[startOfFieledButtons + (i - 1) * countPerColumn + j])->HasMine())//����� �� ������
                            {
                                countOfNeighMines++;
                            }

                            if (((FieldButton*)GetButtons()[startOfFieledButtons + (i - 1) * countPerColumn + j + 1])->HasMine())//����� ����� �� ������
                            {
                                countOfNeighMines++;
                            }
                        }
                        if (i < countPerLine - 1)//�� � ������ ����
                        {
                            if ((((FieldButton*)GetButtons()[startOfFieledButtons + (i + 1) * countPerColumn + j + 1])->HasMine()))//����� ������ �� ������
                            {
                                countOfNeighMines++;
                            }

                            if (!rightChecked && ((FieldButton*)GetButtons()[startOfFieledButtons + (i + 1) * countPerColumn + j])->HasMine())//������ �� ������
                            {
                                countOfNeighMines++;
                            }
                        }
                    }
                    ((FieldButton*)GetButtons()[startOfFieledButtons + i * countPerColumn + j])->SetCountMines(countOfNeighMines);

                }
                
                
            }
        }

    }

    void GameLost(Button* button)
    {
        canChange = false;
        for (int i = startOfFieledButtons; i < endOfFieldButtons; i++)
        {
            if (((FieldButton*)GetButtons()[i])->HasMine())
            {
                ((FieldButton*)GetButtons()[i])->SetPath(((FieldButton*)GetButtons()[i])->GetMinePath());
            }
        }

        for (int i = startOfFieledButtons; i < endOfFieldButtons; i++)
        {
            if (((FieldButton*)GetButtons()[i])->HasFlag() && !((FieldButton*)GetButtons()[i])->HasMine())
            {
                ((FieldButton*)GetButtons()[i])->SetPath(((FieldButton*)GetButtons()[i])->GetNoMinePath());
            }
        }
        ((RefreshButton*)GetButtons()[1])->SetState(1);
        ((FieldButton*)(button))->ChangeColor(button->GetColor().r + 50, button->GetColor().g, button->GetColor().b);
    }

    void GameWon()
    {
        canChange = false;
        for (int i = startOfFieledButtons; i < endOfFieldButtons; i++)
        {
            if (((FieldButton*)GetButtons()[i])->HasMine())
            {
                ((FieldButton*)GetButtons()[i])->SetPath(((FieldButton*)GetButtons()[i])->GetMinePath());
            }
        }
        ((RefreshButton*)GetButtons()[1])->SetState(2);
        InputName(renderer);
    }

    //��������� ����
    void Refresh()
    {
        canChange = true;
        for (int i = startOfFieledButtons; i < endOfFieldButtons + 1; i++)
        {
            ((FieldButton*)GetButtons()[i])->Refresh();
        }
        ((RefreshButton*)GetButtons()[1])->SetState(0);
        started = false;
        timer = 0;
    }

    //���� ���� - false
    bool Open(Button* button)
    {
        countNeedToUnlock--;
        return ((FieldButton*)button)->Open();
    }

    //����� �� ��������
    bool CanChange()
    {
        return canChange;
    }

    bool Started()
    {
        return started;
    }

    void SetStart()
    {
        started = true;
    }

    void OpenNeigh(Button* button)
    {
        bool hasNumber = false;
        bool needToBreak = false;
        bool leftChecked = false;
        bool rightChecked = false;
        int temp = 0;
        for (int i = 0; i < countPerLine; i++)
        {
            for (int j = 0; j < countPerColumn; j++)
            {
                //printf("i=%d j=%d\n", i, j);
                if (button == GetButtons()[startOfFieledButtons + i * countPerColumn + j])
                {
                    if (!((FieldButton*)button)->IsOpened())
                    {
                        Open(button);
                    }

                    if (!((FieldButton*)button)->IsOpened() || ((FieldButton*)button)->GetCountMines())
                    {
                        needToBreak = true;
                        break;
                    }

                    leftChecked = false;
                    rightChecked = false;


                    if (j > 0)//�� � ������� ����
                    {
                        temp = startOfFieledButtons + i * countPerColumn + j - 1;
                        if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                        if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//������ �� ������
                        {
                            if (!((FieldButton*)GetButtons()[temp])->HasMine())
                            {
                                Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                        else if (!hasNumber)
                        {
                            //Open(GetButtons()[temp]);
                            OpenNeigh(GetButtons()[temp]);
                        }
                        
                        if (i > 0)//�� � ����� ����
                        {
                            temp = startOfFieledButtons + (i - 1) * countPerColumn + j;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//����� �� ������
                            {
                                leftChecked = true;
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                            }
                            else if (!hasNumber)
                            {
                                leftChecked = true;
                               // Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }

                            temp = startOfFieledButtons + (i - 1) * countPerColumn + j - 1;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//������ ����� �� ������
                            {
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                            }
                            else if (!hasNumber)
                            {
                                //Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                        if (i < countPerLine - 1)//�� � ������ ����
                        {
                            temp = startOfFieledButtons + (i + 1) * countPerColumn + j;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//������ �� ������
                            {
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                                rightChecked = true;
                            }
                            else if (!hasNumber)
                            {
                                rightChecked = true;
                                //Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }

                            temp = startOfFieledButtons + (i + 1) * countPerColumn + j - 1;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//������ ������ �� ������
                            {
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    //Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                            }
                            else if (!hasNumber)
                            {
                               // Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                    }
                    if (j < countPerColumn - 1)//�� � ������ ����
                    {
                        temp = startOfFieledButtons + i * countPerColumn + j + 1;
                        if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                        if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//����� �� ������
                        {
                            if (!((FieldButton*)GetButtons()[temp])->HasMine())
                            {
                                Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                        else if (!hasNumber)
                        {
                            Open(GetButtons()[temp]);
                            OpenNeigh(GetButtons()[temp]);
                        }

                        if (i > 0)//�� � ����� ����
                        {
                            temp = startOfFieledButtons + (i - 1) * countPerColumn + j;
                            if (!leftChecked && !((FieldButton*)GetButtons()[temp])->IsOpened())
                                if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//����� �� ������
                                {
                                    if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                    {
                                        Open(GetButtons()[temp]);
                                        OpenNeigh(GetButtons()[temp]);
                                    }
                                }
                                else if (!hasNumber)
                                {
                                    //Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }

                            temp = startOfFieledButtons + (i - 1) * countPerColumn + j + 1;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//����� ����� �� ������
                            {
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                            }
                            else if (!hasNumber)
                            {
                                //Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                        if (i < countPerLine - 1)//�� � ������ ����
                        {
                            temp = startOfFieledButtons + (i + 1) * countPerColumn + j;
                            if (!rightChecked && !((FieldButton*)GetButtons()[temp])->IsOpened())
                                if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//������ �� ������
                                {
                                    if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                    {
                                        Open(GetButtons()[temp]);
                                        OpenNeigh(GetButtons()[temp]);
                                    }
                                }
                                else if (!hasNumber)
                                {
                                    //Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }

                            temp = startOfFieledButtons + (i + 1) * countPerColumn + j + 1;
                            if (!((FieldButton*)GetButtons()[temp])->IsOpened())
                            if (((FieldButton*)GetButtons()[temp])->GetCountMines() == 0 && !hasNumber)//����� ������ �� ������
                            {
                                if (!((FieldButton*)GetButtons()[temp])->HasMine())
                                {
                                    Open(GetButtons()[temp]);
                                    OpenNeigh(GetButtons()[temp]);
                                }
                            }
                            else if (!hasNumber)
                            {
                                //Open(GetButtons()[temp]);
                                OpenNeigh(GetButtons()[temp]);
                            }
                        }
                    }
                    needToBreak = true;
                    break;
                }
            }
            if (needToBreak)
                break;
        }
    }

    int GetCountToUnclock()
    {
        return countNeedToUnlock;
    }

    void RefreshDif(Button* button)
    {
        int t = 0;
        for (int i = startOfDif; i < endOfDif; i++)
        {
            t++;
            ((DifficultyButton*)GetButtons()[i])->SetState(false);
            if (GetButtons()[i] == button)
            {
                ((DifficultyButton*)GetButtons()[i])->SetState(true);
                forMark = *((DifficultyButton*)GetButtons()[i])->GetRect();
                switch (t)
                {
                case 1: //�����
                    countOfMines = MINMINES;
                    break;
                case 2: //���������
                    countOfMines = AVGMINES;
                    break;
                case 3://������
                    countOfMines = MAXMINES;
                    break;
                }
            }
        }
    }
};

GameWindow* GameWindow::game = 0;

