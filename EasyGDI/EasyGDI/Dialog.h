#ifndef _DIALOG_H_
#define _DIALOG_H_
#include<Windows.h>
//ϵͳ�Ի���ͨ�öԻ���ǰ��������

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




//�Զ���Ի���ǰ��������





//dialog����extern������






#endif // !_Dialog_H_
