#pragma once

#include "Game.h"
#include "Bullet.h"
#include "DataModels.h"

#define V6_TANK_HALF_EDGE_LENGTH		40

typedef struct _Tank {
	// 决策。
	void (*Decide)(struct _Tank* tis);
	// 根据决策行动。
	void (*Act)(int t, struct _Tank* tis);
	// 死亡的回调函数。
	void (*Dead)(struct _Tank* tis);

	TANKSTYLE m_tankStyle_;
	// 真正实体的额外信息。
	void* m_extra_;
	// 父类指针。
	LogicSprite* m_super_;

	TankData m_data_;

	int m_HP;
	int m_shootCD_;

	// 子弹碰撞判定的左侧坐标。(原始值)
	double m_srcBulletCollideL, m_srcBulletCollideT, m_srcBulletCollideW, m_srcBulletCollideH;
	// 墙体碰撞判定的左侧坐标。(原始值)
	double m_srcCollideL, m_srcCollideT, m_srcCollideW, m_srcCollideH;
	// 子弹碰撞判定的左侧坐标。
	double m_bulletCollideL, m_bulletCollideT, m_bulletCollideW, m_bulletCollideH;
	// 墙体碰撞判定的左侧坐标。
	double m_collideL, m_collideT, m_collideW, m_collideH;
	// 判定会随之改变。除非m_noOrientation_被同时设置为FALSE。
	double m_orientation_;
	// 是否使用角度而非朝向。设置为TRUE时改变朝向不会改变判定。
	BOOLean m_noOrientation_;

	// 是否决策向右(并无合法检测)。
	BOOLean m_move_right_, m_move_up_, m_move_left_, m_move_down_;
	// 是否决策射击。
	BOOLean m_shoot_;
	// 决策的射击角度。
	double m_shoot_angle_;

	BOOLean m_valid_ = TRUE;
} Tank;

Tank* CreateTank(TANKSTYLE tankstyle, void* ex, LogicSprite* super, TankData td, 
	int bcl, int bct, int bcw, int bch,
	int cl, int ct, int cw, int ch,
	double ori, void(*decide)(Tank* tis) = NULL, void(*act)(int t, Tank* tis) = NULL, void(*dead)(Tank* tis) = NULL);

Tank* CreateStronghold(LogicSprite* ls);

Tank* CreatePlayerTank(LogicSprite* ls);
typedef struct _PlayerTank {
	// 是否减速。
	BOOLean m_slow_;
	// 决策是否放置墙。
	BOOLean m_setWall_;

	// 当前朝向角。(装饰用)
	double m_orientation_angle_;
	// 转向角速度。(装饰用)
	double m_palstance_;

	// 剩余可放置的墙数目。
	int m_wall_count_;
} PlayerTank;

Tank* CreateJunkTank(LogicSprite* ls);
Tank* CreateBigTank(LogicSprite* ls);
Tank* CreatePrismTank(LogicSprite* ls);
Tank* CreateFiveTank(LogicSprite* ls);
Tank* CreateQuickTank(LogicSprite* ls);
Tank* CreateSunTank(LogicSprite* ls);
Tank* CreateLanlingkingTank(LogicSprite* ls);
Tank* CreateAttackTank(LogicSprite* ls);

typedef struct _LanlingkingTank {
	// 连续射击剩余次数。
	int m_contiousShootCountLeft_;
} LanlingkingTank;

typedef struct _AttackTank {
	// 追踪的目标。
	Tank* m_aim_;

	// 是否正在追踪目标。
	BOOLean m_tracking_;
} AttackTank;


// 更改坦克的朝向并根据noOrientation属性判断是否据此修改碰撞判定。
void ChangeTankOrientation(Tank* t, double ori);