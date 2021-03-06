// TypeChar.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TypeChar.h"

#define _CRT_RAND_S
#define MAX_LOADSTRING 100
#define Len_GameStr 15
// 全局变量:123123
HINSTANCE hInst;										// 当前实例
WCHAR szTitle[MAX_LOADSTRING] = L"T";					// 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = L"N";			    // 主窗口类名
CHAR szGameStr[Len_GameStr] = "TypeChar";
int nGameCode = 0;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void getRandomChar()
{
	SYSTEMTIME st; //定义SYSTEMTIME变量st
	GetSystemTime(&st); //获取系统时间到st
	srand(st.wMilliseconds);

	int k;
	for (k = 0; k < Len_GameStr - 1; k++)
		szGameStr[k] = 'a' + rand() % ('z' - 'a');
	szGameStr[k] = '\0';
}
BOOL cleanChar(WPARAM wParam, HWND hWnd)
{
	BOOL st = FALSE;
	for (int i = 0; i < strlen(szGameStr); i++)
	{
		if (wParam == szGameStr[i])
		{
			for (int j = i; j < strlen(szGameStr); j++)
			{
				szGameStr[j] = szGameStr[j + 1];
			}
			nGameCode += 10;
			if (strlen(szGameStr) == 0)
			{
				nGameCode = 10 * Len_GameStr;
				st = TRUE;
				MessageBox(hWnd, TEXT("FINISH"), TEXT("WIN"), MB_OK | MB_ICONQUESTION);
			}
			break;
		}
	}

	return st;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TYPECHAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TYPECHAR));

	MSG msg;

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYPECHAR));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TYPECHAR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass,
		L"P", WS_OVERLAPPEDWINDOW,
		100, 100, 400, 300,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	CHAR szScore[32], szPox[32];
	static int dnxPox, dnyPox;
	switch (message)
	{
	case WM_CREATE:
		getRandomChar();
		MessageBox(hWnd, TEXT("GameStart"), TEXT("Message"), MB_OK | MB_ICONQUESTION);
		return 0;

	case WM_MOUSEMOVE:
		dnxPox = GET_X_LPARAM(lParam);
		dnyPox = GET_Y_LPARAM(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (!cleanChar(wParam, hWnd))
		{
			//MessageBox(hWnd, TEXT("FINISH"), TEXT("WIN"), MB_OK | MB_ICONQUESTION);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(0, 150, 255));
		DrawText(hdc, szGameStr,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		wsprintf(szScore, "得分：%d", nGameCode);
		wsprintf(szPox, "location is : %d,%d", dnxPox, dnyPox);
		rect.right = rect.left + 100;
		rect.bottom = rect.top + 30;
		DrawText(hdc, szScore,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect.right = rect.left + 150;
		rect.bottom = rect.top + 60;
		DrawText(hdc, szPox,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
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

// “关于”框的消息处理程序。
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
