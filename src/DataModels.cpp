#include <string.h>
#include "Game.h"
#include "Tank.h"

static void SetActiveRespawns1();
static void SetActiveRespawns3();
static void SetActiveRespawns5();
static void SetActiveRespawns6();
static void GainWallCount3(int x, int y);
static void GainWallCount4(int x, int y);
static void GainWallCount5(int x, int y);
static void GenerateItemHeartAndGWC3(int x, int y);
static void GenerateItemHeartAndGWC4(int x, int y);
static void GenerateItemHeartAndGWC5(int x, int y);

static void FillTankDatum(TankData* td, int HPmax, int shootInterval, int bulletSpeed, int atk, int speed, int score, double ratio = 1.0) {
	td->m_HPmax_ = HPmax;
	td->m_atk_ = atk;
	td->m_shootInterval_ = shootInterval;
	td->m_bulletSpeed_ = bulletSpeed;
	td->m_speed_ = speed;
	td->m_damageRatioToBuilding_ = ratio;
	td->m_score_ = score;
}

static void FillTankData() {
	FillTankDatum(&g_strongholdData_, 1, 0, 0, 0, 0, 0);
	FillTankDatum(&g_playerTankData_, 1000, 300, 800, 100, 400, 0);
	FillTankDatum(&g_wallData_, 2400, 0, 0, 0, 0, 1);

	FillTankDatum(&g_junkTankData_[0], 100, 1500, 500, 50, 200, 100);
	FillTankDatum(&g_junkTankData_[1], 300, 1000, 550, 80, 250, 150);
	FillTankDatum(&g_junkTankData_[2], 300, 400, 650, 100, 300, 200);

	FillTankDatum(&g_bigTankData_[0], 600, 1500, 200, 100, 100, 200);
	FillTankDatum(&g_bigTankData_[1], 750, 1200, 280, 140, 120, 300);
	FillTankDatum(&g_bigTankData_[2], 900, 1000, 320, 180, 150, 400);

	FillTankDatum(&g_prismTankData_[0], 400, 800, 280, 70, 350, 300, 0.5);
	FillTankDatum(&g_prismTankData_[1], 500, 800, 300, 90, 400, 450, 0.5);
	FillTankDatum(&g_prismTankData_[2], 700, 700, 350, 120, 520, 600, 0.5);

	FillTankDatum(&g_fiveTankData_[0], 800, 1200, 400, 80, 180, 400, 0.25);
	FillTankDatum(&g_fiveTankData_[1], 1000, 1200, 450, 100, 220, 600, 0.25);
	FillTankDatum(&g_fiveTankData_[2], 1300, 1000, 500, 110, 250, 800, 0.25);

	FillTankDatum(&g_quickTankData_[0], 700, 500, 200, 80, 440, 480, 0.25);
	FillTankDatum(&g_quickTankData_[1], 900, 400, 200, 100, 480, 720, 0.25);
	FillTankDatum(&g_quickTankData_[2], 1200, 250, 200, 120, 550, 960, 0.25);

	FillTankDatum(&g_lanlingkingTankData_[0], 500, 700, 500, 60, 120, 300, 0.5);
	FillTankDatum(&g_lanlingkingTankData_[1], 600, 600, 500, 80, 160, 450, 0.5);
	FillTankDatum(&g_lanlingkingTankData_[2], 650, 500, 550, 100, 300, 600, 0.5);

	FillTankDatum(&g_attackTankData_[0], 600, 1400, 600, 100, 350, 480);
	FillTankDatum(&g_attackTankData_[1], 800, 1200, 600, 120, 400, 720);
	FillTankDatum(&g_attackTankData_[2], 1000, 900, 650, 150, 450, 960);

	FillTankDatum(&g_sunTankData_[0], 1000, 3000, 180, 160, 200, 400);
	FillTankDatum(&g_sunTankData_[1], 1250, 2500, 200, 190, 200, 600);
	FillTankDatum(&g_sunTankData_[2], 1500, 2000, 250, 220, 200, 800);

	FillTankDatum(&g_bossTankData_[0], 60000, 0, 0, 120, 150, 10000, 0.25);
	FillTankDatum(&g_bossTankData_[1], 80000, 0, 0, 140, 150, 20000, 0.25);
	FillTankDatum(&g_bossTankData_[2], 100000, 0, 0, 180, 150, 40000, 0.25);
}

static void FillWaveDatum(TankWave * tw, int wdata[V6_TANKSTYLE_COUNT]) {
	int i;
	for (i = 0; i < V6_TANKSTYLE_COUNT; ++i) {
		tw->m_tank_num_[i] = wdata[i];
		tw->m_tank_total_count_ += wdata[i];
	}
}

inline void ClearWD(int wd[V6_TANKSTYLE_COUNT]) {
	memset(wd, 0, sizeof(int) * V6_TANKSTYLE_COUNT);
}

static void FillWaveData() {
	int wd[V6_TANKSTYLE_COUNT];
	int j;

	for (j = 0; j < V6_GAME_MAX_WAVE; ++j)
		g_waveData_[0][j].OnClear = GainWallCount5;
	for (j = 0; j < V6_GAME_MAX_WAVE; ++j)
		g_waveData_[1][j].OnClear = GainWallCount4;
	for (j = 0; j < V6_GAME_MAX_WAVE; ++j)
		g_waveData_[2][j].OnClear = GainWallCount4;

	ClearWD(wd);
	wd[TANK_JUNK] = 2;
	FillWaveDatum(&g_waveData_[0][1], wd);
	g_waveData_[0][1].OnStart = SetActiveRespawns3;
	ClearWD(wd);
	wd[TANK_JUNK] = 6;
	FillWaveDatum(&g_waveData_[1][1], wd);
	g_waveData_[1][1].OnStart = SetActiveRespawns3;
	ClearWD(wd);
	wd[TANK_JUNK] = 10;
	FillWaveDatum(&g_waveData_[2][1], wd);
	g_waveData_[2][1].OnStart = SetActiveRespawns5;

	ClearWD(wd);
	wd[TANK_JUNK] = 10;
	FillWaveDatum(&g_waveData_[0][2], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 15;
	FillWaveDatum(&g_waveData_[1][2], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 20;
	FillWaveDatum(&g_waveData_[2][2], wd);
	
	ClearWD(wd);
	wd[TANK_JUNK] = 6;
	wd[TANK_BIG] = 4;
	FillWaveDatum(&g_waveData_[0][3], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 7;
	wd[TANK_BIG] = 5;
	FillWaveDatum(&g_waveData_[1][3], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 12;
	wd[TANK_BIG] = 8;
	FillWaveDatum(&g_waveData_[2][3], wd);
	
	ClearWD(wd);
	wd[TANK_JUNK] = 6;
	wd[TANK_BIG] = 2;
	wd[TANK_PRISM] = 2;
	FillWaveDatum(&g_waveData_[0][4], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 7;
	wd[TANK_BIG] = 3;
	wd[TANK_PRISM] = 5;
	FillWaveDatum(&g_waveData_[1][4], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 8;
	wd[TANK_BIG] = 2;
	wd[TANK_PRISM] = 6;
	FillWaveDatum(&g_waveData_[2][4], wd);
	
	ClearWD(wd);
	wd[TANK_JUNK] = 4;
	wd[TANK_PRISM] = 6;
	FillWaveDatum(&g_waveData_[0][5], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 8;
	wd[TANK_PRISM] = 8;
	FillWaveDatum(&g_waveData_[1][5], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 12;
	wd[TANK_PRISM] = 8;
	FillWaveDatum(&g_waveData_[2][5], wd);
	
	ClearWD(wd);
	wd[TANK_JUNK] = 2;
	wd[TANK_PRISM] = 2;
	wd[TANK_FIVE] = 1;
	FillWaveDatum(&g_waveData_[0][6], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 6;
	wd[TANK_PRISM] = 4;
	wd[TANK_FIVE] = 2;
	FillWaveDatum(&g_waveData_[1][6], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 8;
	wd[TANK_PRISM] = 6;
	wd[TANK_FIVE] = 4;
	FillWaveDatum(&g_waveData_[2][6], wd);
	
	ClearWD(wd);
	wd[TANK_BIG] = 5;
	wd[TANK_FIVE] = 3;
	FillWaveDatum(&g_waveData_[0][7], wd);
	ClearWD(wd);
	wd[TANK_BIG] = 6;
	wd[TANK_FIVE] = 4;
	FillWaveDatum(&g_waveData_[1][7], wd);
	ClearWD(wd);
	wd[TANK_JUNK] = 6;
	wd[TANK_BIG] = 6;
	wd[TANK_FIVE] = 6;
	FillWaveDatum(&g_waveData_[2][7], wd);
	
	ClearWD(wd);
	wd[TANK_QUICK] = 6;
	wd[TANK_PRISM] = 2;
	FillWaveDatum(&g_waveData_[0][8], wd);
	ClearWD(wd);
	wd[TANK_QUICK] = 6;
	wd[TANK_PRISM] = 5;
	FillWaveDatum(&g_waveData_[1][8], wd);
	ClearWD(wd);
	wd[TANK_QUICK] = 7;
	wd[TANK_PRISM] = 8;
	wd[TANK_FIVE] = 2;
	FillWaveDatum(&g_waveData_[2][8], wd);
	
	ClearWD(wd);
	wd[TANK_BIG] = 6;
	wd[TANK_FIVE] = 4;
	FillWaveDatum(&g_waveData_[0][9], wd);
	g_waveData_[0][9].OnClear = GenerateItemHeartAndGWC5;
	ClearWD(wd);
	wd[TANK_BIG] = 9;
	wd[TANK_FIVE] = 6;
	FillWaveDatum(&g_waveData_[1][9], wd);
	g_waveData_[1][9].OnClear = GenerateItemHeartAndGWC4;
	ClearWD(wd);
	wd[TANK_SUN] = 8;
	wd[TANK_FIVE] = 6;
	FillWaveDatum(&g_waveData_[2][9], wd);
	g_waveData_[2][9].OnClear = GenerateItemHeartAndGWC3;
	
	ClearWD(wd);
	wd[TANK_PRISM] = 10;
	FillWaveDatum(&g_waveData_[0][10], wd);
	ClearWD(wd);
	wd[TANK_PRISM] = 10;
	FillWaveDatum(&g_waveData_[1][10], wd);
	g_waveData_[1][10].OnStart = SetActiveRespawns5;
	ClearWD(wd);
	wd[TANK_PRISM] = 15;
	FillWaveDatum(&g_waveData_[2][10], wd);
}

void FillData()
{
	FillTankData();
	FillWaveData();
}

TankData g_strongholdData_;
TankData g_playerTankData_;
TankData g_wallData_;
TankData g_junkTankData_[V6_DIFFICULTY_COUNT];
TankData g_bigTankData_[V6_DIFFICULTY_COUNT];
TankData g_prismTankData_[V6_DIFFICULTY_COUNT];
TankData g_fiveTankData_[V6_DIFFICULTY_COUNT];
TankData g_quickTankData_[V6_DIFFICULTY_COUNT];
TankData g_lanlingkingTankData_[V6_DIFFICULTY_COUNT];
TankData g_attackTankData_[V6_DIFFICULTY_COUNT];
TankData g_sunTankData_[V6_DIFFICULTY_COUNT];
TankData g_bossTankData_[V6_DIFFICULTY_COUNT];

TankWave g_waveData_[V6_DIFFICULTY_COUNT][V6_GAME_MAX_WAVE + 1];

void SetActiveRespawns1() {
	m_activeRespawn_ = 1;
}

void SetActiveRespawns3() {
	m_activeRespawn_ = 3;
}

void SetActiveRespawns5() {
	m_activeRespawn_ = 5;
}

void SetActiveRespawns6() {
	m_activeRespawn_ = 6;
}

void GainWallCount3(int x, int y)
{
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 3;
}
void GainWallCount4(int x, int y)
{
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 4;
}
void GainWallCount5(int x, int y)
{
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 5;
}
void GenerateItemHeartAndGWC3(int x, int y)
{
	GenerateItem(x, y, ITEM_HEART);
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 3;
}
void GenerateItemHeartAndGWC4(int x, int y)
{
	GenerateItem(x, y, ITEM_HEART);
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 4;
}
void GenerateItemHeartAndGWC5(int x, int y)
{
	GenerateItem(x, y, ITEM_HEART);
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ += 5;
}