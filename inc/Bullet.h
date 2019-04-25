#pragma once

#include "GameBase.h"
typedef struct _LogicSprite LogicSprite;

typedef enum _BULLETSTYLE {
	SMALL_BULLET, BIG_BULLET
} BULLETSTYLE;

typedef struct _Bullet{
	void(*Update)(int t, struct _Bullet* b);
	void(*Dead)(struct _Bullet* b);

	BULLETSTYLE m_bulletStyle_;
	// 真正实体。
	void* m_me_;
	// 父类指针。
	LogicSprite* m_super_;

	// 子弹的攻击力。
	int m_atk_;
	double m_damageRatioToBuilding_;

	BOOLean m_ignore_wall_;

	double m_speedX_, m_speedY_;

	// 超出这个范围时将无效。
	double m_validXMin_, m_validYMin_, m_validXMax_, m_validYMax_;

	BOOLean m_valid_ = TRUE;
} Bullet;

LogicSprite* CreateSmallBullet(int x, int y, int atk, 
	BOOLean ignore_wall, double damageRatioToBuilding, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

LogicSprite* CreateBigBullet(int x, int y, int atk,
	BOOLean ignore_wall, double damageRatioToBuilding, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

LogicSprite* CreateBullet(BULLETSTYLE bs, int x, int y, int atk,
						  BOOLean ignore_wall, double damageRatioToBuilding, double speedx, double speedy, void(*update)(int t, Bullet* b),
						  double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

void ShootSmall(int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

void ShootBig(int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

inline void Invalid(Bullet* b) {
	b->m_valid_ = FALSE;
	if (b->Dead != NULL) b->Dead(b);
}

void BulletNormalUpdate(int t, Bullet* b);