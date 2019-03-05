#pragma once

#include "stdafx.h"

// ̹�˵Ļ�������(�������)�������͡�
typedef struct _TankData {
	int m_HP_, m_HPmax_;
	// ������/���롣
	int m_shootInterval_;
	int m_atk_;
	// ���ٶȡ�
	int m_speed_;
} TankData;

// ������ݡ�
void FillData();

extern TankData g_strongholdData_;
extern TankData g_playerTankData_;
extern TankData g_junkTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_bigTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_prismTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_fiveTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_quickTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_lanlingkingTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_attackTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_sunTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_bossTankData_[V6_DIFFICULTY_COUNT];