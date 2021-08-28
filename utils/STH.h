#pragma once
#define WIDTH 800
#define HEIGHT 600
#define FONTSIZE 24
#define CHEATS 1
#define MINMINES 40//40
#define AVGMINES 95//90
#define MAXMINES 135//135
#define MAXUSERNAME 7
#define COUNTOFLEADERS 5
#define UTILITYPATH "utils/Utility.cpp"
#define FONT "fonts/ofont.ru_Arial Unicode MS.ttf"

#define LEADERMINFILENAME "leaders.min"
#define LEADERMAXFILENAME "leaders.avg"
#define LEADERAVGFILENAME "leaders.top"

#define SMILE "images/smiles/smile.png"
#define UNSMILE "images/smiles/unsmile.png"
#define NICE "images/smiles/nice.png"

#define WALL "images/Wall.jpg"
#define FRAME "images/frame.png"
#define MENUIMAGE "images/Menu.png"

#define MINE "images/mines and flags/bomb.png"
#define NOMINE "images/mines and flags/nobomb.png"
#define FLAG "images/mines and flags/flag.png"

#define TIMERBASEPATH "images/timer/timer"


namespace STH
{
	struct FileData
	{
		UINT8 userName[16];
		int result = 0;
		int lenght = 0;
		int trueLen = 0;

		FileData() {};

		FileData(const UINT8* name, int res)
		{
			result = res;
			int i = 0;
			for (; name[i] != '\0'; i++)
			{
				lenght++;
				this->userName[i] = name[i];
			}
			this->userName[i] = '\0';
		}

		~FileData()
		{

		}

		char GetLen()
		{
			return lenght;
		}

		bool operator<(const FileData& data)
		{
			return this->result < data.result;
		}

		FileData operator=(const FileData& data)
		{
			int i = 0;
			for (; data.userName[i] != '\0'; i++)
			{
				this->userName[i] = data.userName[i];
			}
			this->userName[i] = '\0';
			this->result = data.result;
			this->lenght = data.lenght;
			this->trueLen = data.trueLen;
			return FileData(this->userName, this->result);
		}
	};
}



