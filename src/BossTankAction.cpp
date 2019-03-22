#include "Game.h"
#include "BossTankAction.h"
#include "Trigger.h"
#include "TankAction.h"
#include "Tank.h"
#include "LogicEngine.h"
#include "RenderEngine.h"
#include "ResourceManager.h"
#include "RandomEngine.h"
#include <math.h>
#include "Utility.h"

#define V6_BOSS_MAX_ATTACK_MODE			7
// 已完成当前攻击模式(该切换攻击模式了)。
#define V6_BOSS_ATTACK_MODE_COMPLETE	100
// BOSS的1号发射点的相对坐标。
#define V6_BOSS_SHOOT_POINT1_X			29.0
#define V6_BOSS_SHOOT_POINT1_Y			78.0
#define V6_BOSS_SHOOT_POINT2_X			493.0
#define V6_BOSS_SHOOT_POINT2_Y			66.0
#define V6_BOSS_SHOOT_POINT3_X			281.0
#define V6_BOSS_SHOOT_POINT3_Y			59.0
// BOSS的1号发射点的绝对坐标。
#define V6_BOSS_SP1ABS_X				109.0
#define V6_BOSS_SP1ABS_Y				98.0
#define V6_BOSS_SP2ABS_X				573.0
#define V6_BOSS_SP2ABS_Y				86.0
#define V6_BOSS_SP3ABS_X				361.0
#define V6_BOSS_SP3ABS_Y				79.0

#define V6_BOSS_ATTACK1_MAX_DISTANCE	720.0 // （768 - 20(Boss的纵坐标) - 59(V6_BOSS_SHOOT_POINT3_Y)）+ 一点点

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

	int m_attack0_t_;
	int m_attack0_interval_;
	// 扫射角速度。
	double m_attack0_angle_speed_;

	// 初始射击的终止角度。
	double m_attack1_end_angle_;
	// 一次射击射出的子弹数目。
	int m_attack1_bullet_count_;
	// 角加速度。
	double m_attack1_angacc_;
	// 最大角速度。
	double m_attack1_max_angle_speed_;
	// 回归弹的角速度。
	double m_attack1_back_angle_speed_;
	int m_attack1_interval_;
	BOOLean m_attack1_clockwise_;

	int m_attack2_interval_;
	// 扫荡攻击的额外随机弹射击频率。
	int m_attack2_rand_frequence_;
	int m_attack2_rand_t_;
	// 扫荡攻击的扫射角速度。
	double m_attack2_angle_speed_;
	// 是否在顺时针转动？
	BOOLean m_attack2_clockwise_;

	int m_attack3_interval_;
	// 瞄准位置坐标。
	double m_attack3_aimx_, m_attack3_aimy_;

	int m_attack4_interval_;
	double m_attack4_angle_speed_;
	double m_attack4_radius_;
	// 发射点。
	double m_attack4_angle_;

	// 停留时间。
	int m_attack5_stop_time_;
	// 是否该停留。
	int m_attack5_stop_f_;
	int m_attack5_interval_;
	// 扫荡攻击的额外随机弹射击频率。
	int m_attack5_rand_frequence_;
	int m_attack5_rand_t_;
	// 扫荡攻击的扫射角速度。
	double m_attack5_angle_speed_;
	// 是否在顺时针转动？
	BOOLean m_attack5_clockwise_;
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
static LogicSprite* bosssuper;

static LogicSprite* HPBar;
static LogicSprite* emptyHPBar;
void BossHPRender(int t, LogicSprite * ls);

static Tank * CreateBossTank(LogicSprite * ls)
{
	Tank* t = CreateTank(TANK_BOSS, NULL, ls, m_tankData_[TANK_BOSS],
		149, 93, 305, 37, 0, 0, 0, 0,
		0.0);
	t->Decide = TankBossAI;
	t->Act = TankBossAct;
	BossTank* ext = (BossTank*)malloc(sizeof(BossTank));
	memset(ext, 0, sizeof(BossTank)); 
	if (t->m_data_.m_exdata_ == GDFT_EASY) {
		ext->m_fast1_bullet_speed_ = 540.0;
		ext->m_fast2_bullet_speed_ = 480.0;
		ext->m_mid1_bullet_speed_ = 400.0;
		ext->m_mid2_bullet_speed_ = 300.0;
		ext->m_slow1_bullet_speed_ = 200.0;
		ext->m_slow2_bullet_speed_ = 100.0;

		ext->m_attack0_interval_ = 150;
		ext->m_attack0_angle_speed_ = 14.0;

		ext->m_attack1_interval_ = 1500;
		ext->m_attack1_bullet_count_ = 4;
		ext->m_attack1_angacc_ = 2.0;
		ext->m_attack1_back_angle_speed_ = 0.4;
		ext->m_attack1_max_angle_speed_ = 1.8;

		ext->m_attack2_interval_ = 80;
		ext->m_attack2_rand_frequence_ = 40;
		ext->m_attack2_angle_speed_ = 20.0;

		ext->m_attack4_interval_ = 140;
		ext->m_attack4_angle_speed_ = 20.0;
		ext->m_attack4_radius_ = 150.0;

		ext->m_attack5_interval_ = 30;
		ext->m_attack5_stop_time_ = 1000;
		ext->m_attack5_rand_frequence_ = 40;
		ext->m_attack5_angle_speed_ = 10.0;
	}
	else if (t->m_data_.m_exdata_ == GDFT_VEASY) {
		ext->m_fast1_bullet_speed_ = 600.0;
		ext->m_fast2_bullet_speed_ = 540.0;
		ext->m_mid1_bullet_speed_ = 420.0;
		ext->m_mid2_bullet_speed_ = 320.0;
		ext->m_slow1_bullet_speed_ = 250.0;
		ext->m_slow2_bullet_speed_ = 150.0;

		ext->m_attack0_interval_ = 120;
		ext->m_attack0_angle_speed_ = 12.0;

		ext->m_attack1_interval_ = 1200;
		ext->m_attack1_bullet_count_ = 4;
		ext->m_attack1_angacc_ = 2.0;
		ext->m_attack1_back_angle_speed_ = 0.4;
		ext->m_attack1_max_angle_speed_ = 2.2;

		ext->m_attack2_interval_ = 70;
		ext->m_attack2_rand_frequence_ = 30;
		ext->m_attack2_angle_speed_ = 20.0;

		ext->m_attack4_interval_ = 100;
		ext->m_attack4_angle_speed_ = 20.0;
		ext->m_attack4_radius_ = 150.0;

		ext->m_attack5_interval_ = 25;
		ext->m_attack5_stop_time_ = 1000;
		ext->m_attack5_rand_frequence_ = 40;
		ext->m_attack5_angle_speed_ = 10.0;
	}
	else if (t->m_data_.m_exdata_ == GDFT_EEASY) {
		ext->m_fast1_bullet_speed_ = 600.0;
		ext->m_fast2_bullet_speed_ = 540.0;
		ext->m_mid1_bullet_speed_ = 480.0;
		ext->m_mid2_bullet_speed_ = 400.0;
		ext->m_slow1_bullet_speed_ = 300.0;
		ext->m_slow2_bullet_speed_ = 200.0;

		ext->m_attack0_interval_ = 100;
		ext->m_attack0_angle_speed_ = 8.0;

		ext->m_attack1_interval_ = 1000;
		ext->m_attack1_bullet_count_ = 4;
		ext->m_attack1_angacc_ = 2.0;
		ext->m_attack1_back_angle_speed_ = 0.3;
		ext->m_attack1_max_angle_speed_ = 2.5;

		ext->m_attack2_interval_ = 60;
		ext->m_attack2_rand_frequence_ = 20;
		ext->m_attack2_angle_speed_ = 20.0;

		ext->m_attack4_interval_ = 80;
		ext->m_attack4_angle_speed_ = 20.0;
		ext->m_attack4_radius_ = 150.0;

		ext->m_attack5_interval_ = 20;
		ext->m_attack5_stop_time_ = 1000;
		ext->m_attack5_rand_frequence_ = 40;
		ext->m_attack5_angle_speed_ = 10.0;
	}
	ext->m_attack_mode_ = 0; 
	t->m_extra_ = ext;
	boss = t;
	ex = ext;
	bosssuper = boss->m_super_;
	// TODO : delete
	InitBossAttack1();
	return t;
}

void BossDead() {
	m_success_ = TRUE;
	RemoveElement(g_logicSpriteManager_, HPBar);
	RemoveElement(g_logicSpriteManager_, emptyHPBar);
}
BOOLean TriggerCheckBossCome3(Trigger* tr, int t) {
	if (tr->m_exdata1_ > t) {
		tr->m_exdata1_ -= t;
		return FALSE;
	}
	else {
		emptyHPBar = CreateLogicSprite(NULL, NULL, bosssuper->m_x_ + 200, bosssuper->m_y_ - 20,
			255, 30, RenderWithMask, &g_img_emptyHp, &g_img_emptyHpMsk);
		AddElement(g_logicSpriteManager_, emptyHPBar);
		HPBar = CreateLogicSprite(NULL, BossHPRender, bosssuper->m_x_ + 200, bosssuper->m_y_ - 20,
			V6_PROGRESSBAR_WIDTH, 30, RenderWithScaling, &g_img_hp, &g_img_hpMsk);
		AddElement(g_logicSpriteManager_, HPBar);

		m_playingAnimation_ = FALSE;
		tr->m_valid_ = FALSE;
		return FALSE;
	}
}
BOOLean TriggerCheckBossCome2(Trigger* tr, int t) {
	Tank* boss = (Tank*)tr->m_binging_obj_;
	if (bosssuper->m_y_ <= 20.0) {
		return TRUE;
	}
	else {
		Trigger* trnew = CreateTrigger(TriggerCheckBossCome3, NULL, -1, NULL, 2000);
		AddElement(m_triggerList_, trnew);
		bosssuper->m_y_ = 20.0;
		tr->m_valid_ = FALSE;
		return FALSE;
	}
}
void TriggerActBossCome2(Trigger* tr, int t) {
	Tank* boss = (Tank*)tr->m_binging_obj_;
	bosssuper->m_y_ += 50.0 * double(t) / 1000.0;
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
	if (ex->m_attack_process_ >= V6_BOSS_ATTACK_MODE_COMPLETE & m_enemyBulletList_->m_count_ == 0) {
		ex->m_attack_mode_ = NextRand() % (V6_BOSS_MAX_ATTACK_MODE - 1) + 1;
		ex->m_attack_process_ = 0;
		f = TRUE;
	}

	switch (ex->m_attack_mode_)
	{
		// 致死[遥远的辐射]
	case 0:
		if (f) InitBossAttack4();
		BossAttack4(t);
		break;

		// 致幻[催眠摇摆]
	case 1:
		if (f) InitBossAttack1();
		BossAttack1(t);
		break;

		// 怪异[死亡扫描]
	case 2:
		if (f) InitBossAttack2();
		BossAttack2(t);
		break;

		// 现象[镜面反射]
	case 3:
		if (f) InitBossAttack1();
		BossAttack1(t);
		break;

		// 怪奇[自我吞食者]
	case 4:
		if (f) InitBossAttack0();
		BossAttack0(t);
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
	ex->m_attack0_t_ = 0;
	boss->m_shoot_angle_ = NextRand() * V6_PI * 2.0 / V6_RAND_MAX_F;
}

void InitBossAttack1() {
	BOOLean f = NextRand() % 2;
	if (f) {
		boss->m_shoot_angle_ = V6_PI + 0.4;
		ex->m_attack1_end_angle_ = V6_PI * 2.0 - 0.4;
		ex->m_attack1_clockwise_ = FALSE;
	}
	else {
		boss->m_shoot_angle_ = V6_PI * 2.0 - 0.4;
		ex->m_attack1_end_angle_ = V6_PI + 0.4;
		ex->m_attack1_clockwise_ = TRUE;
	}
}

void InitBossAttack2() {
	BOOLean f = NextRand() % 2;
	if (f) {
		boss->m_shoot_angle_ = V6_PI + 0.2;
		ex->m_attack2_clockwise_ = FALSE;
	}
	else {
		boss->m_shoot_angle_ = V6_PI * 2.0 - 0.2;
		ex->m_attack2_clockwise_ = TRUE;
	}

	ex->m_attack2_rand_t_ = 0;
}

BOOLean TCBossAttack3ServentMoveTo(Trigger* tis, int t) {
	return 0;
}
void TABossAttack3ServentMoveTo(Trigger* tis, int t) {

}
BOOLean TCBossAttack3ServentFade(Trigger* tis, int t) {
	return 0;
}
void TABossAttack3ServentFade(Trigger* tis, int t) {

}
void BossAttack3ShootServent(int x, int y) {
	int stx = bosssuper->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
		sty = bosssuper->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2;
	double sa = CalAngle(stx, sty,
		m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH);
	double sx = ex->m_fast1_bullet_speed_ * cos(sa);
	double sy = -ex->m_fast1_bullet_speed_ * sin(sa);
	LogicSprite* b = CreateBigBullet(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
		V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
		boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
	Trigger* t = CreateTrigger(TCBossAttack3ServentMoveTo, TABossAttack3ServentMoveTo, 1, b);
	AddElement(g_logicSpriteManager_, b);
	AddElement(m_enemyBulletList_, b->m_me_);
}
void InitBossAttack3() {
	ex->m_attack3_aimx_ = V6_BOSS_SP3ABS_X;
	ex->m_attack3_aimy_ = V6_BOSS_SP3ABS_Y;

	BossAttack3ShootServent(0, 400);
	DelayWithTwoArgs(50, BossAttack3ShootServent, 80, 440);
	DelayWithTwoArgs(100, BossAttack3ShootServent, 160, 480);
	DelayWithTwoArgs(150, BossAttack3ShootServent, 240, 520);
}

void InitBossAttack4() {
	ex->m_attack4_angle_ = V6_PI / 2;
}

void InitBossAttack5() {
	boss->m_shoot_angle_ = V6_PI / 2.0;
	BOOLean f = NextRand() % 2;
	if (f) {
		ex->m_attack5_clockwise_ = FALSE;
	}
	else {
		ex->m_attack5_clockwise_ = TRUE;
	}

	ex->m_attack5_rand_t_ = 0;
	ex->m_attack5_stop_time_ = 0;
	ex->m_attack5_stop_f_ = 0;
}

void BossSelfProtectAttack(int t) {
	if (m_playerTank_->m_super_->m_y_ <= bosssuper->m_y_ + bosssuper->m_h_) {
		int stx = V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
			sty = V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2;
		double sa = CalAngle(stx, sty,
			m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH) - V6_PI * 3.0 / 9.0;
		int i;
		for (i = 0; i != 5; ++i) {
			sa += V6_PI / 9.0;
			double sx = ex->m_fast1_bullet_speed_ * cos(sa);
			double sy = -ex->m_fast1_bullet_speed_ * sin(sa);
			ShootSmall(boss, stx, sty,
				boss->m_data_.m_atk_, FALSE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		}
	}
}

void BossAttack0(int t) {
	ex->m_attack0_t_ += t;
	if (ex->m_attack0_t_ >= 20000) ex->m_attack_process_ = V6_BOSS_ATTACK_MODE_COMPLETE;
	if (boss->m_shootCD_ <= 0 & ex->m_attack_process_ < V6_BOSS_ATTACK_MODE_COMPLETE) {
		double sa = boss->m_shoot_angle_;
		int i;
		for (i = 0; i < 3; ++i) {
			double sx = ex->m_fast1_bullet_speed_ * cos(sa);
			double sy = -ex->m_fast1_bullet_speed_ * sin(sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			ShootSmall(boss, V6_BOSS_SP1ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP1ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			sx = ex->m_fast1_bullet_speed_ * cos(V6_PI / 2 - sa);
			sy = -ex->m_fast1_bullet_speed_ * sin(V6_PI / 2 - sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			ShootSmall(boss, V6_BOSS_SP2ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP2ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			sa += V6_PI * 2.0 / 3.0;
		}
		boss->m_shootCD_ += ex->m_attack0_interval_;
		boss->m_shoot_angle_ += ex->m_attack0_angle_speed_ * double(t) / 1000.0;
		if (boss->m_shoot_angle_ < 0) { 
			boss->m_shoot_angle_ += 2.0 * V6_PI;
		}
		else if (boss->m_shoot_angle_ >= 2.0 * V6_PI) {
			boss->m_shoot_angle_ -= 2.0 * V6_PI;
		}
	}
}

static BOOLean TCBossAttack1BulletMove3(Trigger* tis, int t) {
	if (ex->m_attack1_clockwise_) {
		if (ex->m_attack_process_ >= 100) tis->m_exdata3_ += 100.0 * double(t) / 1000.0;
		tis->m_exdata4_ += ex->m_attack1_back_angle_speed_ * double(t) / 1000.0;
	}
	else {
		if (ex->m_attack_process_ >= 100) tis->m_exdata3_ += 100.0 * double(t) / 1000.0;
		tis->m_exdata4_ -= ex->m_attack1_back_angle_speed_ * double(t) / 1000.0;
	}
	return FALSE;
}
static BOOLean TCBossAttack1BulletDelay(Trigger* tis, int t) {
	if (tis->m_exdata1_ > t) {
		tis->m_exdata1_ -= t;
		return FALSE;
	}

	Trigger* tr = CreateTrigger(TCBossAttack1BulletMove3, NULL, -1, tis->m_binging_obj_, 0, 0, tis->m_exdata3_, tis->m_exdata4_);
	AddElement(m_triggerList_, tr);

	((Bullet*)tis->m_binging_obj_)->m_me_ = tr;

	tis->m_valid_ = FALSE;

	return FALSE;
}
// exdata5 : 当前角速度
static BOOLean TCBossAttack1BulletMove2(Trigger* tis, int t) {
	if (ex->m_attack1_clockwise_) {
		if (tis->m_exdata4_ <= ex->m_attack1_end_angle_) {
			Trigger* tr = CreateTrigger(TCBossAttack1BulletDelay, NULL, 1, tis->m_binging_obj_, 2000, 0, tis->m_exdata3_, tis->m_exdata4_);
			AddElement(m_triggerList_, tr);

			((Bullet*)tis->m_binging_obj_)->m_me_ = tr;

			tis->m_valid_ = FALSE;
		}
		else {
			tis->m_exdata4_ -= tis->m_exdata5_ * double(t) / 1000.0;
			tis->m_exdata5_ += ex->m_attack1_angacc_ * double(t) / 1000.0;
			if (tis->m_exdata5_ > ex->m_attack1_max_angle_speed_) tis->m_exdata5_ = ex->m_attack1_max_angle_speed_;
		}
	}
	else {
		if (tis->m_exdata4_ >= ex->m_attack1_end_angle_) {
			Trigger* tr = CreateTrigger(TCBossAttack1BulletDelay, NULL, 1, tis->m_binging_obj_, 2000, 0, tis->m_exdata3_, tis->m_exdata4_);
			AddElement(m_triggerList_, tr);

			((Bullet*)tis->m_binging_obj_)->m_me_ = tr;

			tis->m_valid_ = FALSE;
		}
		else {
			tis->m_exdata4_ += tis->m_exdata5_ * double(t) / 1000.0;
			tis->m_exdata5_ += ex->m_attack1_angacc_ * double(t) / 1000.0;
			if (tis->m_exdata5_ > ex->m_attack1_max_angle_speed_) tis->m_exdata5_ = ex->m_attack1_max_angle_speed_;
		}
	}
	return FALSE;
}
static BOOLean TCBossAttack1BulletMove1(Trigger* tis, int t) {
	Bullet* me = (Bullet*)tis->m_binging_obj_;
	double d = Distance2(me->m_super_->m_x_, me->m_super_->m_y_,
		V6_BOSS_SP3ABS_X, V6_BOSS_SP3ABS_Y);
	if (Distance2(me->m_super_->m_x_, me->m_super_->m_y_,
		V6_BOSS_SP3ABS_X, V6_BOSS_SP3ABS_Y) >= tis->m_exdata3_ * tis->m_exdata3_) {
		return TRUE;
	}
	return FALSE;
}
static void BossAttack1BulletAct1(int t, Bullet* b) {
	Trigger* tr = (Trigger*)b->m_me_;
	// exd3 : 子弹距离boss本体的距离 exd4 : 当前角度
	b->m_super_->m_x_ = V6_BOSS_SP3ABS_X + tr->m_exdata3_ * cos(tr->m_exdata4_);
	b->m_super_->m_y_ = V6_BOSS_SP3ABS_Y - tr->m_exdata3_ * sin(tr->m_exdata4_);
}
static void TABossAttack1BulletMove1(Trigger* tis, int t) {
	// tis->exdata3 : 子弹距离boss本体的距离
	Trigger* tr = CreateTrigger(TCBossAttack1BulletMove2, NULL, -1, tis->m_binging_obj_, 0, 0, tis->m_exdata3_, boss->m_shoot_angle_);
	AddElement(m_triggerList_, tr);

	((Bullet*)tis->m_binging_obj_)->Update = BossAttack1BulletAct1;

	((Bullet*)tis->m_binging_obj_)->m_me_ = tr;
}
static void BossAttack1FreeTrigger(Bullet* b) {
	((Trigger*)b->m_me_)->m_valid_ = FALSE;
}
void BossAttack1(int t) {
	if (ex->m_attack_process_ < V6_BOSS_ATTACK_MODE_COMPLETE) {
		if (boss->m_shootCD_ <= 0) {
			int i;
			for (i = 0; i < ex->m_attack1_bullet_count_; ++i) {
				// 路程
				double s = NextRand() * V6_BOSS_ATTACK1_MAX_DISTANCE / V6_RAND_MAX_F;
				// 速度
				double v = s * 1000.0 / 200.0;
				double sx = v * cos(boss->m_shoot_angle_);
				double sy = -v * sin(boss->m_shoot_angle_);
				LogicSprite* b = CreateBigBullet(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
					V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
					boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate, -1000.0, 0.0, 1000.0, 1000.0);
				Trigger* tr = CreateTrigger(TCBossAttack1BulletMove1, TABossAttack1BulletMove1, 1, b->m_me_, 0, 0, s);
				
				((Bullet*)b->m_me_)->m_me_ = tr;
				((Bullet*)b->m_me_)->Dead = BossAttack1FreeTrigger;

				AddElement(g_logicSpriteManager_, b);
				AddElement(m_enemyBulletList_, b->m_me_);
				AddElement(m_triggerList_, tr);
			}
			boss->m_shootCD_ += ex->m_attack1_interval_;
			ex->m_attack_process_ += 4;
		}
	}
}

void BossAttack2(int t) {
	BossSelfProtectAttack(t);

	if (ex->m_attack_process_ < V6_BOSS_ATTACK_MODE_COMPLETE) {

		ex->m_attack2_rand_t_ -= t;
		if (ex->m_attack2_rand_t_ <= 0) {
			double sa = boss->m_shoot_angle_ + double(NextRand()) / V6_RAND_MAX_F * V6_PI / 6.0 - V6_PI / 12.0;
			double s = NextRand() * (ex->m_mid1_bullet_speed_ - ex->m_slow2_bullet_speed_) / V6_RAND_MAX_F + ex->m_slow2_bullet_speed_;
			double sx = s * cos(sa);
			double sy = -s * sin(sa);
			ex->m_attack2_rand_t_ += ex->m_attack2_rand_frequence_;
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		}
		if (boss->m_shootCD_ <= 0) {
			double sa = boss->m_shoot_angle_ - V6_PI / 12.0;
			double sx = ex->m_mid1_bullet_speed_ * cos(sa);
			double sy = -ex->m_mid1_bullet_speed_ * sin(sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

			sa += V6_PI / 6;
			sx = ex->m_mid1_bullet_speed_ * cos(sa);
			sy = -ex->m_mid1_bullet_speed_ * sin(sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			boss->m_shootCD_ += ex->m_attack2_interval_;
			if (ex->m_attack2_clockwise_) {
				boss->m_shoot_angle_ -= ex->m_attack2_angle_speed_ * double(t) / 1000.0;
				if (boss->m_shoot_angle_ <= V6_PI + 0.2) {
					ex->m_attack2_clockwise_ = FALSE;
					ex->m_attack_process_ += 16;
				}
			}
			else {
				boss->m_shoot_angle_ += ex->m_attack2_angle_speed_ * double(t) / 1000.0;
				if (boss->m_shoot_angle_ >= V6_PI * 2.0 - 0.2) {
					ex->m_attack2_clockwise_ = TRUE;
					ex->m_attack_process_ += 16;
				}
			}
		}
	}
}

void BossAttack3(int t) {
	
}

void BossAttack4(int t) {
	if (boss->m_shootCD_ <= 0 & ex->m_attack4_radius_ >= -300) {
		double sa = ex->m_attack4_angle_ - V6_PI / 2.0 - V6_PI / 8.0;
		double s = ex->m_slow2_bullet_speed_; // NextRand() * ((ex->m_slow1_bullet_speed_ - ex->m_slow2_bullet_speed_) - 0.5) / V6_RAND_MAX_F + ex->m_slow2_bullet_speed_;
		double sx = s * cos(sa);
		double sy = -s * sin(sa);
		double shx = V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2 + ex->m_attack4_radius_ * cos(ex->m_attack4_angle_);
		double shy = V6_BOSS_SP3ABS_Y + 120 - V6_SMALLBULLET_EDGE_LENGTH / 2 - ex->m_attack4_radius_ * sin(ex->m_attack4_angle_);
		ShootSmall(boss, shx, shy,
			boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

		sa += V6_PI / 4.0;
		sx = s * cos(sa);
		sy = -s * sin(sa);
		ShootSmall(boss, shx, shy,
			boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

		sa += V6_PI;
		sx = s * cos(sa);
		sy = -s * sin(sa);
		shx = V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2 + ex->m_attack4_radius_ * cos(V6_PI + ex->m_attack4_angle_);
		shy = V6_BOSS_SP3ABS_Y + 120 - V6_SMALLBULLET_EDGE_LENGTH / 2 - ex->m_attack4_radius_ * sin(V6_PI + ex->m_attack4_angle_);
		ShootSmall(boss, shx, shy,
			boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

		sa -= V6_PI / 4.0;
		sx = s* cos(sa);
		sy = -s * sin(sa);
		ShootSmall(boss, shx, shy,
			boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

		ex->m_attack4_angle_ += ex->m_attack4_angle_speed_ * (1.0 + (150.0 - ex->m_attack4_radius_) / 150.0) * double(t) / 1000.0;
		boss->m_shootCD_ += ex->m_attack4_interval_ / (1.0 + (150.0 - ex->m_attack4_radius_) / 150.0);
		ex->m_attack4_radius_ -= 50.0 * (1.0 + (150.0 - ex->m_attack4_radius_) / 150.0) * double(t) / 1000.0;
	}
	else if(ex->m_attack4_radius_ < -300) 
		ex->m_attack_process_ = 100;
}

void BossAttack5(int t) {
	BossSelfProtectAttack(t);

	if (ex->m_attack_process_ < V6_BOSS_ATTACK_MODE_COMPLETE) {
		if (ex->m_attack5_stop_time_ > 0) {
			ex->m_attack5_stop_time_ -= t;
		}
		else {
			if (ex->m_attack5_clockwise_) {
				boss->m_shoot_angle_ -= ex->m_attack5_angle_speed_ * double(t) / 1000.0;
				if (boss->m_shoot_angle_ <= V6_PI + 0.2) {
					if (++ex->m_attack5_stop_f_ == 5) {
						ex->m_attack5_stop_time_ = 1000;
						ex->m_attack5_stop_f_ = 0;
						ex->m_attack_process_ += 20;
					}
					ex->m_attack5_clockwise_ = FALSE;
				}
			}
			else {
				boss->m_shoot_angle_ += ex->m_attack5_angle_speed_ * double(t) / 1000.0;
				if (boss->m_shoot_angle_ >= V6_PI * 2.0 - 0.2) {
					if (++ex->m_attack5_stop_f_ == 5) {
						ex->m_attack5_stop_time_ = 1000;
						ex->m_attack5_stop_f_ = 0;
						ex->m_attack_process_ += 20;
					}
					ex->m_attack5_clockwise_ = TRUE;
				}
			}
		}
		ex->m_attack5_rand_t_ -= t;
		if (ex->m_attack5_rand_t_ <= 0) {
			double sa = boss->m_shoot_angle_ + double(NextRand()) / V6_RAND_MAX_F * V6_PI / 6.0 - V6_PI / 12.0;
			double s = NextRand() * (ex->m_mid1_bullet_speed_ - ex->m_slow2_bullet_speed_) / V6_RAND_MAX_F + ex->m_slow2_bullet_speed_;
			double sx = s * cos(sa);
			double sy = -s * sin(sa);
			ex->m_attack5_rand_t_ += ex->m_attack5_rand_frequence_;
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		}
		if (boss->m_shootCD_ <= 0) {
			double sa = boss->m_shoot_angle_ - V6_PI / 12.0;
			double sx = ex->m_fast2_bullet_speed_ * cos(sa);
			double sy = -ex->m_fast2_bullet_speed_ * sin(sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);

			sa += V6_PI / 6.0;
			sx = ex->m_fast2_bullet_speed_ * cos(sa);
			sy = -ex->m_fast2_bullet_speed_ * sin(sa);
			ShootSmall(boss, V6_BOSS_SP3ABS_X - V6_SMALLBULLET_EDGE_LENGTH / 2,
				V6_BOSS_SP3ABS_Y - V6_SMALLBULLET_EDGE_LENGTH / 2,
				boss->m_data_.m_atk_, TRUE, boss->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			boss->m_shootCD_ += ex->m_attack5_interval_;
		}
	}
}

void BossHPRender(int t, LogicSprite * ls)
{
	ls->m_w_ = double(boss->m_HP) / double(boss->m_data_.m_HPmax_) * V6_PROGRESSBAR_WIDTH;
}