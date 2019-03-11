#pragma once

#include <math.h>
#include "stdafx.h"

// �ж����������Ƿ��н�����
inline BOOLean CollisionDetection(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	BOOLean b1 = ((x1 <= x2) & (x1 + w1 >= x2)) | ((x2 <= x1) & (x2 + w2 >= x1));
	BOOLean b2 = ((y1 <= y2) & (y1 + h1 >= y2)) | ((y2 <= y1) & (y2 + h2 >= y1));
	return b1 & b2;
}

// ��������ƽ����
inline double Distance2(int x1, int y1, int x2, int y2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

// �ж�һ�����κ�һ��Բ���Ƿ��н�����Բ��λ��(x2, y2)��
inline BOOLean CollisionDetectionWithCircle(int x1, int y1, int w1, int h1, int x2, int y2, int r) {
	int x3 = x2 - r, x4 = x2 + r, y3 = y2 - r, y4 = y2 + r, dx, dy;
	if (!CollisionDetection(x1, y1, w1, h1,
		x2 - r, y2 - r, 2 * r, 2 * r)) 
		return FALSE;
	dx = abs(x1 + w1 / 2 - x2); dy = abs(y1 + h1 / 2 - y2);
	if (dx < w1 / 2 | dy < h1 / 2) 
		return TRUE;
	r *= r;
	if (Distance2(x1, y1, x2, y2) <= (double)r)
		return TRUE;
	if (Distance2(x1 + w1, y1, x2, y2) <= (double)r)
		return TRUE;
	if (Distance2(x1, y1 + h1, x2, y2) <= (double)r)
		return TRUE;
	if (Distance2(x1 + w1, y1 + h1, x2, y2) <= (double)r)
		return TRUE;
	return FALSE;
}

// �ж�(x1, y1)�Ƿ���(x2, y2, x2 + w, y2 + h)��Χ�ڡ�
inline BOOLean IsIn(int x1, int y1, int x2, int y2, int w2, int h2) {
	BOOLean b1 = ((x2 <= x1) & (x2 + w2 >= x1));
	BOOLean b2 = ((y2 <= y1) & (y2 + h2 >= y1));
	return b1 & b2;
}

// ������x0, y0Ϊԭ�㣬��Ϊ���ᣬ��ʱ��Ϊ������ļ�����ϵ�У�ֱ������(x1, y1)�ĵ��Ӧ�ĽǶȡ�
inline double CalAngle(int x0, int y0, int x1, int y1) {
	double t = atan2(y1 - y0, x1 - x0);
	return t > 0 ? 2 * V6_PI - t : -t;
}

inline double ConvertIntoStandard(double a) {
	if (a < 0) a += 2 * V6_PI; 
		else if (a >= 2 * V6_PI) a -= 2 * V6_PI;
	return a;
}