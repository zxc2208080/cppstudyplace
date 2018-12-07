#pragma once
#include "stdafx.h"


class needToDraw
{
protected:
	POINT m_pSLT;
	POINT m_pERB;
	int m_Index;
	char m_szBuffer[64];
	HPEN m_hPen=(HPEN) GetStockObject(BLACK_PEN);
public:
	virtual void DrawMyself(HDC hdc)=0;
	virtual void setHPen(HPEN hPen)=0;
	virtual BOOL isInner(POINT) = 0;
	virtual void setChar(char* szBuffer) = 0;
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
	BOOL isInner(POINT)
	{
		return 0;
	}
	void setChar(char* szBuffer)
	{
		m_szBuffer[64] = NULL;
	};
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
	BOOL isInner(POINT pt)
	{
		if (pt.x>= m_pSLT.x&&
			pt.y>= m_pSLT.y&&
			pt.x<= m_pERB.x&&
			pt.y<= m_pERB.y)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	void setChar(char* szBuffer)
	{
		m_szBuffer[64] = NULL;
	};
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

	BOOL isInner(POINT pt)
	{
		POINT ptCenter = { (m_pSLT.x + m_pERB.x) / 2,(m_pSLT.y + m_pERB.y) / 2 };
		int a = m_pERB.x - ptCenter.x;
		int b = m_pERB.y - ptCenter.y;
		int f = (pt.x - ptCenter.x) ^ 2 / a ^ 2 + (pt.y - ptCenter.y) ^ 2 / b ^ 2;
		if (f>1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	void setChar(char* szBuffer)
	{
		m_szBuffer[64] = NULL;
	};
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
		RECT textRect = {
			m_pSLT.x,
			m_pSLT.y,
			m_pERB.x,
			m_pERB.y,
		};
		
		POINT ptText[5];
		ptText[0].x = m_pSLT.x; ptText[0].y = m_pSLT.y;
		ptText[1].x = m_pSLT.x; ptText[1].y = m_pERB.y;
		ptText[2].x = m_pERB.x; ptText[2].y = m_pERB.y;
		ptText[3].x = m_pERB.x; ptText[3].y = m_pSLT.y;
		ptText[4].x = m_pSLT.x; ptText[4].y = m_pSLT.y;
		Polyline(hdc, ptText, 5);
		DrawText(hdc,m_szBuffer, -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	BOOL isInner(POINT pt)
	{
		if (pt.x >= m_pSLT.x&&
			pt.y >= m_pSLT.y&&
			pt.x <= m_pERB.x&&
			pt.y <= m_pERB.y)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	void setHPen(HPEN hPen)
	{
		this->m_hPen = hPen;
	}
	void setChar(char* szBuffer)
	{
		wsprintf(m_szBuffer,szBuffer);
	};
};
