#pragma once
#include <Windows.h>
#include <string.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

class CShape
{
protected:
	Pen *pen;
	Brush *brush;
	POINT startPoint;				
	POINT endPoint;		
	bool pressedShift = false;
public:
	void setPresShift(bool);
	virtual void Draw(Graphics *shape) = 0;
	
	CShape();
	~CShape();
};

class CLine : public CShape
{
public:
	void Draw(Graphics*);
	CLine();
	CLine(POINT, POINT);
	~CLine();
};

class CEllipse : public CShape
{
public:
	CEllipse();
	CEllipse(POINT, POINT);		
	
	void Draw(Graphics*);
	~CEllipse();
};

class CRetangle :public CShape
{
public:
	CRetangle();
	CRetangle(POINT, POINT);
	
	void Draw(Graphics*);
	~CRetangle();
};


// Aid function:
void ChangeEndIfShift(POINT start, POINT &end);