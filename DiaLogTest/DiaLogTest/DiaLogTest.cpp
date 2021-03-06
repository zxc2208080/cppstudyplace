// DiaLogTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DiaLogTest.h"

#define MAX_LOADSTRING 100

// 全局变量:
MYLINE gLine;
BOOL gbInit = FALSE;
HINSTANCE hInst;										        // 当前实例
WCHAR szTitle[MAX_LOADSTRING]=L"实验四 对话框颜色选择";		    // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]=L"颜色选择";				// 主窗口类名

int iCurrentColor = IDC_CC_BLACK;						//控件按键标识
int iBackCurrentColor = 6666666;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_DIALOGTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIALOGTEST));

    MSG msg;

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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIALOGTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIALOGTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static COLORREF     crCustomColors[3] = { RGB(255, 0, 0), RGB(0, 255, 0),
										RGB(0, 0, 255) };
	static COLORREF     crText = RGB(0, 0, 0);

	static HINSTANCE    hInstance;
	RECT                rect;
	static int    iSelectionFunc = IDM_FUNC_POLYLINE;
	POINT         ptDiamond[5], ptppline[10];
	DWORD pn[2];
	static SYSTEMTIME st;
	static CHAR szTime[32];
    switch (message)
    {
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		SetTimer(hWnd, NULL, USER_TIMER_MINIMUM, NULL);
		return 0;

	case WM_TIMER:
	{
		GetLocalTime(&st);
		HDC hdc1 = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		wsprintf((LPWSTR)szTime, L"时间是:%d年%d月%d日%d时%d分%d秒", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		SetTextColor(hdc1, crCustomColors[iCurrentColor - IDC_CC_RED]);
		DrawText(hdc1, (LPWSTR)szTime,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		ReleaseDC(hWnd, hdc1);
	}
	
	return 0;


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_FILE_CTCOLOR:
				//打开“颜色”通用对话框并选择颜色添加到自定义颜色中
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_CC), hWnd, ColorDlgProc))
					InvalidateRect(hWnd, NULL, TRUE);
				break;
			case IDM_FILE_CBCOLOR:
				//打开“颜色”通用对话框并选择颜色添加到自定义颜色中
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_CCB), hWnd, BColorDlgProc))
					InvalidateRect(hWnd, NULL, TRUE);
				break;

			case IDM_LINE_OPEN:
				if (LoadData())
				{
					gbInit = TRUE;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			case IDM_LINE_SAVE:
				SaveData();
				break;
			//画图分组
			case IDM_FUNC_POLYLINE:
			case IDM_FUNC_POLYPOLYLINE:
			case IDM_FUNC_POLYGON:
			case IDM_FUNC_POLYPOLYGON:
				iSelectionFunc = LOWORD(wParam);	
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_LBUTTONDOWN:
		gLine.ptS.x = GET_X_LPARAM(lParam);
		gLine.ptS.y = GET_Y_LPARAM(lParam);
		SetCapture(hWnd);
		return 0;


	case WM_LBUTTONUP:
		gLine.ptE.x = GET_X_LPARAM(lParam);
		gLine.ptE.y = GET_Y_LPARAM(lParam);
		ReleaseCapture();
		gbInit = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONUP:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ClientToScreen(hWnd, &pt);
		HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),MAKEINTRESOURCE(IDC_DIALOGTEST));     //菜单载入
		HMENU NewhMenu = GetSubMenu(hMenu, 2);
		TrackPopupMenu(NewhMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
	}
		return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

			SelectObject(hdc, CreateSolidBrush(iBackCurrentColor));
			GetClientRect(hWnd, &rect);
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

			switch (iSelectionFunc)
			{
			case IDM_FUNC_POLYLINE:
				ptDiamond[0].x = 0;					ptDiamond[0].y = rect.bottom / 2;
				ptDiamond[1].x = rect.right / 2;	ptDiamond[1].y = rect.bottom / 4;
				ptDiamond[2].x = rect.right;		ptDiamond[2].y = rect.bottom / 2;
				ptDiamond[3].x = rect.right / 2;	ptDiamond[3].y = rect.bottom;

				Polyline(hdc, ptDiamond, 4);
				break;

			case IDM_FUNC_POLYPOLYLINE:
				ptppline[0].x  = 50;	ptppline[0].y  = 50;
				ptppline[1].x  = 100;	ptppline[1].y  = 50;
				ptppline[2].x  = 125;	ptppline[2].y  = 75;
				ptppline[3].x  = 100;	ptppline[3].y  = 100;
				ptppline[4].x  = 50;	ptppline[4].y  = 100;
				ptppline[5].x  = 25;	ptppline[5].y  = 75;
				pn[0] = 6;

				ptppline[6].x  = 200;	ptppline[6].y  = 25;
				ptppline[7].x  = 300;	ptppline[7].y  = 25;
				ptppline[8].x  = 300;	ptppline[8].y  = 125;
				ptppline[9].x  = 200;	ptppline[9].y  = 125;
				pn[1] = 4;

				PolyPolyline(hdc, ptppline,pn , 2);
				break;

			case IDM_FUNC_POLYGON:
				if (GetPolyFillMode(hdc) == WINDING)
					SetPolyFillMode(hdc, ALTERNATE);
				else
					SetPolyFillMode(hdc, WINDING);

				ptDiamond[0].x = 0;					ptDiamond[0].y = rect.bottom / 3;
				ptDiamond[1].x = rect.right;		ptDiamond[1].y = rect.bottom / 3;
				ptDiamond[2].x = rect.right / 4;	ptDiamond[2].y = rect.bottom;
				ptDiamond[3].x = rect.right / 2;	ptDiamond[3].y = 0;
				ptDiamond[4].x = 3 * rect.right / 4;ptDiamond[4].y = rect.bottom;

				Polygon(hdc, ptDiamond, 5);
				break;

			case IDM_FUNC_POLYPOLYGON:				
				break;
			}

			if (gbInit)
			{
				MoveToEx(hdc, gLine.ptS.x, gLine.ptS.y, NULL);
				LineTo(hdc, gLine.ptE.x, gLine.ptE.y);
			}

		
	
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
		KillTimer(hWnd,NULL);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}