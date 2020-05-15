#pragma once
#include<iostream>
#include<string.h>
#include <Windows.h>
class Time
{

public:
	int minutes = 0;
	int seconds = 0;
	int milliseconds = 0;
	Time() {}
	Time(int m, int s, int ms)
	{
		if (m >= 0 || m < 100)
			minutes = m;
		if (s >= 0 || s < 100)
			seconds = s;
		if (ms >= 0)
			milliseconds = ms;
	}
	wchar_t* ToLPCWSTR()
	{
		wchar_t* buf = new wchar_t[255];
		if (minutes <= 9)
			if (seconds <= 9)

				wsprintf(buf, L"0%d:0%d:%d", minutes, seconds, milliseconds);
			else
				wsprintf(buf, L"0%d:%d:%d", minutes, seconds, milliseconds);
		else
			if (seconds <= 9)
				wsprintf(buf, L"%d:0%d:%d", minutes, seconds, milliseconds);
			else
				wsprintf(buf, L"%d:%d:%d", minutes, seconds, milliseconds);

		return buf;
	}
};

