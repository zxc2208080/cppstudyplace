  /*******************************************************************
程序：ChildWin
文件：ChildWin.c
功能：子窗口演示程序——命中测试
********************************************************************/

#include "stdafx.h"


#define ROW_NUM 4     //行数
#define COL_NUM 8     //列数
#define Len_GameStr 15//字符串长度

#define WM_CHILDWIN  WM_USER+1    //主窗口通知子窗口的自定义消息

BOOL InitWindow (HINSTANCE hInstance, int nCmdShow);

//主窗口函数原型
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//子窗口函数原型
LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

TCHAR szAppName[] = TEXT ("ChildWin");            //主窗口类名称
TCHAR szChildName[] = TEXT ("ChildWin_Child");    //子窗口类名称

CHAR szGameStr[Len_GameStr];
int nGameCode = 0;

HINSTANCE hInst;                                  //全程变量——实例句柄


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

BOOL cleanChar(char	TypeChar)
{
	BOOL st = FALSE;
	for (int i = 0; i < strlen(szGameStr); i++)
	{
		if (TypeChar == szGameStr[i])
		{
			for (int j = i; j < strlen(szGameStr); j++)
			{
				szGameStr[j] = szGameStr[j + 1];
			}
			nGameCode += 10;
			if (strlen(szGameStr) == 0)
			{
				st = TRUE;
			}
			break;
		}
	}

	return st;
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	MSG msg;

	if (!InitWindow (hInstance, iCmdShow))
		return FALSE;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return msg.wParam;
}

static BOOL InitWindow (HINSTANCE hInstance, int iCmdShow)
{
	HWND hwnd;   
	WNDCLASS wcMain; 

	wcMain.style         = CS_VREDRAW | CS_HREDRAW;
	wcMain.lpfnWndProc   = MainWndProc;
	wcMain.cbClsExtra    = 0;
	wcMain.cbWndExtra    = 0;
	wcMain.hInstance     = hInstance;
	wcMain.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcMain.hCursor       = LoadCursor (hInstance, MAKEINTRESOURCE(IDC_MAINCURSOR));
	wcMain.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	wcMain.lpszMenuName = MAKEINTRESOURCE(IDC_CHILDWIN);
	wcMain.lpszClassName = szAppName;

	if (!RegisterClass (&wcMain))
	{
		MessageBox (NULL, TEXT ("注册主窗口类失败！"), szAppName,
                    MB_ICONERROR);
		return 0;
	}
	
	hInst = hInstance;

	hwnd = CreateWindow (szAppName, 
	                    TEXT ("子窗口演示程序——命中测试"),
                     	WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 
						CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, hInstance, NULL); 

	if( !hwnd ) return FALSE;

	ShowWindow( hwnd, iCmdShow );
	UpdateWindow( hwnd );
	return TRUE;
}

LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hChildWnd[COL_NUM][ROW_NUM];  //子窗口句柄
    static int  nXBox, nYBox;                 //子窗口宽度和高度
    static WORD nChildWin_ID;                 //被命中的子窗口标识
    static int  nRow, nCol;                   //被命中的子窗口位置（即行列号）
	int         x, y;
	static int judge = 0;
	static int index = 0;
	static int code = 0;
	char szScore[32];
	RECT rect;
	WNDCLASS    wcChild;                      //子窗口类
    
    switch (message)
    {
    case WM_CREATE:
		wcChild.style         = CS_VREDRAW | CS_HREDRAW;
		wcChild.lpfnWndProc   = ChildWndProc;
		wcChild.cbClsExtra    = 0;
		wcChild.cbWndExtra	  = sizeof(long)*2;
		wcChild.hInstance     = hInst;
		wcChild.hIcon         = NULL;
		wcChild.hCursor       = LoadCursor (hInst, MAKEINTRESOURCE(IDC_BBT));
		wcChild.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH);
								//GetStockObject (HOLLOW_BRUSH);
		wcChild.lpszMenuName  = NULL;
		wcChild.lpszClassName = szChildName ;
     
		if (!RegisterClass (&wcChild))
		{
			MessageBox (NULL, TEXT ("注册子窗口类失败！"), szChildName,
				        MB_ICONERROR);
			return 0;
		}

        //1创建类似“画图”程序中绘图工具栏的各工具子窗口
		for (x=0; x<COL_NUM; x++)
		for (y=0; y<ROW_NUM; y++)
		{
			nChildWin_ID = x | y<<3;            //子窗口标识值
			hChildWnd[x][y] = CreateWindow (szChildName, NULL,
											WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
											0,0,0,0,
											hWnd,
											(HMENU)(nChildWin_ID),
											//0,
											hInst, NULL);
		}
		
		//令0号子窗口为当前命中的子窗口
		nChildWin_ID = 0;
		nRow = 0;
		nCol = 0;


		//获得字符串
		getRandomChar();
		return 0;

    case WM_SIZE:
        nXBox = LOWORD (lParam)/COL_NUM;
        nYBox = (HIWORD (lParam)/2)/ROW_NUM;

   		//主窗口大小改变时，重新确定各子窗口在主窗口客户区的位置
		for (x=0; x<COL_NUM; x++)
		for (y=0; y<ROW_NUM; y++)
		{
			MoveWindow (hChildWnd[x][y], x*nXBox, y*nYBox, nXBox, nYBox, TRUE);
		}
		return 0;
          
    case WM_COMMAND:                //子窗口被命中的通知消息
		//当子窗口被命中时发送此消息通知主窗口，以便由主窗口确定是否命中另一个子窗口，
		//若是，则发送消息通知子窗口函数改变前一次被命中子窗口的状态，
		if (!(LOWORD (wParam) == nChildWin_ID))
		{
			//显示TypeChar字符串
			if (!cleanChar((char) ((int)wParam + 'a') ))
				{
					code += 10;
				}
				else
				{
					MessageBox(hWnd, TEXT("输入完成了!"), TEXT(""), MB_OK);
				}
			SendMessage ((HWND)hChildWnd[nCol][nRow], WM_CHILDWIN, 0, 0L);
			//记录新的被命中的子窗口
			nChildWin_ID = LOWORD (wParam);
			//子窗口行列算法
			nRow = nChildWin_ID >> 3;
			nCol = (nChildWin_ID > 7) ? (nChildWin_ID - 8 * nRow) : nChildWin_ID;
		}
		
		switch (LOWORD(wParam))
		{
		case ID_CHANGECURSOR:
			//给窗口消息队列发送 WM_CLOSE 消息以便退出应用程序
			if (judge % 2)
			{
				SetClassLong(hWnd, GCL_HCURSOR,
					(LONG)LoadCursor((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
						MAKEINTRESOURCE(IDC_MAINCURSOR)));
				judge++;
			}
			else
			{
				SetClassLong(hWnd, GCL_HCURSOR,
					(LONG)LoadCursor((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
						MAKEINTRESOURCE(IDC_FIRE)));
				judge++;
			}
			return 0;

		case IDM_EXIT:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			return 0;

		}
		GetClientRect(hWnd, &rect);
		rect.top = rect.bottom / 2;
		InvalidateRect(hWnd, &rect, TRUE);
		return 0;

	
    case WM_LBUTTONDOWN:
		MessageBeep(-1);         //发出标准蜂鸣声
		MessageBox (hWnd, TEXT ("鼠标左键按下了!"), TEXT ("主窗口鼠标消息"), MB_OK);
		return 0;

	case WM_RBUTTONDOWN:
		{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ClientToScreen(hWnd, &pt);
		HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDC_CHILDWIN));     //菜单载入
		HMENU NewhMenu = GetSubMenu(hMenu, 0);
		TrackPopupMenu(NewhMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		}
		return 0;

	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(0, 255, 0));
		rect.top = rect.bottom / 2;
		DrawText(hdc, szGameStr,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect.top = rect.top - rect.bottom / 4;
		wsprintf(szScore, "得分：%d", code);
		DrawText(hdc, szScore,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);


	}
	break;


	case WM_DESTROY: 
		PostQuitMessage (0);
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);

} //函数 WinProc 结束

LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;
	static long id;
	//char charshow1;
	static char charshow;
	
	CHAR Charid[32];
	CHAR cChar[32];
	switch (message)
    {
    case WM_CREATE:
		
		//置各子窗口命中标志为0，（0号子窗口则为1）
		if (GetWindowLong (hWnd, GWL_ID) == 0)
//		if (GetWindowWord (hWnd, GWL_ID) == 0)		  // 2004.03.11 Liangchenghui
			SetWindowLong (hWnd, 0, 1);
		else
			SetWindowLong (hWnd, 0, 0); 
		return 0;
          
    case WM_LBUTTONDOWN:
		//新的子窗口被命中
		if (GetWindowLong (hWnd, 0) == 0)
		{
			//置该子窗口命中标志为1
			SetWindowLong (hWnd, 0, 1);
			//发送消息通知主窗口
			SendMessage (GetParent (hWnd), WM_COMMAND, 
						//(WPARAM) MAKELONG ((WORD)GetWindowWord (hWnd, GWL_ID), (WORD)0),
						GetWindowLong (hWnd, GWL_ID),
						(LPARAM) hWnd);
			//发送字符给主窗口
			
			//重绘子窗口客户区
			GetClientRect (hWnd, &rect);
			InvalidateRect (hWnd, &rect, TRUE);
		}


		return 0;

    case WM_CHILDWIN:          //主窗口发送的通知消息，改变原命中子窗口的状态
		//置子窗口命中标志为0
		SetWindowLong (hWnd, 0, 0);

		//恢复子窗口初始显示方式
		GetClientRect (hWnd, &rect);
		InvalidateRect (hWnd, &rect,TRUE);

		return 0;

    case WM_PAINT:
        hdc = BeginPaint (hWnd, &ps);
		SelectObject(hdc, CreateSolidBrush(500));
		GetClientRect(hWnd, &rect);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		
		id = GetWindowLong(hWnd, GWL_ID);
		charshow = 'a' + (int)id;
		SetWindowLong(hWnd, sizeof(long), (long)charshow);
		//charshow1 = (CHAR)GetWindowLong(hWnd, sizeof(long));
		wsprintf(cChar, "%c", charshow);
		DrawText(hdc, cChar,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		wsprintf(Charid, "id=：%d", id);
		rect.right = rect.left + 50;
		rect.bottom = rect.top + 30;
		DrawText(hdc, Charid,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		//反相绘制被命中的子窗口
		if (GetWindowLong (hWnd, 0))
		{
			GetClientRect (hWnd, &rect);

			//用黑色画矩形rect的左边线和上边线
			MoveToEx (hdc, 0, rect.bottom-1, NULL);
			LineTo (hdc, 0, 0);
			LineTo (hdc, rect.right-1, 0);

			MoveToEx(hdc, 1,1, NULL);
			LineTo(hdc, rect.right-1, rect.bottom-1);
			MoveToEx(hdc, 1, rect.bottom-1, NULL);
			LineTo(hdc, rect.right-1, 1);

			//用白色画矩形rect的右边线和边线
			SelectObject (hdc, GetStockObject (WHITE_PEN));
			MoveToEx (hdc, rect.right-1, 0, NULL);
			LineTo (hdc, rect.right-1, rect.bottom-1);
			LineTo (hdc, 0, rect.bottom-1);
			SelectObject (hdc, GetStockObject (BLACK_PEN));

			

		}
		
		EndPaint (hWnd, &ps);
        return 0;
               
	case WM_DESTROY: 
		PostQuitMessage (0);
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);

} //函数 ChildWinProc 结束

