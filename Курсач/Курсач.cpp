// Курсач.cpp : Определяет точку входа для приложения.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

#include "framework.h"
#include "Курсач.h"

#include <thread>
#include"Time.h"
#include"Player.h"
#include<io.h>
#include <cassert>

#include<WinSock2.h>



#define ButtonOk_Click 1231
#define ShowPlayersButton_Click 2222
#define LIST_ID 1233
#define ClearPlayers 1435
#define RestartButton_Click 1446
#define CreateServer 1123
#define Button_Click 1000
#define ConnectToServer 3211
#define ButtonSecondField_Click 2323
#define ButtonFirstField_Click 2322

using namespace std;
HINSTANCE hInst;
int Counter = 0;
const char* FileName = "Players.txt";

class Timer
{
	Time time;

public:
	void Reset()
	{
		time.milliseconds = 0;
		time.minutes = 0;
		time.seconds = 0;
	}
	void Active()
	{
		if (time.milliseconds == 63)
		{
			if (time.seconds == 59)
			{
				if (time.minutes != 99)
				{
					time.minutes++;
				}
				time.seconds = 0;
			}
			else
				time.seconds++;
			time.milliseconds = 0;
		}
		else time.milliseconds++;
	}
	LPCWSTR ToString()
	{
		return time.ToLPCWSTR();
	}
	Time GetTime()
	{
		return time;
	}
};

Timer timer;
static HWND hListBox;
int width = 1000;//МОЖНО МЕНЯТЬ 
int height = 1000;//МОЖНО МЕНЯТЬ 
int ButtonSize = 100;//МОЖНО МЕНЯТЬ 
bool waiting = true;
bool GameWithFriend = false;
bool ButtonMove(int TrueRecX, int TrueRecY, LPARAM lParam, int& EmptySpaceX, int& EmptySpaceY);
HWND button[15];
HWND buttonFriends1[15];
HWND buttonFriends2[15];
int EmptySpaceXSolo;
int EmptySpaceYSolo;
int EmptySpaceX1, EmptySpaceX2;
int EmptySpaceY1, EmptySpaceY2;
bool FirstPlayerTurn = true;
bool DuoBlocked = false;
void CreateButtons(HWND hWnd, bool GameModeWithFriend)
{

	if (!GameModeWithFriend)
	{
		ButtonSize = 100;
		int w = width / 2 - 2 * ButtonSize;

		int h = height / 2 - 4 * ButtonSize;
		int k = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (!(i == 3 && j == 3))
				{
					wchar_t buffer[256];
					wsprintf(buffer, L"%d", k + 1);


					button[k] = CreateWindow(L"Button", buffer, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w + j * (ButtonSize + 1), h + i * (ButtonSize + 1), ButtonSize, ButtonSize, hWnd, reinterpret_cast<HMENU>(Button_Click), nullptr, nullptr);
					k++;
				}
				else
				{
					EmptySpaceXSolo = w + j * (ButtonSize + 1);
					EmptySpaceYSolo = h + i * (ButtonSize + 1);
				}
			}
		}
	}
	else
	{
		ButtonSize = 50;
		int w = width / 2 - 2 * ButtonSize;
		int h = height / 2 - 8 * ButtonSize;
		int k = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (!(i == 3 && j == 3))
				{
					wchar_t buffer[256];
					wsprintf(buffer, L"%d", k + 1);


					buttonFriends1[k] = CreateWindow(L"Button", buffer, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w + j * (ButtonSize + 1), h + i * (ButtonSize + 1), ButtonSize, ButtonSize, hWnd, reinterpret_cast<HMENU>(ButtonFirstField_Click), nullptr, nullptr);
					buttonFriends2[k] = CreateWindow(L"Button", buffer, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w + j * (ButtonSize + 1), h + ButtonSize * 6 + i * (ButtonSize + 1), ButtonSize, ButtonSize, hWnd, reinterpret_cast<HMENU>(ButtonSecondField_Click), nullptr, nullptr);
					k++;
				}
				else
				{
					EmptySpaceX1 = w + j * (ButtonSize + 1);
					EmptySpaceX2 = w + j * (ButtonSize + 1);
					EmptySpaceY1 = h + i * (ButtonSize + 1);
					EmptySpaceY2 = h + ButtonSize * 6 + i * (ButtonSize + 1);
				}
			}
		}

	}
}
class WinWindow
{
	const int thickness = 20;
	const int lenght = 200;

	HWND VictoryMessageLabel;
	HWND NameLabel;
	HWND NameTextBox;
	HWND okButton;
public:
	void Invoke(HWND hwnd)
	{

		wchar_t str[80];
		SendMessage(NameTextBox, WM_GETTEXT, 80, LPARAM(str)); //считали
		Player* player = new Player(str, Counter, timer.GetTime());
		MessageBox(hwnd, player->GetName(), player->GetTime().ToLPCWSTR(), MB_OK);
		FILE* stream;
		fopen_s(&stream, FileName, "ab+");
		fwrite(player, sizeof(Player), 1, stream);
		fclose(stream);
		ShowWindow(okButton, SW_HIDE);
		ShowWindow(NameTextBox, SW_HIDE);
		ShowWindow(NameLabel, SW_HIDE);
		ShowWindow(VictoryMessageLabel, SW_HIDE);
		//EnableWindow(okButton, false);
	}
	WinWindow(int height, int width, int buttonSize, HWND hWnd)
	{

		int x = width / 2;
		int y = height / 2 + ButtonSize;
		VictoryMessageLabel = CreateWindow(L"STATIC", L"ПОБЕДА!", WS_CHILD | WS_VISIBLE, x - lenght / 5, y, lenght / 3, thickness, hWnd, nullptr, nullptr, nullptr);

		NameLabel = CreateWindow(L"STATIC", L"Введите имя:", WS_CHILD | WS_VISIBLE, x - lenght, y + 50, lenght, thickness, hWnd, nullptr, nullptr, nullptr);

		NameTextBox = CreateWindow(L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL, x - lenght / 2, y + 50, lenght, thickness, hWnd, NULL, hInst, 0);

		okButton = CreateWindow(L"Button", L"Готово", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x - lenght / 2, y + thickness * 4, lenght, thickness, hWnd, reinterpret_cast<HMENU>(ButtonOk_Click), nullptr, nullptr);
	}
};

//#define ButtonOk_Click 1231
#define MAX_LOADSTRING 100

#define IDT_TIMER1 30130
// Глобальные переменные:
//HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	int k = 460; //разрешение экрана/2- width/2;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		k, 0, width, height, nullptr, nullptr, hInstance, nullptr);




	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

#pragma region Объявление переменных




HWND labelFirstPlayer;
HWND labelSecondPlayer;
bool GameStarted = false;
bool WinAcces = false;
bool victory = false;
bool firstClick = false;
bool answer = false;
static HWND labelCounter;
static HWND labelTimer;
HWND SHOW_PLAYERS_BUTTON;
HWND restartButton;
HWND clearButton;
HWND serverButton;
HWND clientButton;


HWND textBox;
wchar_t str[80];
WinWindow* winWindow;


#pragma endregion
void Shuffle(WPARAM wParam, LPARAM lParam)
{
	if (!GameWithFriend)
	{
		srand(time(NULL));
		int Iterations = rand() % 30 + 20; // Количество итераций
		this_thread::sleep_for(chrono::milliseconds(2000));
		GameStarted = true;
		for (int j = 0; j < Iterations; j++)
		{
			for (int i = 0; i < 15; i++)
			{
				SendMessage(button[i], BM_CLICK, wParam, lParam);
			}
		}
		WinAcces = true;
	}
	else
	{
		srand(time(NULL));
		int Iterations = rand() % 30 + 20; // Количество итераций
		this_thread::sleep_for(chrono::milliseconds(2000));
		GameStarted = true;
		for (int j = 0; j < Iterations; j++)
		{
			for (int i = 0; i < 15; i++)
			{
				SendMessage(buttonFriends1[i], BM_CLICK, wParam, lParam);
				SendMessage(buttonFriends2[i], BM_CLICK, wParam, lParam);
			}
		}
	}
}
bool Victory(HWND hWnd)
{
	if (!GameWithFriend)
	{
		int w = width / 2 - 2 * ButtonSize;
		int h = height / 2 - 4 * ButtonSize > 0 ? height / 2 - 4 * ButtonSize : 10;
		bool Vic = true;
		int k = 0;
		for (int i = 0; i < 4; i++)
		{
			if (!Vic)break;
			for (int j = 0; j < 4; j++)
			{
				if (!(j == 3 && i == 3))
				{
					RECT rectButton;
					RECT rectClientWindow;

					GetWindowRect(hWnd, &rectClientWindow);
					GetWindowRect(button[k], &rectButton);

					int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
					int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

					if (!(TrueRecX == w + j * (ButtonSize + 1) && TrueRecY == h + i * (ButtonSize + 1)))
					{
						Vic = false;
						break;
					}
					k++;
				}
			}
		}
		return Vic;
	}

	else
	{
		int w = width / 2 - 2 * ButtonSize;
		int h = height / 2 - 8 * ButtonSize;
		if (FirstPlayerTurn)
		{
			bool Vic = true;
			int k = 0;
			for (int i = 0; i < 4; i++)
			{
				if (!Vic)break;
				for (int j = 0; j < 4; j++)
				{
					if (!(j == 3 && i == 3))
					{
						RECT rectButton;
						RECT rectClientWindow;

						GetWindowRect(hWnd, &rectClientWindow);
						GetWindowRect(buttonFriends1[k], &rectButton);

						int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
						int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

						if (!(TrueRecX == w + j * (ButtonSize + 1) && TrueRecY == h + i * (ButtonSize + 1)))
						{
							Vic = false;
							break;
						}
						k++;
					}
				}
			}
			return Vic;
		}
		else
		{
			bool Vic = true;
			int k = 0;
			for (int i = 0; i < 4; i++)
			{
				if (!Vic)break;
				for (int j = 0; j < 4; j++)
				{
					if (!(j == 3 && i == 3))
					{
						RECT rectButton;
						RECT rectClientWindow;

						GetWindowRect(hWnd, &rectClientWindow);
						GetWindowRect(buttonFriends2[k], &rectButton);

						int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
						int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

						if (!(TrueRecX == w + j * (ButtonSize + 1) && TrueRecY == h + ButtonSize * 6 + i * (ButtonSize + 1)))
						{
							Vic = false;
							break;
						}
						k++;
					}
				}
			}
			return Vic;
		}
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:

		switch (wParam)
		{
		case IDT_TIMER1:
			if (WinAcces && !GameWithFriend && Victory(hWnd) && !victory)
			{

				victory = true;
				winWindow = new WinWindow(height, width, ButtonSize, hWnd);

			}
			if (firstClick && !victory)
				timer.Active();
			SetWindowText(labelTimer, timer.ToString());

			return 0;
		}
	case WM_CREATE:
	{

		SetTimer(hWnd, IDT_TIMER1, 1, (TIMERPROC)NULL);

		labelCounter = CreateWindow(L"STATIC", L"COUNT 0", WS_CHILD | WS_VISIBLE, 10, 10, 100, 20, hWnd, nullptr, nullptr, nullptr);
		labelTimer = CreateWindow(L"STATIC", L"TIME 0", WS_CHILD | WS_VISIBLE, 10, 30, 100, 20, hWnd, nullptr, nullptr, nullptr);

		int w = width / 2 - 2 * 100;
		int h = height / 2 - 4 * 100;
		serverButton = CreateWindow(L"Button", L"Игра с другом", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w / 2 - 140, h / 2 + 200, 150, 30, hWnd, reinterpret_cast<HMENU>(CreateServer), nullptr, nullptr);
		clearButton = CreateWindow(L"Button", L"Очистить список", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w / 2 - 140, h / 2 + 50, 150, 30, hWnd, reinterpret_cast<HMENU>(ClearPlayers), nullptr, nullptr);
		SHOW_PLAYERS_BUTTON = CreateWindow(L"Button", L"Показать игроков", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w / 2 - 140, h / 2 + 100, 150, 30, hWnd, reinterpret_cast<HMENU>(ShowPlayersButton_Click), nullptr, nullptr);
		restartButton = CreateWindow(L"Button", L"Начать новую игру", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, w / 2 - 140, h / 2 + 150, 150, 30, hWnd, reinterpret_cast<HMENU>(RestartButton_Click), nullptr, nullptr);
		CreateButtons(hWnd, 0);
		GameWithFriend = false;
		thread th(Shuffle, wParam, lParam);
		th.detach();
	}
	return 0;

	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;
		case Button_Click:
		{
			if (WinAcces && !victory)
			{
				firstClick = true;
				Counter++;
				wchar_t buffer[256];
				wsprintf(buffer, L"COUNT %d", Counter);
				SetWindowText(labelCounter, buffer);
			}
			if (GameStarted && !victory)
			{
				HWND h = (HWND)lParam;

				RECT rectButton;
				RECT rectClientWindow;

				GetWindowRect(hWnd, &rectClientWindow);
				GetWindowRect(h, &rectButton);

				int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
				int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

				ButtonMove(TrueRecX, TrueRecY, lParam, EmptySpaceXSolo, EmptySpaceYSolo);                   //  
				//else																	//
				//{																		// ЧТОБЫ ТАЙМЕР НЕ ПРИОСТАНАВЛИВАЛСЯ ВО ВРЕМЯ ДВИЖЕНИЯ КНОПКИ
				//	thread th(ButtonMove, TrueRecX, TrueRecY, lParam, EmptySpaceX, EmptySpaceY);					//
				//	th.detach();														//
				//}
			}
		}
		break;
		case ButtonOk_Click:
		{

			winWindow->Invoke(hWnd);
		}
		break;
		case ButtonFirstField_Click:
		{
			if (FirstPlayerTurn&&!DuoBlocked)
			{
				HWND h = (HWND)lParam;

				RECT rectButton;
				RECT rectClientWindow;

				GetWindowRect(hWnd, &rectClientWindow);
				GetWindowRect(h, &rectButton);

				int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
				int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

				if (ButtonMove(TrueRecX, TrueRecY, lParam, EmptySpaceX1, EmptySpaceY1))
				{
					if (Victory(hWnd))
					{
						MessageBox(NULL, L"Победил 1-й игрок", NULL, MB_OK);
						DuoBlocked = true;
					}
					FirstPlayerTurn = false;
				}

			}

		}
		break;
		case ButtonSecondField_Click:
		{
			if (!FirstPlayerTurn&& !DuoBlocked)
			{
				HWND h = (HWND)lParam;

				RECT rectButton;
				RECT rectClientWindow;

				GetWindowRect(hWnd, &rectClientWindow);
				GetWindowRect(h, &rectButton);

				int TrueRecX = abs(rectClientWindow.left - rectButton.left) - 8;
				int TrueRecY = abs(rectClientWindow.top - rectButton.top) - 50;

				if (ButtonMove(TrueRecX, TrueRecY, lParam, EmptySpaceX2, EmptySpaceY2))
				{
					if (Victory(hWnd))
					{
						MessageBox(NULL, L"Победил 2-й игрок", NULL, MB_OK);
						DuoBlocked = true;
					}
					FirstPlayerTurn = true;
				}


			}

		}
		break;
		case ClearPlayers:
		{
			FILE* f;
			fopen_s(&f, FileName, "wb+");
			fclose(f);
			MessageBox(hWnd, L"Очищено!", L"", MB_OK);

		}
		break;

		case RestartButton_Click:
		{
			DestroyWindow(labelFirstPlayer);
			DestroyWindow(labelSecondPlayer);

			GameStarted = false;
			WinAcces = false;
			victory = false;
			firstClick = false;
			Counter = 0;
			timer.Reset();
			SetWindowText(labelTimer, timer.ToString());

			wchar_t buffer[256];
			wsprintf(buffer, L"COUNT %d", Counter);
			SetWindowText(labelCounter, buffer);
			if (!GameWithFriend)
				for (int i = 0; i < 15; i++)
				{
					DestroyWindow(button[i]);
				}
			else
			{
				for (int i = 0; i < 15; i++)
				{
					DestroyWindow(buttonFriends1[i]);
					DestroyWindow(buttonFriends2[i]);
				}
			}
			CreateButtons(hWnd, 0);
			GameWithFriend = false;
			Shuffle(wParam, lParam);
		}
		break;
		case CreateServer:
		{
			Counter = 0;
			timer.Reset();
			SetWindowText(labelTimer, timer.ToString());

			wchar_t buffer[256];
			wsprintf(buffer, L"COUNT %d", Counter);
			SetWindowText(labelCounter, buffer);

			labelFirstPlayer = CreateWindow(L"STATIC", L"Первый игрок", WS_CHILD | WS_VISIBLE, 450, 50, 100, 20, hWnd, nullptr, nullptr, nullptr);
			labelSecondPlayer = CreateWindow(L"STATIC", L"Второй игрок", WS_CHILD | WS_VISIBLE, 450, 350, 100, 20, hWnd, nullptr, nullptr, nullptr);
			DuoBlocked = false;
			FirstPlayerTurn = true;
			if (!GameWithFriend)
				for (int i = 0; i < 15; i++)
					DestroyWindow(button[i]);
			else
				for (int i = 0; i < 15; i++)
				{
					DestroyWindow(buttonFriends1[i]);
					DestroyWindow(buttonFriends2[i]);
				}
			CreateButtons(hWnd, 1);
			GameWithFriend = true;
			Shuffle(wParam, lParam);
		}
		break;


		case ShowPlayersButton_Click:
		{
			FILE* f;
			fopen_s(&f, FileName, "ab+");
			fclose(f);

			int WIDTH = 500;
			int HEIGHT = 500;
			hListBox = CreateWindow(L"LISTBOX", NULL, WS_VISIBLE | LBS_STANDARD | WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST, 300, 400, WIDTH, HEIGHT, NULL, NULL, NULL, NULL); //CreateWindow(L"listbox", nullptr, WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT | WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, hWnd, nullptr, hInst, nullptr);

			FILE* stream;
			fopen_s(&stream, FileName, "rb+");
			int len = _filelength(_fileno(stream)) / sizeof(Player);

			for (int i = 0; i < len; i++)
			{
				Player player;
				fread(&player, sizeof(Player), 1, stream);
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)player.ToString());

			}
			fclose(stream);
		}
		break;


		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool ButtonMove(int TrueRecX, int TrueRecY, LPARAM lParam, int& EmptySpaceX, int& EmptySpaceY)
{
	bool result = false;
	if (abs(TrueRecY - EmptySpaceY) == ButtonSize + 1 && TrueRecX - EmptySpaceX == 0)
	{
		if (TrueRecY > EmptySpaceY)
			for (int i = TrueRecY; i >= EmptySpaceY; i--)
			{
				SetWindowPos((HWND)lParam, NULL, EmptySpaceX, i, 0, 0, SWP_NOSIZE);
				//Sleep(1);
			}
		else
		{
			for (int i = TrueRecY; i <= EmptySpaceY; i++)
			{
				SetWindowPos((HWND)lParam, NULL, EmptySpaceX, i, 0, 0, SWP_NOSIZE);
				//Sleep(1);
			}
		}


		EmptySpaceX = TrueRecX;
		EmptySpaceY = TrueRecY;
		result = true;
	}
	else if (abs(TrueRecX - EmptySpaceX) == ButtonSize + 1 && TrueRecY - EmptySpaceY == 0)
	{
		if (TrueRecX >= EmptySpaceX)
			for (int i = TrueRecX; i >= EmptySpaceX; i--)
			{
				SetWindowPos((HWND)lParam, NULL, i, EmptySpaceY, 0, 0, SWP_NOSIZE);
				//Sleep(1);
			}
		else
		{
			for (int i = TrueRecX; i <= EmptySpaceX; i++)
			{
				SetWindowPos((HWND)lParam, NULL, i, EmptySpaceY, 0, 0, SWP_NOSIZE);
				//Sleep(1);
			}
		}


		EmptySpaceX = TrueRecX;
		EmptySpaceY = TrueRecY;
		result = true;
	}
	return result;
}