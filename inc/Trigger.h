#pragma once

#include "stdafx.h"

typedef struct _Trigger {
	BOOLean (*Check)(_Trigger* tis, int t);
	void (*Act)(_Trigger* tis, int t);

	// ��󴥷�������
	int m_trigger_max_times_;
	// �󶨶���
	void* m_binging_obj_;
	int m_exdata1_;
	int m_exdata2_;
	double m_exdata3_;
	double m_exdata4_;
	double m_exdata5_;
	BOOLean m_valid_;
} Trigger;

Trigger* CreateTrigger(BOOLean (*check)(Trigger*, int), void (*act)(Trigger*, int),
	int maxtimes, void* bingingobj = NULL, int exd1 = 0, int exd2 = 0.0, double exd3 = 0.0, double exd4 = 0.0, double exd5 = 0.0);