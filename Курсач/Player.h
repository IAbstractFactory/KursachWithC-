#pragma once
#include"Time.h"
#include<Windows.h>
class Player
{
	int steps;
	Time time;
	wchar_t name[20];
public:
	Player() {}
	Player(wchar_t name[], int s, Time t)
	{
		for (int i = 0; i < 20; i++)
		{
			this->name[i] = name[i];
		}
		steps = s;
		time.milliseconds = t.milliseconds;
		time.seconds = t.seconds;
		time.minutes = t.minutes;
	}
	wchar_t* GetName()
	{
		return name;
	}
	Time GetTime()
	{
		return time;
	}
	int GetSteps()
	{
		return steps;
	}
	wchar_t* ToString()
	{

		wchar_t* playerToString = new wchar_t[50];

		wcscpy_s(playerToString, wcslen(playerToString), L"");

		wcscat_s(playerToString, wcslen(playerToString), name);
		wcscat_s(playerToString, wcslen(playerToString), L":    ");
		wcscat_s(playerToString, wcslen(playerToString), time.ToLPCWSTR());
		wcscat_s(playerToString, wcslen(playerToString), L"      :    ");
		wchar_t buff[10];
		wsprintf(buff, L"%d", steps);
		wcscat_s(playerToString, wcslen(playerToString), buff);

		return playerToString;
		//TODO: »—œ–¿¬‹ ›“Œ √Œ¬ÕŒ
	}


};

