#pragma once

#include "ResourceManager.h"

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
	// ����ʵ�塣
	void* m_me_;
	// ����ָ�롣
	LogicSprite* m_super_;
	// �����ߡ�
	Tank* m_sender_;

	// �ӵ��Ĺ�������
	int m_atk_;

	BOOLean m_ignore_wall_;

	double m_speedX_, m_speedY_;

	BOOLean m_valid_ = TRUE;
} Bullet;

LogicSprite* CreateSmallBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b));
LogicSprite* CreateBigBullet(Tank* sender, int x, int y, int atk, BOOLean ignore_wall, double speedx, double speedy, void(*update)(int t, Bullet* b));

void BulletNormalUpdate(int t, Bullet* b);