#pragma once

#include "ResourceManager.h"

typedef struct _LogicSprite LogicSprite;

typedef enum _BULLETSTYLE {
	SMALL_BULLET, BIG_BULLET
} BULLETSTYLE;

typedef struct _Bullet{
	void(*Update)(struct _Bullet* b);

	BULLETSTYLE m_bulletStyle_;
	// ����ʵ�塣
	void* m_me_;
	// ����ָ�롣
	LogicSprite* m_super_;

	BOOLean m_ignore_wall_;

	double m_speedX_, m_speedY_;
} Bullet;