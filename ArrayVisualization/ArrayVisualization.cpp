﻿// ArrayVisualization.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ArrayVisualization.h"
#include <math.h>
#include <stdio.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
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
    LoadStringW(hInstance, IDC_ARRAYVISUALIZATION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARRAYVISUALIZATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ARRAYVISUALIZATION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    HBRUSH positiveBrush = CreateSolidBrush(RGB(255, 100, 100));
    HBRUSH negativeBrush = CreateSolidBrush(RGB(100, 100, 255));

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

        Rectangle(hdc, left + i * elementWidht, bottom - num[i] * 10, left + (i + 1) * elementWidht, bottom);
    }

}

void TaskA(int* array, int elementsCount)
{
    for(int i = 0; i < elementsCount; i++)
        array[i] = array[i] * -1;
}

void TaskB(int* array, int elementsCount, int* minID, int* maxID)
{
    int min = 999999999;
    int max = -999999999;

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
    int min = 999999999;
    int max = -999999999;

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
            ++*newElementsCount;

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
    int min = 999999999;
    int max = -999999999;

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
        if ((array[i] % 2) != 0)
            ++* newElementsCount;

    *newArray = new int[*newElementsCount];
    int j = 0;
    for (int i = minId + 1; i < maxId; i++)
        if ((array[i] % 2) != 0)
        {
            (*newArray)[j] = array[i];
            j++;
        }

}

int* num;
int elementsCount = 0;
int minID = -1;
int maxID = -1;
void GetArrayFromFile(int** array, int* elementsCount)
{
    FILE* filePointer;
    fopen_s(&filePointer, "data.txt", "r");

    fscanf_s(filePointer, "%d", elementsCount);
    (*array) = new int[*elementsCount];

    for(int i = 0; i < *elementsCount; i++)
        fscanf_s(filePointer, "%d", &(*array)[i]);
}

void UseTaskC()
{
    GetArrayFromFile(&num, &elementsCount);
    int* newNum;
    int newElementsCount = 0;
    TaskC(num, elementsCount, &newNum, &newElementsCount);
    delete[elementsCount] num;
    num = newNum;
    elementsCount = newElementsCount;
}

void UseTaskD()
{
    GetArrayFromFile(&num, &elementsCount);
    int* newNum;
    int newElementsCount = 0;
    TaskD(num, elementsCount, &newNum, &newElementsCount);
    delete[elementsCount] num;
    num = newNum;
    elementsCount = newElementsCount;
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
        //PlotArbitraryGraph();

        //GetArrayFromFile(&num, &elementsCount);

        //TaskA(num, elementsCount);
        //TaskB(num, elementsCount, &minID, &maxID);
        //UseTaskC();
        //UseTaskD();

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

            PlotGraph(hdc, 100, 100, 1000, 500, num, elementsCount, minID, maxID);

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
