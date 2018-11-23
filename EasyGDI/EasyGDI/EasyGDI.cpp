// EasyGDI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EasyGDI.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;													    	    // 当前实例
WCHAR szTitle[MAX_LOADSTRING]=L"简易绘图—作者：熊九龙";					   // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];										   // 主窗口类名

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDC_EASYGDI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EASYGDI));
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EASYGDI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EASYGDI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

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
	static int iSelectionFunc = IDM_STY_MOUSE;
	static int iSelectionLine = IDM_EDIT_LINE_LINE;
	static int iSelectionEdit;
	static CHOOSECOLOR cc;
	static COLORREF crCustomColors[16];
	static COLORREF crText = RGB(0, 0, 0);
	static CHOOSEFONT cf;
	static LOGFONT logfont;
	static HFONT hFont;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HMENU hMenu;

    switch (message)
    {

	case WM_CREATE:
		cc.lStructSize = sizeof(CHOOSECOLOR); 
		cc.hwndOwner = hWnd; 
		cc.hInstance = NULL;
		cc.rgbResult = RGB(0, 0, 0);
		cc.lpCustColors = crCustomColors; 
		cc.Flags = CC_RGBINIT | CC_FULLOPEN; 
		cc.lCustData = 0; cc.lpfnHook = NULL; 
		cc.lpTemplateName = NULL; 

		GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), (PTSTR)&logfont);
		cf.lStructSize = sizeof(CHOOSEFONT); 
		cf.hwndOwner = hWnd; 
		cf.hDC = NULL; 
		cf.lpLogFont = &logfont; 
		cf.iPointSize = 0; 
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
		cf.rgbColors = 0;
		cf.lCustData = 0; 
		cf.lpfnHook = NULL;
		cf.lpTemplateName = NULL; 
		cf.hInstance = NULL; 
		cf.lpszStyle = NULL;

		return 0;
    case WM_COMMAND:
        {
			hMenu = GetMenu(hWnd);
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
				//文件
			case IDM_FILE_SAVE:

				return 0;
			case IDM_FILE_OPEN:

				return 0;

			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

				//图形类型
			case IDM_STY_MOUSE:
			case IDM_STY_LINETO:
			case IDM_STY_TEXT:
			case IDM_STY_RECT:
			case IDM_STY_ELIP:
				CheckMenuItem(hMenu, iSelectionFunc, MF_UNCHECKED);
				iSelectionFunc = LOWORD(wParam); 
				CheckMenuItem(hMenu, iSelectionFunc, MF_CHECKED);
				InvalidateRect(hWnd, NULL, TRUE); 
				return 0;

				//颜色和字体
			case IDM_CT_TEXT:
				if (ChooseFont(&cf))
				{
					if (hFont) DeleteObject(hFont);
					hFont = CreateFontIndirect(&logfont);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				return 0;

			case IDM_CT_LINECOLOR:
			case IDM_CT_RECTCOLOR:
				if (ChooseColor(&cc)) {
					crText = crCustomColors[0];
					InvalidateRect(hWnd, NULL, TRUE);
				}
				return 0;

				//编辑划线
			case IDM_EDIT_LINE_LINE:
			case IDM_EDIT_LINE_POT:
				CheckMenuItem(hMenu, iSelectionLine, MF_UNCHECKED);
				CheckMenuItem(hMenu, iSelectionLine, MF_CHECKED);
				InvalidateRect(hWnd, NULL, TRUE);
				//编辑操作
			case IDM_EDIT_MOVE:
			case IDM_EDIT_SIZE:
			case IDM_EDIT_COLOR:
			case IDM_EDIT_TEXT:
			case IDM_EDIT_DELETE:
				iSelectionEdit = LOWORD(wParam);
				return 0;

				
            case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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


