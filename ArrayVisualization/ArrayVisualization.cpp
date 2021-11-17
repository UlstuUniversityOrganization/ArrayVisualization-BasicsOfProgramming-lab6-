#include "framework.h"
#include "ArrayVisualization.h"
#include <math.h>
#include <stdio.h>

#define MAX_LOADSTRING 100

enum class Task
{
	A = 0,
	B = 1,
	C = 2,
	D = 3,
	Arbitrary = 4
};

Task task = Task::D;

// Глобальные переменные:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

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
	LoadStringW(hInstance, IDC_ARRAYVISUALIZATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARRAYVISUALIZATION));

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARRAYVISUALIZATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ARRAYVISUALIZATION);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}



void DrawTriangle(HDC hdc, POINT v1, POINT v2, POINT v3)
{
	POINT point[3] =
	{
		v1.x, v1.y,
		v2.x, v2.y,
		v3.x, v3.y
	};
	Polygon(hdc, point, 3);
}

void DrawAxies(HDC hdc, int left, int top, int right, int bottom, int offset = 0)
{
	HBRUSH triangleBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, triangleBrush);


	MoveToEx(hdc, left, top - offset, NULL);
	LineTo(hdc, left, bottom);
	LineTo(hdc, right + offset, bottom);

	int size = 7;
	int pickSize = 3;
	DrawTriangle(hdc, { left - size, top - offset }, { left, top - size * pickSize - offset }, { left + size, top - offset });
	DrawTriangle(hdc, { right + offset, bottom - size }, { right + size * pickSize + offset, bottom }, { right + offset, bottom + size });
}



void PlotGraph(HDC hdc, int left, int top, int right, int bottom, int* num, int elementsCount, int minID = -1, int maxID = -1)
{
	DrawAxies(hdc, left, top, right, bottom, 20);

	float width = (float)(right - left);
	float height = (float)(bottom - top);
	float elementWidht = width / elementsCount;

	HBRUSH positiveBrush = CreateSolidBrush(RGB(100, 100, 255));
	HBRUSH negativeBrush = CreateSolidBrush(RGB(255, 100, 100));


	HBRUSH minBrush = CreateSolidBrush(RGB(100, 255, 100));
	HBRUSH maxBrush = CreateSolidBrush(RGB(100, 255, 100));

	for (int i = 0; i < elementsCount; i++)
	{
		if (i != minID && i != maxID)
		{
			if (num[i] > 0)
				SelectObject(hdc, positiveBrush);
			else if (num[i] < 0)
				SelectObject(hdc, negativeBrush);
		}
		else if (i == minID)
			SelectObject(hdc, minBrush);
		else if (i == maxID)
			SelectObject(hdc, maxBrush);

		Rectangle(hdc, left + i * elementWidht, bottom - (num[i] / 20.0f) * (bottom - top), left + (i + 1) * elementWidht, bottom);
	}

}

void TaskA(int* array, int elementsCount)
{
	for (int i = 0; i < elementsCount; i++)
		array[i] = array[i] * -1;
}

void TaskB(int* array, int elementsCount, int* minID, int* maxID)
{
	int min = array[0];
	int max = array[0];

	int minIDLocal = 0;
	int maxIDLocal = 0;
	for (int i = 0; i < elementsCount; i++)
	{
		if (array[i] < min)
		{
			min = array[i];
			minIDLocal = i;
		}
		if (array[i] > max)
		{
			max = array[i];
			maxIDLocal = i;
		}
	}
	*minID = minIDLocal;
	*maxID = maxIDLocal;
}

void TaskC(int* array, int elementsCount, int** newArray, int* newElementsCount)
{
	int min = array[0];
	int max = array[0];

	int minId = -1;
	int maxId = -1;

	for (int i = 0; i < elementsCount; i++)
	{
		if (array[i] < min)
		{
			min = array[i];
			minId = i;
		}

		if (array[i] > max)
		{
			max = array[i];
			maxId = i;
		}
	}

	*newElementsCount = 0;
	for (int i = minId + 1; i < maxId; i++)
		if ((array[i] % 2) == 0)
			++* newElementsCount;

	*newArray = new int[*newElementsCount];
	int j = 0;
	for (int i = minId + 1; i < maxId; i++)
		if ((array[i] % 2) == 0)
		{
			(*newArray)[j] = array[i];
			j++;
		}

}

void TaskD(int* array, int elementsCount, int** newArray, int* newElementsCount)
{
	int min = array[0];
	int max = array[0];

	int minId = 0;
	int maxId = 0;

	for (int i = 0; i < elementsCount; i++)
	{
		if (array[i] < min)
		{
			min = array[i];
			minId = i;
		}

		if (array[i] > max)
		{
			max = array[i];
			maxId = i;
		}
	}

	*newElementsCount = 0;
	for (int i = minId + 1; i < maxId; i++)
		if ((array[i] % 2) != 0)
			++* newElementsCount;
	if (maxId >= minId)
		*newElementsCount += elementsCount - ((maxId - 1) - minId);
	else
		*newElementsCount += elementsCount - ((minId - 1) - maxId);


	*newArray = new int[*newElementsCount];

	if (minId >= maxId)
	{
		int tempNum = minId;
		minId = maxId;
		maxId = tempNum;
	}

	int j = 0;
	for (int i = 0; i < elementsCount; i++)
	{
		if (i > minId && i < maxId)
		{
			if ((array[i] % 2) != 0)
			{
				(*newArray)[j] = array[i];
				j++;
			}
		}
		else
		{
			(*newArray)[j] = array[i];
			j++;
		}
	}
}

int* num;
int elementsCount = 0;
int* num2;
int elementsCount2 = 0;
int minID = -1;
int maxID = -1;



void GetArrayFromFile(int** array, int* elementsCount)
{
	FILE* filePointer;
	fopen_s(&filePointer, "data.txt", "r");

	fscanf_s(filePointer, "%d", elementsCount);
	(*array) = new int[*elementsCount];

	for (int i = 0; i < *elementsCount; i++)
		fscanf_s(filePointer, "%d", &(*array)[i]);
}


void PlotArbitraryGraph()
{
	elementsCount = 50;
	num = new int[elementsCount];
	for (int i = 0; i < elementsCount; i++)
		num[i] = sin(i / (float)elementsCount * 2 * 3.14) * 10;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		switch (task)
		{
		case Task::Arbitrary:
		{
			PlotArbitraryGraph();
			break;
		}
		case Task::A:
		{
			GetArrayFromFile(&num, &elementsCount);
			TaskA(num, elementsCount);
			break;
		}
		case Task::B:
		{
			GetArrayFromFile(&num, &elementsCount);
			TaskB(num, elementsCount, &minID, &maxID);
			break;
		}
		case Task::C:
		{
			GetArrayFromFile(&num, &elementsCount);
			TaskC(num, elementsCount, &num2, &elementsCount2);
			break;
		}
		case Task::D:
		{
			GetArrayFromFile(&num, &elementsCount);
			TaskD(num, elementsCount, &num2, &elementsCount2);
			break;
		}
		}
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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

		RECT client;
		GetClientRect(hWnd, &client);
		int width = client.right - client.left;
		int height = client.bottom - client.top;

		if (task == Task::C || task == Task::D)
		{
			RECT client;
			GetClientRect(hWnd, &client);

			PlotGraph(hdc, width * 0.05f, height * 0.3f, width * 0.4f, height * 0.7f, num, elementsCount, minID, maxID);
			PlotGraph(hdc, width * 0.6f, height * 0.3f, width - width * 0.05f, height * 0.7f, num2, elementsCount2, minID, maxID);
		}
		else
			PlotGraph(hdc, width * 0.25f, height * 0.3f, width * 0.75f, height * 0.7f, num, elementsCount, minID, maxID);
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