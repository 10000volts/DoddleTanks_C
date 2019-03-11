#pragma once

#include "stdafx.h"

#define V6_TANKSTYLE_COUNT		12

typedef enum _TANKSTYLE {
	TANK_STRONGHOLD, TANK_PLAYER, TANK_WALL, TANK_JUNK, TANK_BIG, TANK_PRISM,
	TANK_FIVE, TANK_QUICK, TANK_LANLINGKING, TANK_ATTACK, TANK_SUN,
	TANK_BOSS
} TANKSTYLE;

// ̹�˵Ļ�������(�������)�������͡�
typedef struct _TankData {
	int m_HPmax_;
	// ������/���롣
	int m_shootInterval_;
	// �ӵ��ٶȡ�
	int m_bulletSpeed_;
	int m_atk_;
	// ���ٶȡ�
	double m_speed_;
	// �Խ������˺�������
	double m_damageRatioToBuilding_;
	// ����������
	int m_score_;
} TankData;

// ������ݡ�
void FillData();

extern TankData g_strongholdData_;
extern TankData g_playerTankData_;
extern TankData g_wallData_;
extern TankData g_junkTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_bigTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_prismTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_fiveTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_quickTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_lanlingkingTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_attackTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_sunTankData_[V6_DIFFICULTY_COUNT];
extern TankData g_bossTankData_[V6_DIFFICULTY_COUNT];

typedef struct _TankWave {
	void (*OnStart)();
	// ���(���һ���з�̹�˱��ݻ���(x, y))ʱ�������¼���
	void (*OnClear)(int x, int y);
	int m_tank_num_[V6_TANKSTYLE_COUNT];
	int m_tank_total_count_;
} TankWave;

extern TankWave g_waveData_[V6_DIFFICULTY_COUNT][V6_GAME_MAX_WAVE + 1];