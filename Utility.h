#pragma once

#include "stdafx.h"

// �ж����������Ƿ��н�����
inline BOOLean CollisionDetection(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	BOOLean b1 = ((x1 <= x2) & (x1 + w1 >= x2)) | ((x2 <= x1) & (x2 + w2 >= x1));
	BOOLean b2 = ((y1 <= y2) & (y1 + h1 >= y2)) | ((y2 <= y1) & (y2 + h2 >= y1));
	return b1 & b2;
}

// �ж�(x1, y1)�Ƿ���(x2, y2, x2 + w, y2 + h)��Χ�ڡ�
inline BOOLean IsIn(int x1, int y1, int x2, int y2, int w2, int h2) {
	BOOLean b1 = ((x2 <= x1) & (x2 + w2 >= x1));
	BOOLean b2 = ((y2 <= y1) & (y2 + h2 >= y1));
	return b1 & b2;
}