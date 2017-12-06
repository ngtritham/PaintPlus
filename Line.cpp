#include "stdafx.h"
#include "Shape.h"

CLine::CLine() {
	startPoint.x = 0;
	startPoint.y = 0;
	endPoint.x = 0;
	endPoint.y = 0;
}

CLine::~CLine() {
}

CLine::CLine(POINT start, POINT end)	{
	startPoint = start;
	endPoint = end;
}


void CLine::Draw(Graphics *graphic) {
	graphic->DrawLine(pen, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
}