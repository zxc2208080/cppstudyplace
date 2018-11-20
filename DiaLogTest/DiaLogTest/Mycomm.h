#ifndef _MY_COMM_H_

#define _MY_COMM_H_

#include <Windows.h>

// 此代码模块中包含的函数的前向声明:
BOOL CALLBACK ColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL                LoadData();
BOOL				SaveData();


typedef struct tagMyline
{
	POINT ptS;
	POINT ptE;
	COLORREF color;
}MYLINE, *PMYLINE;

extern MYLINE gLine;
extern BOOL gbInit;
extern int iCurrentColor;
extern int iBackCurrentColor;

#endif //_MY_COMM_H_