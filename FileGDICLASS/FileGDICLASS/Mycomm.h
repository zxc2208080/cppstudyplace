#ifndef _MY_COMM_H_

#define _MY_COMM_H_

#include <Windows.h>

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


#endif //_MY_COMM_H_