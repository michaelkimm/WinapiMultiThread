// ThreadProgramming.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "ThreadProgramming.h"

#define MAX_LOADSTRING 100

// : >> 디버깅용 콘솔창
#include <stdio.h>
#include <iostream>

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;
static int cnt = 0;
// <<


// : >> 멀티스레드
#include <process.h>
#include <time.h>
unsigned __stdcall ThFunc1(LPVOID lpParam);
CRITICAL_SECTION cs;
void ExitThread(DWORD, HANDLE);
HWND hwnd;
// <<

// : >> 도형
const int OBJECT_SIZE = 100;
int object_cnt = 0;
CObject *pt_objects[OBJECT_SIZE];
void DrawObject(HDC hdc);
void DestoryObject();
// <<

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
RECT rectView;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THREADPROGRAMMING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THREADPROGRAMMING));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THREADPROGRAMMING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THREADPROGRAMMING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD dwThID1;
	static HANDLE hThread;

    switch (message)
    {
	case WM_CREATE:
		{
			SetTimer(hWnd, 123, 100, NULL);
			GetClientRect(hWnd, &rectView);
			hwnd = hWnd;

			// 멀티쓰레드
			InitializeCriticalSection(&cs);
			hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall*)(void*))ThFunc1, NULL, 0, (unsigned*)&dwThID1);

		}
		break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &rectView);
		}
		break;
	case WM_TIMER:
		{
			switch (wParam)
			{
			case 123:
				InvalidateRect(hWnd, NULL, false);
				break;
			default:
				break;
			}
		}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
	case WM_LBUTTONDOWN:
		{
			int px = rand() % rectView.right;
			int py = rand() % rectView.bottom;
			int r = 10;
			pt_objects[object_cnt] = new CCircle(px, py, 0, 0, 0, 0, SIZE{2 * r, 2 * r}, 1, CObject::circle, r);
			object_cnt++;
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DestoryObject();
		ExitThread(dwThID1, hThread);
		DeleteCriticalSection(&cs);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

void DrawObject(HDC hdc)
{
	for (int i = 0; i < object_cnt; i++)
	{
		if (pt_objects[i] == NULL)
			return;
		else
			pt_objects[i]->Draw(hdc);
	}
}

void DestoryObject()
{
	for (int i = 0; i < object_cnt; i++)
	{
		if (pt_objects[i] == NULL)
			return;
		else
		{
			delete pt_objects[i];
			pt_objects[i] = NULL;
		}
	}
}

unsigned __stdcall ThFunc1(LPVOID lpParam)
{
	HBRUSH hBrush;
	HBRUSH hOldBrush;
	while (1)
	{
		EnterCriticalSection(&cs);
		HDC hdc = GetDC(hwnd);

		for (int i = 0; i < object_cnt; i++)
		{
			if (pt_objects[i] == NULL)
				return 0;
			else
			{
				hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
				hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				pt_objects[i]->Draw(hdc);

				SelectObject(hdc, hOldBrush);
				DeleteObject(hBrush);
			}
		}

		ReleaseDC(hwnd, hdc);
		Sleep(1000);
		LeaveCriticalSection(&cs);
	}
	return 0;
}

void ExitThread(DWORD ID, HANDLE h)
{
	DWORD dwExitCode = 0;
	GetExitCodeThread(h, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		TerminateThread(h, dwExitCode);
		// CloseHandle(h);
	}
}