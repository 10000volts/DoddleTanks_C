#include "DataModels.h"

void FillDatum(TankData* td, int m_HP_, int m_HPmax_, int m_shootInterval_, int m_atk_, double m_speed_) {

}

void FillData()
{
	FillDatum(&g_strongholdData_, 1, 1, 0, 0, 0);
	FillDatum(&g_playerTankData_, 1000, 1000, 500, 100, 400);
}

TankData g_strongholdData_;
TankData g_playerTankData_;
TankData g_junkTankData_[V6_DIFFICULTY_COUNT];
TankData g_bigTankData_[V6_DIFFICULTY_COUNT];
TankData g_prismTankData_[V6_DIFFICULTY_COUNT];
TankData g_fiveTankData_[V6_DIFFICULTY_COUNT];
TankData g_quickTankData_[V6_DIFFICULTY_COUNT];
TankData g_lanlingkingTankData_[V6_DIFFICULTY_COUNT];
TankData g_attackTankData_[V6_DIFFICULTY_COUNT];
TankData g_sunTankData_[V6_DIFFICULTY_COUNT];
TankData g_bossTankData_[V6_DIFFICULTY_COUNT];