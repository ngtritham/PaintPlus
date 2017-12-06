#include "stdafx.h"
#include "Shape.h"

CRetangle::CRetangle() {
	startPoint.x = 0;
	startPoint.y = 0;
	endPoint.x = 0;
	endPoint.y = 0;
}

CRetangle::~CRetangle() {
}

CRetangle::CRetangle(POINT start, POINT end) {
	startPoint = start;
	endPoint = end;
}


void CRetangle::Draw(Graphics *graphic) {
	if (pressedShift) {
		ChangeEndIfShift(startPoint, endPoint);
	}


	POINT topLeft;
	int width = 0;
	int height = 0;

	if (startPoint.x > endPoint.x) {			
		if (startPoint.y > endPoint.y) {
			topLeft = endPoint;
			width = startPoint.x - endPoint.x;
			height = startPoint.y - endPoint.y;
		}
		else {
			topLeft.x = endPoint.x;
			topLeft.y = startPoint.y;
			width = startPoint.x - endPoint.x;
			height = endPoint.y - startPoint.y;
		}
	}

	else {
		if (startPoint.y > endPoint.y) {
			topLeft.x = startPoint.x;
			topLeft.y = endPoint.y;
			width = endPoint.x - startPoint.x;
			height = startPoint.y - endPoint.y;
		}
		else {
			topLeft = startPoint;
			width = endPoint.x - startPoint.x;
			height = endPoint.y - startPoint.y;
		}
	}

	graphic->DrawRectangle(pen, topLeft.x, topLeft.y, width, height);
}