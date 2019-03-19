#pragma once

#include "ResourceManager.h"
#include "Game.h"

#define V6_SMALLBULLET_EDGE_LENGTH		20
#define V6_BIGBULLET_EDGE_LENGTH		80

typedef struct _LogicSprite LogicSprite;
typedef struct _Tank Tank;

typedef enum _BULLETSTYLE {
	SMALL_BULLET, BIG_BULLET
} BULLETSTYLE;

typedef struct _Bullet{
	void(*Update)(int t, struct _Bullet* b);

	BULLETSTYLE m_bulletStyle_;
	// 真正实体。
	void* m_me_;
	// 父类指针。
	LogicSprite* m_super_;
	// 发射者。
	Tank* m_sender_;

	// 子弹的攻击力。
	int m_atk_;

	BOOLean m_ignore_wall_;

	double m_speedX_, m_speedY_;

	// 超出这个范围时将无效。
	double m_validXMin_, m_validYMin_, m_validXMax_, m_validYMax_;

	BOOLean m_valid_ = TRUE;
} Bullet;

LogicSprite* CreateSmallBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);
LogicSprite* CreateBigBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT);

inline void ShootSmall(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT){
	LogicSprite* ls = CreateSmallBullet(sender, x, y, atk, ignore_wall, speedx, speedy, update, xmin, ymin, xmax, ymax);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyBulletList_, ls->m_me_);
}
inline void ShootBig(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin = 0, double ymin = 0, double xmax = V6_GAMEFIELD_WIDTH, double ymax = V6_GAMEFIELD_HEIGHT) {
	LogicSprite* ls = CreateBigBullet(sender, x, y, atk, ignore_wall, speedx, speedy, update, xmin, ymin, xmax, ymax);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyBulletList_, ls->m_me_);
}

void BulletNormalUpdate(int t, Bullet* b);