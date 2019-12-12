// CourseProject.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "CourseProject.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <OleCtl.h> 
#include <tchar.h> 
#include "atlstr.h"
#include <tlhelp32.h>
#include <iostream>
#include <regex>
#include <Windows.h>
#include <ctime>
//#include <omp.h>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
static HWND hRangeTextBox, luList, luParList, gaussParList, timeLU, timeLUParallel, timeGaussParallel, hThreadTextBox;
static SLAU slau;
double* xLu;
double* xLuPar;
double* xGaussPar;
static int n;
static int thread;
void Calculate();
bool CheckMatrix(SLAU slau, int n);

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
    LoadStringW(hInstance, IDC_COURSEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEPROJECT));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+2);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COURSEPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 900, 500, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//LPWSTR buf = NULL;
	
    switch (message)
    {
	case WM_CREATE:
	{
		setlocale(LC_ALL, "Russian");
		HWND button = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Прочитать из файла", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 360, 200, 150, 40, hWnd, (HMENU)1, GetModuleHandle(NULL), 0);
		HWND buttonRandom = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Случайные значения", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 360, 100, 150, 40, hWnd, (HMENU)2, GetModuleHandle(NULL), 0);
		HWND rangeText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("Размерность"), WS_CHILD | WS_VISIBLE, 500, 20, 100, 20, hWnd, NULL, NULL, NULL);
		hRangeTextBox = CreateWindowEx(NULL, TEXT("edit"), TEXT("3"), WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 40, 100, 20, hWnd, NULL, NULL, NULL);
		HWND threadText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("Кол-во потоков"), WS_CHILD | WS_VISIBLE, 360, 20, 120, 20, hWnd, NULL, NULL, NULL);
		hThreadTextBox = CreateWindowEx(NULL, TEXT("edit"), TEXT("8"), WS_VISIBLE | WS_CHILD | WS_BORDER, 360, 40, 120, 20, hWnd, NULL, NULL, NULL);
		HWND luText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("LU-разложение"), WS_CHILD | WS_VISIBLE, 20, 5, 150, 20, hWnd, NULL, NULL, NULL);
		luList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 20, 25, 150, 330, hWnd, NULL, NULL, NULL);
		HWND luParText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("LU-разложение пар."), WS_CHILD | WS_VISIBLE, 190, 5, 150, 20, hWnd, NULL, NULL, NULL);
		luParList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 190, 25, 150, 330, hWnd, NULL, NULL, NULL);
		HWND gaussParText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("Метод Гаусса пар."), WS_CHILD | WS_VISIBLE, 660, 5, 150, 20, hWnd, NULL, NULL, NULL);
		gaussParList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 660, 25, 150, 330, hWnd, NULL, NULL, NULL);
		HWND timeText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"), TEXT("Время выполнения"), WS_CHILD | WS_VISIBLE, 360, 360, 150, 20, hWnd, NULL, NULL, NULL);
		timeLU = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 20, 360, 150, 20, hWnd, NULL, NULL, NULL);
		timeLUParallel = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 190, 360, 150, 20, hWnd, NULL, NULL, NULL);
		timeGaussParallel = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 660, 360, 150, 20, hWnd, NULL, NULL, NULL);
		
	}
    case WM_COMMAND:
        {
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case 1:
		{
			SendMessage(luList, LB_RESETCONTENT, 0, 0);
			SendMessage(gaussParList, LB_RESETCONTENT, 0, 0);
			SendMessage(luParList, LB_RESETCONTENT, 0, 0);
			SendMessage(timeLU, LB_RESETCONTENT, 0, 0);
			SendMessage(timeLUParallel, LB_RESETCONTENT, 0, 0);
			SendMessage(timeGaussParallel, LB_RESETCONTENT, 0, 0);
			
			TCHAR* bufThr = new TCHAR[100];
			GetWindowText(hThreadTextBox, bufThr, 100);
			thread = _ttoi(bufThr);
			if (thread > 0) {
				ifstream in("input.txt");
				ifstream inB("inputB.txt");
				if (in.is_open())
				{
					int count = 0;// число чисел в файле
					int temp;

					while (!in.eof())
					{
						in >> temp;//в пустоту считываем из файла числа
						count++;
					}

					in.seekg(0, ios::beg);
					in.clear();

					int count_space = 0;
					char symbol;
					while (!in.eof())
					{
						in.get(symbol);//считали текущий символ
						if (symbol == ' ') count_space++;//Если это пробел, то число пробелов увеличиваем
						if (symbol == '\n') break;//Если дошли до конца строки, то выходим из цикла
					}

					in.seekg(0, ios::beg);
					in.clear();

					n = count / (count_space + 1);

					int m = count_space + 1;//число столбцов на единицу больше числа пробелов

					slau.A = new double* [n];
					for (int i = 0; i < n; i++) slau.A[i] = new double[m];

					for (int i = 0; i < n; i++)
						for (int j = 0; j < m; j++)
						{
							in >> temp;
							slau.A[i][j] = temp;
						}


					for (int i = 0; i < n; i++)
					{
						for (int j = 0; j < m; j++)
						{
							temp = slau.A[i][j];
						}
					}
					in.close();
				}
				if (inB.is_open())
				{
					int count = 0;
					int temp;

					while (!inB.eof())
					{
						inB >> temp;
						count++;
					}

					inB.seekg(0, ios::beg);
					inB.clear();

					int count_space = 0;
					char symbol;
					while (!inB.eof())
					{
						inB.get(symbol);
						if (symbol == ' ') count_space++;
						if (symbol == '\n') break;
					}

					inB.seekg(0, ios::beg);
					inB.clear();

					n = count / (count_space + 1);
					slau.B = new double[n];

					for (int i = 0; i < n; i++)
					{
						inB >> temp;
						slau.B[i] = temp;
					}

					for (int i = 0; i < n; i++)
					{
						temp = slau.B[i];
					}

					inB.close();


					if (!CheckMatrix(slau, n))
					{
						MessageBox(hWnd, L"Ошибка в матрице.", NULL, NULL);
						slau.A = NULL;
						slau.B = NULL;
						break;
					}
					MessageBox(hWnd, L"Данные получены.", L"Все хорошо", NULL);
					Calculate();
				}
				else
				{
					MessageBox(hWnd, L"Файл не найден.", NULL, NULL);
				}
			}
			else
			{
				MessageBox(hWnd, L"Вы ввели неверное число", NULL, NULL);
			}

		}

		break;
		case 2:
		{
			//bool checkRange = false;
			TCHAR* buf = new TCHAR[100];
			GetWindowText(hRangeTextBox, buf, 100);
			n = _ttoi(buf);
			TCHAR* bufThr = new TCHAR[100];
			GetWindowText(hThreadTextBox, bufThr, 100);
			thread = _ttoi(bufThr);
			/*for (int i = 0; i < sizeof(buf); i++)
				checkRange = isdigit(buf[i]);*/
			if (n > 0 && thread > 0) {
				

				slau.A = new double* [n];
				for (int i = 0; i < n; i++) slau.A[i] = new double[n];

				slau.B = new double[n];

				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						slau.A[i][j] = 0;
					}
					slau.B[i] = 0;
				}
				double a, b;
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						slau.A[i][j] = rand() % 20 + 1;
						a = slau.A[i][j];


					}
					slau.B[i] = rand() % 20 + 1;
					b = slau.B[i];
				}
				if (slau.A != NULL && slau.B != NULL)
				{
					SendMessage(luList, LB_RESETCONTENT, 0, 0);
					SendMessage(gaussParList, LB_RESETCONTENT, 0, 0);
					SendMessage(luParList, LB_RESETCONTENT, 0, 0);
					SendMessage(timeLU, LB_RESETCONTENT, 0, 0);
					SendMessage(timeLUParallel, LB_RESETCONTENT, 0, 0);
					SendMessage(timeGaussParallel, LB_RESETCONTENT, 0, 0);

					Calculate();
				}
			}
			else
			{
				MessageBox(hWnd, L"Вы ввели неверное число", NULL, NULL);
			}
		}
		break;
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
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
void Calculate() 
{
	TCHAR* buf = new TCHAR[100];
	GetWindowText(hRangeTextBox, buf, 100);
	n = _ttoi(buf);
	GetWindowText(hThreadTextBox, buf, 100);
	thread = _ttoi(buf);
	clock_t start_c_time = clock();
	xLu = Matrix::LU(slau, n);
	clock_t end_c_time = clock();
	double dt = (float)(end_c_time - start_c_time)/CLOCKS_PER_SEC;
	stringstream sstime;
	sstime << dt << endl;
	string newString = sstime.str();
	USES_CONVERSION;
	LPWSTR pstTime = A2T(newString.c_str());
	SendMessage(timeLU, LB_ADDSTRING, 0, (LPARAM)pstTime);

	start_c_time = end_c_time = dt = 0;
	start_c_time = clock();
	xLuPar = Matrix::LUPar(slau, n, thread);
	end_c_time = clock();
	dt = (float)(end_c_time - start_c_time) / CLOCKS_PER_SEC;
	stringstream sstime2;
	sstime2 << dt << endl;
	string newString1 = sstime2.str();
	pstTime = A2T(newString1.c_str());
	SendMessage(timeLUParallel, LB_ADDSTRING, 0, (LPARAM)pstTime);

	start_c_time = end_c_time = dt = 0;
	start_c_time = clock();
	xGaussPar = Matrix::GaussPar(slau, n, thread);
	end_c_time = clock();
	dt = (float)(end_c_time - start_c_time) / CLOCKS_PER_SEC;
	stringstream sstime3;
	sstime3 << dt << endl;
	newString = sstime3.str();
	pstTime = A2T(newString.c_str());
	SendMessage(timeGaussParallel, LB_ADDSTRING, 0, (LPARAM)pstTime);

	for (int i = 0; i < n; i++)
	{
		stringstream stringstream1, stringstream2, stringstream3;
		stringstream1 << floor(xGaussPar[i] * 100) / 100 << endl;
		string newString = stringstream1.str();
		USES_CONVERSION;
		LPWSTR pst = A2T(newString.c_str());

		SendMessage(luList, LB_ADDSTRING, 0, (LPARAM)pst);

		stringstream2 << floor(xGaussPar[i]*100)/100 << endl;
		newString = stringstream2.str();
		pst = A2T(newString.c_str());

		SendMessage(gaussParList, LB_ADDSTRING, 0, (LPARAM)pst);

		stringstream3 << floor(xGaussPar[i]*100)/100 << endl;
		newString = stringstream3.str();
		pst = A2T(newString.c_str());

		SendMessage(luParList, LB_ADDSTRING, 0, (LPARAM)pst);

	}

	for (int i = 0; i < n; i++) 
		delete[] slau.A[i];
	delete[] slau.A;
	delete[] slau.B;
}
bool CheckMatrix(SLAU slau, int n)//проверяем матрицу на нулевые значения
{
	int countZero = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			if (slau.A[i][j] == 0)
				countZero++;

		}
		if (countZero == n)
			return false;
		countZero = 0;
	}
	return true;
}

