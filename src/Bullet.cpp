#include "Bullet.h"
#include "Game.h"
#include "LogicEngine.h"
#include "RenderEngine.h"
#include "ResourceManager.h"

LogicSprite * CreateSmallBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet *b),
	double xmin, double ymin, double xmax, double ymax)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, 20, 20, RenderWithMask, &g_img_smallBullet, &g_img_smallBulletMsk);
	Bullet* b = (Bullet*)malloc(sizeof(Bullet));
	memset(b, 0, sizeof(Bullet));

	b->m_sender_ = sender;
	b->m_super_ = ls;
	b->m_bulletStyle_ = SMALL_BULLET;
	b->m_atk_ = atk;
	b->m_ignore_wall_ = ignore_wall;
	b->m_damageRatioToBuilding_ = damageRatioToBuilding;
	b->m_speedX_ = speedx;
	b->m_speedY_ = speedy;
	b->Update = update;
	b->m_validXMin_ = xmin;
	b->m_validYMin_ = ymin;
	b->m_validXMax_ = xmax;
	b->m_validYMax_ = ymax;
	b->m_valid_ = TRUE;
	ls->m_me_ = b;
	return ls;
}

LogicSprite * CreateBigBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet *b),
	double xmin, double ymin, double xmax, double ymax)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, 80, 80, RenderWithMask, &g_img_bigBullet, &g_img_bigBulletMsk);
	Bullet* b = (Bullet*)malloc(sizeof(Bullet));
	memset(b, 0, sizeof(Bullet));

	b->m_sender_ = sender;
	b->m_super_ = ls;
	b->m_bulletStyle_ = BIG_BULLET;
	b->m_atk_ = atk;
	b->m_ignore_wall_ = ignore_wall;
	b->m_damageRatioToBuilding_ = damageRatioToBuilding;
	b->m_speedX_ = speedx;
	b->m_speedY_ = speedy;
	b->Update = update;
	b->m_validXMin_ = xmin;
	b->m_validYMin_ = ymin;
	b->m_validXMax_ = xmax;
	b->m_validYMax_ = ymax;
	b->m_valid_ = TRUE;
	ls->m_me_ = b;
	return ls;
}

void ShootSmall(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin, double ymin, double xmax, double ymax) {
	LogicSprite* ls = CreateSmallBullet(sender, x, y, atk, ignore_wall, damageRatioToBuilding,
		speedx, speedy, update, xmin, ymin, xmax, ymax);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyBulletList_, ls->m_me_);
}

void ShootBig(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double damageRatioToBuilding,
	double speedx, double speedy, void(*update)(int t, Bullet* b),
	double xmin, double ymin, double xmax, double ymax) {
	LogicSprite* ls = CreateBigBullet(sender, x, y, atk, ignore_wall, damageRatioToBuilding,
		speedx, speedy, update, xmin, ymin, xmax, ymax);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyBulletList_, ls->m_me_);
}

void BulletNormalUpdate(int t, Bullet * b)
{
	b->m_super_->m_x_ += b->m_speedX_ * double(t) / 1000.0;
	b->m_super_->m_y_ += b->m_speedY_ * double(t) / 1000.0;
}
