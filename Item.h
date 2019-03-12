#pragma once

#include "stdafx.h"

typedef struct _Tank Tank;
typedef struct _LogicSprite LogicSprite;

typedef enum _ITEMSTYLE {
	ITEM_HEART
} ITEMSTYLE;

typedef struct _Item {
	ITEMSTYLE m_item_style_;
	LogicSprite* m_super_;
	BOOLean m_valid_;

	// 获得时触发的事件。
	void (*OnGet)(Tank* t);
} Item;

LogicSprite* CreateItem(int x, int y, ITEMSTYLE is);