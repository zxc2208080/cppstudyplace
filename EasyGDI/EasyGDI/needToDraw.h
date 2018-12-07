#pragma once
#include "stdafx.h"
class needToDraw
{
protected:
	POINT m_pSLT;
	POINT m_pERB;
	int m_Index;
	char* m_pBuffer;
	HPEN m_hPen=(HPEN) GetStockObject(BLACK_PEN);
public:
	virtual void DrawMyself(HDC hdc)=0;
	virtual void setHPen(HPEN hPen)=0;
	void setChar(char* pBuffer)
	{

	}
	void setPoint_ERB(POINT pERB)
	{
		this->m_pERB = pERB;
	}
};

class MyLine:public needToDraw
{
public:
	MyLine(int index, POINT pSLT)
	{
		this->m_Index = index;
		this->m_pSLT = pSLT;
	};

	void DrawMyself(HDC hdc)
	{
		SelectObject(hdc, this->m_hPen);
		MoveToEx(hdc, m_pSLT.x, m_pSLT.y, NULL);
		LineTo(hdc, m_pERB.x, m_pERB.y);
	}

	void setHPen(HPEN hPen)
	{
		this->m_hPen = hPen;
	}
};
class MyRect :public needToDraw
{
public:
	MyRect(int index, POINT pSLT)
	{
		this->m_Index = index;
		this->m_pSLT = pSLT;
	};

	void DrawMyself(HDC hdc)
	{
		POINT ptRect[5];
		SelectObject(hdc, this->m_hPen);
		ptRect[0].x = m_pSLT.x; ptRect[0].y = m_pSLT.y;
		ptRect[1].x = m_pSLT.x; ptRect[1].y = m_pERB.y;
		ptRect[2].x = m_pERB.x; ptRect[2].y = m_pERB.y;
		ptRect[3].x = m_pERB.x; ptRect[3].y = m_pSLT.y;
		ptRect[4].x = m_pSLT.x; ptRect[4].y = m_pSLT.y;

		Polyline(hdc, ptRect, 5);
	}

	void setHPen(HPEN hPen)
	{
		this->m_hPen = hPen;
	}
};

class MyEllip :public needToDraw
{
public:
	MyEllip(int index, POINT pSLT)
	{
		this->m_Index = index;
		this->m_pSLT = pSLT;
	};

	void DrawMyself(HDC hdc)
	{
		SelectObject(hdc, this->m_hPen);
		Ellipse(hdc, m_pSLT.x, m_pSLT.y, m_pERB.x, m_pERB.y);
	}

	void setHPen(HPEN hPen)
	{
		this->m_hPen = hPen;
	}
};

class MyText :public needToDraw
{
public:
	MyText(int index, POINT pSLT)
	{
		this->m_Index = index;
		this->m_pSLT = pSLT;
	};

	void DrawMyself(HDC hdc)
	{
		//创建红色点划线画笔
		HPEN hRedDashdotPen = CreatePen(PS_DASHDOT, 1, RGB(255,0, 255));
		SelectObject(hdc, hRedDashdotPen);
		POINT ptText[5];
		ptText[0].x = m_pSLT.x; ptText[0].y = m_pSLT.y;
		ptText[1].x = m_pSLT.x; ptText[1].y = m_pERB.y;
		ptText[2].x = m_pERB.x; ptText[2].y = m_pERB.y;
		ptText[3].x = m_pERB.x; ptText[3].y = m_pSLT.y;
		ptText[4].x = m_pSLT.x; ptText[4].y = m_pSLT.y;
		Polyline(hdc, ptText, 5);
	}

	void setHPen(HPEN hPen)
	{
		this->m_hPen = hPen;
	}
};
