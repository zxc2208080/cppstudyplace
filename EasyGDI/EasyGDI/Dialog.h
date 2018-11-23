#ifndef _DIALOG_H_
#define _DIALOG_H_
#include<Windows.h>
//系统对话框，通用对话框前向声明：

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




//自定义对话框前向声明：





//dialog所需extern变量：






#endif // !_Dialog_H_
