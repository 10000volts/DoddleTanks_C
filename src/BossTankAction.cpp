#include "BossTankAction.h"
#include "Trigger.h"
#include "TankAction.h"
#include "Tank.h"
#include "RandomEngine.h"
#include <math.h>
#include "Utility.h"

#define V6_BOSS_MAX_ATTACK_MODE			7
// 已完成当前攻击模式(该切换攻击模式了)。
#define V6_BOSS_ATTACK_MODE_COMPLETE	100
// BOSS的1号发射点坐标。
#define V6_BOSS_SHOOT_POINT1_X			29.0
#define V6_BOSS_SHOOT_POINT1_Y			78.0
#define V6_BOSS_SHOOT_POINT2_X			493.0
#define V6_BOSS_SHOOT_POINT2_Y			66.0
#define V6_BOSS_SHOOT_POINT3_X			281.0
#define V6_BOSS_SHOOT_POINT3_Y			59.0

typedef struct _BossTank {
	// 攻击模式。0：扫荡 1：摇摆 2：双重扫荡 3：反弹 4：翅膀 5：锁链
	int m_attack_mode_;
	// 当前攻击的攻击进度。
	int m_attack_process_;

	// 能够发射的最快子弹速度。
	double m_fast1_bullet_speed_;
	double m_fast2_bullet_speed_;
	double m_mid1_bullet_speed_;
	double m_mid2_bullet_speed_;
	double m_slow1_bullet_speed_;
	// 能够发射的最慢子弹速度。
	double m_slow2_bullet_speed_;

	int m_attack0_interval_;
	// 扫射角速度。
	double m_attack0_angle_speed_;

	int m_attack1_interval_;
	// 扫荡攻击的额外随机弹射击频率。
	int m_attack1_rand_frequence_;
	// 扫荡攻击的扫射角速度。
	double m_attack1_angle_speed_;
	// 是否在顺时针转动？
	BOOLean m_attack1_clockwise_;
} BossTank;

static void BossAttack0(int t);
static void BossAttack1(int t);
static void BossAttack2(int t);
static void BossAttack3(int t);
static void BossAttack4(int t);
static void BossAttack5(int t);
static void InitBossAttack0();
static void InitBossAttack1();
static void InitBossAttack2();
static void InitBossAttack3();
static void InitBossAttack4();
static void InitBossAttack5();

static Tank* boss;
static BossTank* ex;

static Tank * CreateBossTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_BOSS, NULL, ls, g_bossTankData_[g_gameDifficulty_],
		149, 93, 305, 37, 0, 0, 0, 0,
		0.0);
	t->Decide = TankBossAI;
	t->Act = TankBossAct;
	BossTank* ext = (BossTank*)malloc(sizeof(BossTank));
	memset(ext, 0, sizeof(BossTank)); 
	if (t->m_data_.m_exdata_ == GDFT_EASY) {
		ext->m_fast1_bullet_speed_ = 550.0;
		ext->m_fast2_bullet_speed_ = 500.0;
		ext->m_mid1_bullet_speed_ = 400.0;
		ext->m_mid2_bullet_speed_ = 300.0;
		ext->m_slow1_bullet_speed_ = 200.0;
		ext->m_slow2_bullet_speed_ = 100.0;

		ext->m_attack0_interval_ = 150;
		ext->m_attack0_angle_speed_ = 0.2;

		ext->m_attack1_interval_ = 60;
		ext->m_attack1_rand_frequence_ = 100;
		ext->m_attack1_angle_speed_ = 1.678;
	}
	else if (t->m_data_.m_exdata_ == GDFT_VEASY) {
		ext->m_fast1_bullet_speed_ = 600.0;
		ext->m_fast2_bullet_speed_ = 540.0;
		ext->m_mid1_bullet_speed_ = 420.0;
		ext->m_mid2_bullet_speed_ = 320.0;
		ext->m_slow1_bullet_speed_ = 250.0;
		ext->m_slow2_bullet_speed_ = 150.0;

		ext->m_attack0_interval_ = 100;
		ext->m_attack0_angle_speed_ = 0.15;

		ext->m_attack1_interval_ = 50;
		ext->m_attack1_rand_frequence_ = 100;
		ext->m_attack1_angle_speed_ = 1.678;
	}
	else if (t->m_data_.m_exdata_ == GDFT_EEASY) {
		ext->m_fast1_bullet_speed_ = 600.0;
		ext->m_fast2_bullet_speed_ = 540.0;
		ext->m_mid1_bullet_speed_ = 480.0;
		ext->m_mid2_bullet_speed_ = 400.0;
		ext->m_slow1_bullet_speed_ = 300.0;
		ext->m_slow2_bullet_speed_ = 200.0;

		ext->m_attack0_interval_ = 100;
		ext->m_attack0_angle_speed_ = 0.1;

		ext->m_attack1_interval_ = 40;
		ext->m_attack1_rand_frequence_ = 100;
		ext->m_attack1_angle_speed_ = 1.678;
	}
	ext->m_attack_mode_ = 0; 
	t->m_extra_ = ext;
	boss = t;
	ex = ext;
	InitBossAttack0();
	InitBossAttack2();
	return t;
}

void BossDead() {
	m_success_ = TRUE;
}
BOOLean TriggerCheckBossCome3(Trigger* tr, int t) {
	if (tr->m_exdata1_ > t) {
		tr->m_exdata1_ -= t;
		return FALSE;
	}
	else {
		m_playingAnimation_ = FALSE;
		tr->m_valid_ = FALSE;
		return FALSE;
	}
}
BOOLean TriggerCheckBossCome2(Trigger* tr, int t) {
	Tank* boss = (Tank*)tr->m_binging_obj_;
	if (boss->m_super_->m_y_ <= 20.0) {
		return TRUE;
	}
	else {
		Trigger* trnew = CreateTrigger(TriggerCheckBossCome3, NULL, -1, NULL, 2000);
		AddElement(m_triggerList_, trnew);
		tr->m_valid_ = FALSE;
		return FALSE;
	}
}
void TriggerActBossCome2(Trigger* tr, int t) {
	Tank* boss = (Tank*)tr->m_binging_obj_;
	boss->m_super_->m_y_ += 50.0 * t / 1000.0;
}
BOOLean TriggerCheckBossCome(Trigger* tr, int t) {
	int i;
	Tank* tk;
	for (i = 0; i < m_neutralTankList_->m_count_; ++i) {
		tk = (Tank*)m_neutralTankList_->m_me_[i];
		// 摧毁不在据点周围的墙。
		if (tk->m_super_->m_y_ < 620.0 | tk->m_super_->m_x_ < 240.0 | tk->m_super_->m_x_ > 400.0) {
			return TRUE;
		} 
	}
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 80, -250, 600, 250, RenderWithMask, &g_img_bossTank, &g_img_bossTankMsk);
	Tank* boss = CreateBossTank(ls);
	Trigger* trnew = CreateTrigger(TriggerCheckBossCome2, TriggerActBossCome2, -1, boss);

	ls->m_me_ = boss;

	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, boss);
	AddElement(m_triggerList_, trnew);

	tr->m_valid_ = FALSE;
	return FALSE;
}
void TriggerActBossCome(Trigger* tr, int t) {
	if (tr->m_exdata1_ <= t) {
		int i;
		Tank* tk;
		for (i = 0; i < m_neutralTankList_->m_count_; ++i) {
			tk = (Tank*)m_neutralTankList_->m_me_[i];
			// 摧毁不在据点周围的墙。
			if (tk->m_super_->m_y_ < 620.0 | tk->m_super_->m_x_ < 240.0 | tk->m_super_->m_x_ > 400.0) {
				tk->m_HP -= 599;
				if (tk->m_HP <= 0)tk->m_valid_ = FALSE;
			}
		}
		tr->m_exdata1_ = 1000;
	}
	tr->m_exdata1_ -= t;
}
void BossCome()
{
	m_playingAnimation_ = TRUE;
	m_special_wave_ = TRUE;

	Trigger* tr = CreateTrigger(TriggerCheckBossCome, TriggerActBossCome, -1);
	AddElement(m_triggerList_, tr);
}

void TankBossAI(Tank * t) {

}

void TankBossAct(int t, Tank * tis) {
	BOOLean f = FALSE;
	ex = (BossTank*)tis->m_extra_;
	if (ex->m_attack_process_ >= V6_BOSS_ATTACK_MODE_COMPLETE) {
		ex->m_attack_mode_ = NextRand() % (V6_BOSS_MAX_ATTACK_MODE - 1) + 1;
		f = TRUE;
	}

	switch (ex->m_attack_mode_)
	{
		// 致死[遥远的辐射]
	case 0:
		if (f) InitBossAttack2();
		BossAttack2(t);
		break;

		// 致幻[催眠摇摆]
	case 1:
		if (f) InitBossAttack0();
		BossAttack0(t);
		break;

		// 怪异[死亡扫描]
	case 2:
		if (f) InitBossAttack2();
		BossAttack2(t);
		break;

		// 现象[镜面反射]
	case 3:
		if (f) InitBossAttack0();
		BossAttack0(t);
		break;

		// 怪奇[远古幻想之翼]
	case 4:
		if (f) InitBossAttack2();
		BossAttack2(t);
		break;

		// 怪异[急迫扫描]
	case 5:
		if (f) InitBossAttack5();
		BossAttack5(t);
		break;

		// 秘术[禁断的古代知识]
	case 6:
		if (f) InitBossAttack2();
		BossAttack2(t);
		break;
	}
}

void InitBossAttack0() {
	boss->m_shoot_angle_ = NextRand() * V6_PI * 2 / V6_RAND_MAX_F;
}

void InitBossAttack1() {

}

void InitBossAttack2() {
	BOOLean f = NextRand() % 2;
	if (f) {
		boss->m_shoot_angle_ = V6_PI + 0.2;
		ex->m_attack1_clockwise_ = FALSE;
	}
	else {
		boss->m_shoot_angle_ = V6_PI * 2 - 0.2;
		ex->m_attack1_clockwise_ = TRUE;
	}
}

void InitBossAttack3() {

}

void InitBossAttack4() {

}

void InitBossAttack5() {

}

void BossAttack0(int t) {
	if (boss->m_shootCD_ == 0) {
		double sa = boss->m_shoot_angle_;
		int i;
		for (i = 0; i < 3; ++i) {
			double sx = ex->m_fast1_bullet_speed_ * cos(sa);
			double sy = -ex->m_fast1_bullet_speed_ * sin(sa);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT3_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT3_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT1_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT1_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			sx = ex->m_fast1_bullet_speed_ * cos(V6_PI / 2 - sa);
			sy = -ex->m_fast1_bullet_speed_ * sin(V6_PI / 2 - sa);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT3_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT3_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT2_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT2_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			sa += V6_PI * 2 / 3;
		}
		boss->m_shootCD_ = ex->m_attack0_interval_;
		boss->m_shoot_angle_ += ex->m_attack0_angle_speed_ * 60.0 / 1000.0;
		boss->m_shoot_angle_ = ConvertIntoStandard(boss->m_shoot_angle_);
	}
}

void BossAttack1(int t) {

}

void BossAttack2(int t) {
	if (boss->m_shootCD_ == 0) {
		double sa = boss->m_shoot_angle_;
		double sx = ex->m_mid1_bullet_speed_ * cos(sa);
		double sy = -ex->m_mid1_bullet_speed_ * sin(sa);
		ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT3_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
			boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT3_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
			boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);

		if (ex->m_attack1_clockwise_) {
			sa -= V6_PI / 4;
			sx = ex->m_mid1_bullet_speed_ * cos(sa);
			sy = -ex->m_mid1_bullet_speed_ * sin(sa);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT3_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT3_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			boss->m_shootCD_ = ex->m_attack1_interval_;
			boss->m_shoot_angle_ -= ex->m_attack1_angle_speed_ * 60.0 / 1000.0;
			if (boss->m_shoot_angle_ <= V6_PI + 0.2) {
				ex->m_attack1_clockwise_ = FALSE;
			}
		} else {
			sa += V6_PI / 4;
			sx = ex->m_mid1_bullet_speed_ * cos(sa);
			sy = -ex->m_mid1_bullet_speed_ * sin(sa);
			ShootSmall(boss, boss->m_super_->m_x_ + V6_BOSS_SHOOT_POINT3_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_super_->m_y_ + V6_BOSS_SHOOT_POINT3_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			boss->m_shootCD_ = ex->m_attack1_interval_;
			boss->m_shoot_angle_ += ex->m_attack1_angle_speed_ * 60.0 / 1000.0;
			if (boss->m_shoot_angle_ >= V6_PI * 2 - 0.2) {
				ex->m_attack1_clockwise_ = TRUE;
			}
		}
	}
}

void BossAttack3(int t) {

}

void BossAttack4(int t) {

}

void BossAttack5(int t) {

}