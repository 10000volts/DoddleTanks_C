#pragma once

#include "Item.h"
#include "Tank.h"

typedef struct _LogicStep LogicStep;
typedef struct _Container Container;

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void LoadGameStepResource();

void InitializeGame();
// ���ļ�������Ϸ��
void LoadGameFromFile(const char file[]);

// ��Ϸ��ѭ����
extern LogicStep* g_stepGameUpdate_;

void GameOver(BOOLean success = FALSE);

void GenerateItem(int x, int y, ITEMSTYLE is);

// �����Ƿ���䵽�������ϰ��ϡ�(��ζ���ƶ�ʧ��)
void AlignToBarriers(Tank* t, double xnew, double ynew);

// �ӳ�t���������������Ϊ�Ų���Ч���Ա���
void Delay(int t, void(*act)());
// �ӳ�t���������������Ϊ�Ų���Ч���Ա���
void DelayWithTwoArgs(int t, void(*act)(int, int), int exd1, int exd2);

extern GAME_DIFFICULTY g_gameDifficulty_;
// ��Ϸ�Ƿ���ɡ�
extern BOOLean m_success_;
// ��Ϸ�Ƿ�ʧ�ܡ�
extern BOOLean m_failed_;

extern TankData m_tankData_[V6_TANKSTYLE_COUNT];

// ��ǰ�غ��Ƿ�Ϊ����غϣ����ǣ����Զ����ɵз�̹���Ҳ����ж�Ĭ�ϵĻغϽ�����
extern BOOLean m_special_wave_;
// ��ǰ����غ��Ƿ������
extern BOOLean m_special_wave_end_;
// ��ǰ������
extern int m_wave_;
// ʱ��ʣ�ࡣ
extern int m_time_;
extern int m_score_;
// �Ѽ������������Ŀ��
extern int m_activeRespawn_;

// �Ƿ����ڲ��Żƽ�24K�Ųʶ������ڼ��������ѿ�̹�˺��ӵ���Ϊ�Ա����޷�������
extern BOOLean m_playingAnimation_;

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
// �������б�
extern Container* m_triggerList_;