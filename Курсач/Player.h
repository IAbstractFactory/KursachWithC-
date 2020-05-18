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


		wchar_t* buff = new wchar_t[30];
		wsprintf(buff, L"%ws / %ws / %d", time.ToLPCWSTR(), name, steps);


		return buff;

	}


};

