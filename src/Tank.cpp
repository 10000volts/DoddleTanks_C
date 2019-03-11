#include "Tank.h"
#include "TankAction.h"
#include "RandomEngine.h"

Tank* CreateTank(TANKSTYLE tankstyle, void * me, LogicSprite * super, TankData td, 
	int bcl, int bct, int bcw, int bch,
	int cl, int ct, int cw, int ch,
	double ori, void(*decide)(Tank* tis), void(*act)(int t, Tank* tis), void(*dead)(Tank* tis))
{
	Tank* t = (Tank*)malloc(sizeof(Tank));
	memset(t, 0, sizeof(Tank));
	t->m_tankStyle_ = tankstyle;
	t->m_extra_ = me;
	t->m_super_ = super;
	t->m_data_ = td;
	t->m_HP = td.m_HPmax_;
	t->m_srcBulletCollideL = bcl;
	t->m_srcBulletCollideT = bct;
	t->m_srcBulletCollideW = bcw;
	t->m_srcBulletCollideH = bch;
	t->m_srcCollideL = cl;
	t->m_srcCollideT = ct;
	t->m_srcCollideW = cw;
	t->m_srcCollideH = ch;
	t->m_bulletCollideL = bcl;
	t->m_bulletCollideT = bct;
	t->m_bulletCollideW = bcw;
	t->m_bulletCollideH = bch;
	t->m_collideL = cl;
	t->m_collideT = ct;
	t->m_collideW = cw;
	t->m_collideH = ch;
	t->m_orientation_ = ori;
	t->Decide = decide;
	t->Act = act;
	t->Dead = dead;
	t->m_valid_ = TRUE;
	return t;
}

BOOLean TryShoot(Tank * t)
{
	return BOOLean();
}

Tank * CreateStronghold(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_STRONGHOLD, NULL, ls, g_strongholdData_,
		7, 25, 45, 38, 7, 25, 45, 38,
		0.0);
	return t;
}

Tank* CreatePlayerTank(LogicSprite* ls) {
	Tank* t = CreateTank(TANK_PLAYER, NULL, ls, g_playerTankData_,
		36, 36, 10, 10, 16, 18, 50, 44,
		V6_DRT_UP);
	t->m_noOrientation_ = TRUE;
	PlayerTank* pt = (PlayerTank*)malloc(sizeof(PlayerTank));
	memset(pt, 0, sizeof(PlayerTank));
	pt->m_palstance_ = 2 * V6_PI;
	t->m_extra_ = pt;
	return t;
}

Tank * CreateJunkTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_JUNK, NULL, ls, g_junkTankData_[g_gameDifficulty_],
		22, 18, 41, 41, 15, 13, 55, 56,
		0.0);
	t->Decide = TankJunkAI;
	t->Act = TankJunkAct;
	ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
	return t;
}
Tank * CreateBigTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_BIG, NULL, ls, g_bigTankData_[g_gameDifficulty_],
		11, 14, 57, 56, 11, 14, 57, 56,
		0.0);
	t->Decide = TankJunkAI;
	t->Act = TankBigAct;
	ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
	return t;
}

Tank * CreatePrismTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_PRISM, NULL, ls, g_prismTankData_[g_gameDifficulty_],
		23, 23, 33, 33, 9, 9, 63, 63,
		0.0);
	t->Decide = TankPrismAI;
	t->Act = TankPrismAct;
	t->m_noOrientation_ = TRUE;
	ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
	return t;
}

Tank * CreateFiveTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_FIVE, NULL, ls, g_fiveTankData_[g_gameDifficulty_],
		9, 10, 35, 53, 4, 6, 69, 66,
		0.0);
	t->Decide = TankFiveAI;
	t->Act = TankFiveAct;
	ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
	return t;
}

//Tank * CreateQuickTank(LogicSprite * ls)
//{
//	//Tank* t = CreateTank(TANK_JUNK, NULL, ls, g_junkTankData_[g_gameDifficulty_],
//	//	22, 18, 41, 41, 15, 13, 55, 56,
//	//	0.0);
//	//t->Decide = TankJunkAI;
//	//t->Act = TankJunkAct;
//	//ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
//	//return t;
//}

Tank * CreateSunTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_JUNK, NULL, ls, g_sunTankData_[g_gameDifficulty_],
		18, 18, 40, 42, 10, 9, 59, 59,
		0.0);
	t->Decide = TankFiveAI;
	t->Act = TankSunAct;
	ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
	return t;
}

//Tank * CreateLanlingkingTank(LogicSprite * ls)
//{
//	//Tank* t = CreateTank(TANK_JUNK, NULL, ls, g_junkTankData_[g_gameDifficulty_],
//	//	23, 23, 33, 33, 9, 9, 63, 63,
//	//	0.0);
//	//t->Decide = TankJunkAI;
//	//t->Act = TankJunkAct;
//	//ChangeTankOrientation(t, double(NextRand() % 4) * V6_DRT_UP);
//	//return t;
//}

void ChangeTankOrientation(Tank * t, double ori)
{
	int i = ori / V6_DRT_UP;
	t->m_orientation_ = ori;
	if (!t->m_noOrientation_) {
		switch (i) {
		case 0:
			t->m_bulletCollideL = t->m_srcBulletCollideL;
			t->m_bulletCollideT = t->m_srcBulletCollideT;
			t->m_bulletCollideW = t->m_srcBulletCollideW;
			t->m_bulletCollideH = t->m_srcBulletCollideH;
			t->m_collideL = t->m_srcCollideL;
			t->m_collideT = t->m_srcCollideT;
			t->m_collideW = t->m_srcCollideW;
			t->m_collideH = t->m_srcCollideH;
			break;
		case 1:
			t->m_bulletCollideL = t->m_srcBulletCollideT;
			t->m_bulletCollideT = t->m_super_->m_w_ - t->m_srcBulletCollideL - t->m_srcBulletCollideW;
			t->m_bulletCollideW = t->m_srcBulletCollideH;
			t->m_bulletCollideH = t->m_srcBulletCollideW;
			t->m_collideL = t->m_srcCollideT;
			t->m_collideT = t->m_super_->m_w_ - t->m_srcCollideL - t->m_srcCollideW;
			t->m_collideW = t->m_srcCollideH;
			t->m_collideH = t->m_srcCollideW;
			break;
		case 2:
			t->m_bulletCollideL = t->m_super_->m_w_ - t->m_srcBulletCollideL - t->m_srcBulletCollideW;
			t->m_bulletCollideT = t->m_srcBulletCollideT;
			t->m_bulletCollideW = t->m_srcBulletCollideW;
			t->m_bulletCollideH = t->m_srcBulletCollideH;
			t->m_collideL = t->m_super_->m_w_ - t->m_srcCollideL - t->m_srcCollideW;
			t->m_collideT = t->m_srcCollideT;
			t->m_collideW = t->m_srcCollideW;
			t->m_collideH = t->m_srcCollideH;
			break;
		case 3:
			t->m_bulletCollideL = t->m_srcBulletCollideL;
			t->m_bulletCollideT = t->m_super_->m_w_ - t->m_srcBulletCollideT - t->m_srcBulletCollideH;
			t->m_bulletCollideW = t->m_srcBulletCollideH;
			t->m_bulletCollideH = t->m_srcBulletCollideW;
			t->m_collideL = t->m_srcCollideL;
			t->m_collideT = t->m_super_->m_w_ - t->m_srcCollideT - t->m_srcCollideH;
			t->m_collideW = t->m_srcCollideH;
			t->m_collideH = t->m_srcCollideW;
			break;
		}
	}
}
