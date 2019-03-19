#include <stdlib.h>
#include <string.h>
#include "Trigger.h"

Trigger* CreateTrigger(BOOLean(*check)(Trigger*, int), void(*act)(Trigger*, int),
	int maxtimes, void* bingingobj, int exd1, int exd2, double exd3, double exd4, double exd5)
{
	Trigger* t = (Trigger*)malloc(sizeof(Trigger));
	memset(t, 0, sizeof(Trigger));
	t->Check = check;
	t->Act = act;
	t->m_trigger_max_times_ = maxtimes;
	t->m_binging_obj_ = bingingobj;
	t->m_exdata1_ = exd1;
	t->m_exdata2_ = exd2;
	t->m_exdata3_ = exd3;
	t->m_exdata4_ = exd4;
	t->m_exdata5_ = exd5;
	t->m_valid_ = TRUE;
	return t;
}
