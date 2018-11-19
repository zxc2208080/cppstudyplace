  /*******************************************************************
����ChildWin
�ļ���ChildWin.c
���ܣ��Ӵ�����ʾ���򡪡����в���
********************************************************************/

#include "stdafx.h"
#include <windowsx.h>

#define ROW_NUM 4     //����
#define COL_NUM 8     //����
#define Len_GameStr 15//�ַ�������

#define WM_CHILDWIN  WM_USER+1    //������֪ͨ�Ӵ��ڵ��Զ�����Ϣ

BOOL InitWindow (HINSTANCE hInstance, int nCmdShow);

//�����ں���ԭ��
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//�Ӵ��ں���ԭ��
LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

TCHAR szAppName[] = TEXT ("ChildWin");            //������������
TCHAR szChildName[] = TEXT ("ChildWin_Child");    //�Ӵ���������

CHAR szGameStr[Len_GameStr];
int nGameCode = 0;

HINSTANCE hInst;                                  //ȫ�̱�������ʵ�����


void getRandomChar()
{
	SYSTEMTIME st; //����SYSTEMTIME����st
	GetSystemTime(&st); //��ȡϵͳʱ�䵽st
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
		MessageBox (NULL, TEXT ("ע����������ʧ�ܣ�"), szAppName,
                    MB_ICONERROR);
		return 0;
	}
	
	hInst = hInstance;

	hwnd = CreateWindow (szAppName, 
	                    TEXT ("�Ӵ�����ʾ���򡪡����в���"),
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
    static HWND hChildWnd[COL_NUM][ROW_NUM];  //�Ӵ��ھ��
    static int  nXBox, nYBox;                 //�Ӵ��ڿ�Ⱥ͸߶�
    static WORD nChildWin_ID;                 //�����е��Ӵ��ڱ�ʶ
    static int  nRow, nCol;                   //�����е��Ӵ���λ�ã������кţ�
	int         x, y;
	static int judge = 0;
	static int index = 0;
	static int code = 0;
	char szScore[32];
	RECT rect;
	WNDCLASS    wcChild;                      //�Ӵ�����
    
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
			MessageBox (NULL, TEXT ("ע���Ӵ�����ʧ�ܣ�"), szChildName,
				        MB_ICONERROR);
			return 0;
		}

        //1�������ơ���ͼ�������л�ͼ�������ĸ������Ӵ���
		for (x=0; x<COL_NUM; x++)
		for (y=0; y<ROW_NUM; y++)
		{
			nChildWin_ID = x | y<<3;            //�Ӵ��ڱ�ʶֵ
			hChildWnd[x][y] = CreateWindow (szChildName, NULL,
											WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
											0,0,0,0,
											hWnd,
											(HMENU)(nChildWin_ID),
											//0,
											hInst, NULL);
		}
		
		//��0���Ӵ���Ϊ��ǰ���е��Ӵ���
		nChildWin_ID = 0;
		nRow = 0;
		nCol = 0;


		//����ַ���
		getRandomChar();
		return 0;

    case WM_SIZE:
        nXBox = LOWORD (lParam)/COL_NUM;
        nYBox = (HIWORD (lParam)/2)/ROW_NUM;

   		//�����ڴ�С�ı�ʱ������ȷ�����Ӵ����������ڿͻ�����λ��
		for (x=0; x<COL_NUM; x++)
		for (y=0; y<ROW_NUM; y++)
		{
			MoveWindow (hChildWnd[x][y], x*nXBox, y*nYBox, nXBox, nYBox, TRUE);
		}
		return 0;
          
    case WM_COMMAND:                //�Ӵ��ڱ����е�֪ͨ��Ϣ
		//���Ӵ��ڱ�����ʱ���ʹ���Ϣ֪ͨ�����ڣ��Ա���������ȷ���Ƿ�������һ���Ӵ��ڣ�
		//���ǣ�������Ϣ֪ͨ�Ӵ��ں����ı�ǰһ�α������Ӵ��ڵ�״̬��
		if (!(LOWORD (wParam) == nChildWin_ID))
		{
			//��ʾTypeChar�ַ���
			if (!cleanChar((char) ((int)wParam + 'a') ))
				{
					code += 10;
				}
				else
				{
					MessageBox(hWnd, TEXT("���������!"), TEXT(""), MB_OK);
				}
			SendMessage ((HWND)hChildWnd[nCol][nRow], WM_CHILDWIN, 0, 0L);
			//��¼�µı����е��Ӵ���
			nChildWin_ID = LOWORD (wParam);
			//�Ӵ��������㷨
			nRow = nChildWin_ID >> 3;
			nCol = (nChildWin_ID > 7) ? (nChildWin_ID - 8 * nRow) : nChildWin_ID;
		}
		
		switch (LOWORD(wParam))
		{
		case ID_CHANGECURSOR:
			//��������Ϣ���з��� WM_CLOSE ��Ϣ�Ա��˳�Ӧ�ó���
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
		MessageBeep(-1);         //������׼������
		MessageBox (hWnd, TEXT ("������������!"), TEXT ("�����������Ϣ"), MB_OK);
		return 0;

	case WM_RBUTTONDOWN:
		{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ClientToScreen(hWnd, &pt);
		HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDC_CHILDWIN));     //�˵�����
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
		wsprintf(szScore, "�÷֣�%d", code);
		DrawText(hdc, szScore,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		EndPaint(hWnd, &ps);


	}
	break;


	case WM_DESTROY: 
		PostQuitMessage (0);
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);

} //���� WinProc ����

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
		
		//�ø��Ӵ������б�־Ϊ0����0���Ӵ�����Ϊ1��
		if (GetWindowLong (hWnd, GWL_ID) == 0)
//		if (GetWindowWord (hWnd, GWL_ID) == 0)		  // 2004.03.11 Liangchenghui
			SetWindowLong (hWnd, 0, 1);
		else
			SetWindowLong (hWnd, 0, 0); 
		return 0;
          
    case WM_LBUTTONDOWN:
		//�µ��Ӵ��ڱ�����
		if (GetWindowLong (hWnd, 0) == 0)
		{
			//�ø��Ӵ������б�־Ϊ1
			SetWindowLong (hWnd, 0, 1);
			//������Ϣ֪ͨ������
			SendMessage (GetParent (hWnd), WM_COMMAND, 
						//(WPARAM) MAKELONG ((WORD)GetWindowWord (hWnd, GWL_ID), (WORD)0),
						GetWindowLong (hWnd, GWL_ID),
						(LPARAM) hWnd);
			//�����ַ���������
			
			//�ػ��Ӵ��ڿͻ���
			GetClientRect (hWnd, &rect);
			InvalidateRect (hWnd, &rect, TRUE);
		}


		return 0;

    case WM_CHILDWIN:          //�����ڷ��͵�֪ͨ��Ϣ���ı�ԭ�����Ӵ��ڵ�״̬
		//���Ӵ������б�־Ϊ0
		SetWindowLong (hWnd, 0, 0);

		//�ָ��Ӵ��ڳ�ʼ��ʾ��ʽ
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

		wsprintf(Charid, "id=��%d", id);
		rect.right = rect.left + 50;
		rect.bottom = rect.top + 30;
		DrawText(hdc, Charid,
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		//������Ʊ����е��Ӵ���
		if (GetWindowLong (hWnd, 0))
		{
			GetClientRect (hWnd, &rect);

			//�ú�ɫ������rect������ߺ��ϱ���
			MoveToEx (hdc, 0, rect.bottom-1, NULL);
			LineTo (hdc, 0, 0);
			LineTo (hdc, rect.right-1, 0);

			MoveToEx(hdc, 1,1, NULL);
			LineTo(hdc, rect.right-1, rect.bottom-1);
			MoveToEx(hdc, 1, rect.bottom-1, NULL);
			LineTo(hdc, rect.right-1, 1);

			//�ð�ɫ������rect���ұ��ߺͱ���
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

} //���� ChildWinProc ����

