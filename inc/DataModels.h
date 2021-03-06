#pragma once

#include "stdafx.h"

#define V6_TANKSTYLE_COUNT		12

typedef enum _TANKSTYLE {
	TANK_STRONGHOLD, TANK_PLAYER, TANK_WALL, TANK_JUNK, TANK_BIG, TANK_PRISM,
	TANK_FIVE, TANK_QUICK, TANK_LANLINGKING, TANK_ATTACK, TANK_SUN,
	TANK_BOSS
} TANKSTYLE;

// 坦克的基础属性(面板属性)数据类型。
typedef struct _TankData {
	int m_HPmax_;
	// 射击间隔/毫秒。
	int m_shootInterval_;
	// 子弹速度。
	double m_bulletSpeed_;
	int m_atk_;
	// 秒速度。
	double m_speed_;
	// 对建筑的伤害倍数。
	double m_damageRatioToBuilding_;
	// 奖励分数。
	int m_score_;
	// 额外数据。通常可以用于区分不同难度的不同行为。
	int m_exdata_;
} TankData;

// 填充数据。
void FillData();

extern TankData g_tankData_[V6_DIFFICULTY_COUNT][V6_TANKSTYLE_COUNT];

typedef struct _TankWave {
	void (*OnStart)();
	// 完成(最后一个敌方坦克被摧毁于(x, y))时触发的事件。
	void (*OnClear)(int x, int y);
	int m_tank_num_[V6_TANKSTYLE_COUNT];
	int m_tank_total_count_;
} TankWave;

extern TankWave g_waveData_[V6_DIFFICULTY_COUNT][V6_GAME_MAX_WAVE + 1];