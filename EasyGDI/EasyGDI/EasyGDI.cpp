// EasyGDI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EasyGDI.h"
#include "needToDraw.h"

#define MAX_LOADSTRING 100
#define MAX_DATA 256
// 全局变量:
HINSTANCE hInst;													    	    // 当前实例
WCHAR szTitle[MAX_LOADSTRING];												   // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];										   // 主窗口类名
CHAR szFileName[128] = "Line.bin";
	//类使用变量
int index=0;
needToDraw** pMyDraw=NULL;
char szBuffer[64];

BOOL SaveData();
BOOL OpenData();

INT_PTR CALLBACK DlgText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemTextA(hDlg, IDC_TEXT_EDIT, "你所想显示的文本");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			GetDlgItemText(hDlg, IDC_TEXT_EDIT, szBuffer, 64);
			pMyDraw[index]->setChar(szBuffer);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


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

   HWND hWnd = CreateWindowW(szWindowClass,(LPCWSTR)TEXT("简易绘图—作者熊九龙"), WS_OVERLAPPEDWINDOW,
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
	//对话框 文字颜色
	static CHOOSECOLOR cc;
	static COLORREF crCustomColors[16];
	static COLORREF crText = RGB(0, 0, 0);
	static CHOOSEFONT cf;
	static LOGFONT logfont;
	static HFONT hFont;
	static LOGPEN logpen;

	//菜单选择状态
	static int iSelectionFunc = IDM_STY_MOUSE;
	static int iSelectionLine = IDM_PEN_BLACK;
	static int iSelectionEdit;
	//画笔
	static HPEN   hRedDashdotPen, hBlueSolidPen;

	static HMENU hPopMenu;      //快捷菜单
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HMENU hMenu;
	POINT pt;
	static HPEN hOldPen;


    switch (message)
    {

	case WM_CREATE:

		//创建红色点划线画笔
		hRedDashdotPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));

		//创建宽度为3的蓝色画笔
		logpen.lopnStyle = PS_SOLID;
		logpen.lopnWidth.x = 3;
		logpen.lopnColor = RGB(0, 0, 255);
		hBlueSolidPen = CreatePenIndirect(&logpen);

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

		//创建红色点划线画笔
		hRedDashdotPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
		//chushihua lei
		pMyDraw = new needToDraw*[MAX_DATA];
		return 0;

	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		switch (iSelectionFunc)
		{
			//图形类型
		case IDM_STY_TEXT:
			pMyDraw[index] = new MyText(index, pt);
			break;
		case IDM_STY_LINETO:
			pMyDraw[index] = new MyLine(index, pt);
			break;
		case IDM_STY_RECT:
			pMyDraw[index] = new MyRect(index, pt);
			break;
		case IDM_STY_ELIP:
			pMyDraw[index] = new MyEllip(index, pt);
			break;
		case IDM_STY_MOUSE:
			break;
		default:
			break;
		}
		SetCapture(hWnd);
		return 0;
		

	case WM_LBUTTONUP:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		switch (iSelectionFunc)
		{
			//图形类型
		case IDM_STY_TEXT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXT), hWnd, DlgText);
		case IDM_STY_LINETO:		
		case IDM_STY_RECT:	
		case IDM_STY_ELIP:
			pMyDraw[index]->setPoint_ERB(pt);
			pMyDraw[index]->setHPen(hOldPen);
			index++;
			break;
		case IDM_STY_MOUSE:
			break;
		default:
			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseCapture();
		return 0;

	case WM_RBUTTONDOWN:
		if (iSelectionFunc != IDM_STY_MOUSE)
		{
			//非鼠标模式下的快捷菜单
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			ClientToScreen(hWnd, &pt);
			HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDC_EASYGDI));
			HMENU NewhMenu = GetSubMenu(hMenu, 3);
			TrackPopupMenu(NewhMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		}
		return 0;

    case WM_COMMAND:
		
        {
			hMenu = GetMenu(hWnd);
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
				//文件
			case IDM_FILE_SAVE:
				SaveData();
				return 0;
			case IDM_FILE_OPEN:
				OpenData();
				InvalidateRect(hWnd, NULL, TRUE);
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

				//编辑画笔
			case IDM_PEN_BLACK:			
			case IDM_PEN_REDDASHDOT:
			case IDM_PEN_BLUESOLID:
				CheckMenuItem(hMenu, iSelectionLine, MF_UNCHECKED);
				iSelectionLine = LOWORD(wParam);
				CheckMenuItem(hMenu, iSelectionLine, MF_CHECKED);
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;

				//编辑操作
			case IDM_EDIT_TEXT:
			case IDM_EDIT_MOVE:
			case IDM_EDIT_SIZE:
			case IDM_EDIT_COLOR:
			
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
			hdc = BeginPaint(hWnd, &ps);
			switch (iSelectionLine)
			{
			case IDM_PEN_BLACK:
				hOldPen = (HPEN)GetStockObject(BLACK_PEN);
				break;
			case IDM_PEN_REDDASHDOT:
				hOldPen = hRedDashdotPen;
				break;
			case IDM_PEN_BLUESOLID:
				hOldPen = hBlueSolidPen;
				break;
			}
			for (int i=0; i < index; i++)
			{
				pMyDraw[i]->DrawMyself(hdc);
			}
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



BOOL SaveData()
{
	FILE *fp = fopen(szFileName, "wb");
	fwrite(&index, sizeof(int), 1, fp);
	fwrite(*pMyDraw, sizeof(needToDraw), index - 1, fp);
	fclose(fp);
	return 1;
}
BOOL OpenData()
{
	FILE *fp = fopen(szFileName, "rb");
	fread(&index, sizeof(int), 1, fp);
	fread(*pMyDraw, sizeof(needToDraw), index, fp);
	fclose(fp);
	return 1;
}