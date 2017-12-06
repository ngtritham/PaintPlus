#include "stdafx.h"
#include "Shape.h"
#include <string.h>

CShape::CShape()
{
	pen = new Pen(Color(255, 0, 0, 0));
	brush = new SolidBrush(Color(255, 0, 0, 0));
}
CShape::~CShape()
{
	delete pen;
	delete brush;
}


void CShape::setPresShift(bool flag) {
	this->pressedShift = flag;
}

// Sub functions
void ChangeEndIfShift(POINT start, POINT &end) {
	int width = abs(start.x - end.x);
	int height = abs(start.y - end.y);

	if (start.x > end.x) {
		if (width > height) {
			end.x = start.x - height;
		}
		else {
			if (start.y > end.y) {
				end.y = start.y - width;
			}
			else {
				end.y = start.y + width;
			}
		}
	}

	else {
		if (width > height) {
			end.x = start.x + height;
		}
		else {
			if (start.y > end.y) {
				end.y = start.y - width;
			}
			else {
				end.y = start.y + width;
			}
		}
	}
}