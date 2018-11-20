#include "stdafx.h"

CHAR szFileName[128] = "Line.bin";

//bin存取struct
BOOL LoadData()
{
	FILE *fp = fopen(szFileName, "rb");
	if (fp)
	{
		fread(&gLine, sizeof(MYLINE), 1, fp);
		fclose(fp);
		return TRUE;
	}
	return TRUE;
}
BOOL SaveData()
{
	FILE *fp = fopen(szFileName, "wb");
	if (fp)
	{
		fwrite(&gLine, sizeof(MYLINE), 1, fp);
		fclose(fp);
		return TRUE;
	}

	return TRUE;
}

//关于
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

//字体颜色选择框初始化
BOOL CALLBACK ColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  iColor, iFigure;

	switch (message)
	{
	case WM_INITDIALOG:
		iColor = iCurrentColor;
		CheckRadioButton(hDlg, IDC_CC_RED, IDC_CC_BLACK, iColor);
		SetFocus(GetDlgItem(hDlg, iColor));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CCOK:
			iCurrentColor = iColor;
			EndDialog(hDlg, TRUE);
			return TRUE;

		case ID_CCCANCEL:
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;

		case IDC_CC_RED:
		case IDC_CC_GREEN:
		case IDC_CC_BLACK:
			iColor = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_CC_RED, IDC_CC_BLACK, LOWORD(wParam));
			return TRUE;
		}
		break;

	}
	return FALSE;

}
//背景颜色选择框初始化
BOOL CALLBACK BColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemTextA(hDlg, IDC_CCB_EDIT, "请输入颜色");
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_CCB_OK:
			// Get the characters. 
			iBackCurrentColor = GetDlgItemInt(hDlg, IDC_CCB_EDIT, NULL, TRUE);
			InvalidateRect(GetParent(hDlg), NULL, TRUE);
			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDM_CCB_CANCEL:
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

