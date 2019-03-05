#pragma once

#include "Game.h"
#include "Bullet.h"
#include "DataModels.h"

typedef enum _TANKSTYLE {
	STRONGHOLD, PLAYER_TANK, JUNK_TANK, BIG_TANK, PRISM_TANK, 
	FIVE_TANK, QUICK_TANK, LANLINGKING_TANK, ATTACK_TANK, SUN_TANK, 
	BOSS_TANK
} TANKSTYLE;

typedef struct _Tank {
	void (*Update)(struct _Tank* tis);

	TANKSTYLE m_tankStyle_;
	// 真正实体。
	void* m_me_;
	// 父类指针。
	LogicSprite* m_super_;

	TankData m_data_;

	// 子弹碰撞判定的右侧。
	int m_bulletCollideR, m_bulletCollideT, m_bulletCollideL, m_bulletCollideB;
	// 墙体碰撞判定的右侧。
	int m_CollideR, m_CollideT, m_CollideL, m_CollideB;
	DIRECTION m_orientation_;
} Tank;

typedef struct _PrismTank {

};