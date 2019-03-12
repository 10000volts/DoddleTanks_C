#pragma once

#include "LogicControl.h"
#include "Item.h"

#define V6_GAMEFIELD_LEFT		0.0
#define V6_GAMEFIELD_TOP		0.0
#define V6_GAMEFIELD_WIDTH		723.0
#define V6_GAMEFIELD_HEIGHT		768.0
// �з�̹�˵����ɼ����
#define V6_GAME_GENERATE_CD		1000

typedef struct _Tank Tank;

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void LoadGameStepResource();

void InitializeGame();
// ���ļ�������Ϸ��
void LoadGameFromFile(LPCSTR file);

// ��Ϸ��ѭ����
extern LogicStep* g_stepGameUpdate_;

void GameOver();

void GenerateItem(int x, int y, ITEMSTYLE is);

// �����Ƿ���䵽�������ϰ��ϡ�(��ζ���ƶ�ʧ��)
void AlignToBarriers(Tank* t, double xnew, double ynew);

extern GAME_DIFFICULTY g_gameDifficulty_;
// ��ǰ������
extern int m_wave_;
// ʱ��ʣ�ࡣ
extern int m_time_;
extern int m_score_;
// �Ѽ������������Ŀ��
extern int m_activeRespawn_;

extern Tank* m_playerTank_;
extern Tank* m_stronghold_;

extern Container* m_playerTankList_;
extern Container* m_playerBulletList_;
extern Container* m_enemyTankList_;
extern Container* m_enemyBulletList_;
extern Container* m_neutralTankList_;
// ��ҿ�ʰ��ĵ����б�
extern Container* m_playerItemList_;
// �з�̹���������б�
extern Container* m_respawn_;