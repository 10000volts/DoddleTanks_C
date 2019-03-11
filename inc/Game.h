#pragma once

#include "LogicControl.h"
#include "Item.h"

#define V6_GAMEFIELD_LEFT		0.0
#define V6_GAMEFIELD_TOP		0.0
#define V6_GAMEFIELD_WIDTH		723.0
#define V6_GAMEFIELD_HEIGHT		768.0
// 敌方坦克的生成间隔。
#define V6_GAME_GENERATE_CD		1000

typedef struct _Tank Tank;

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void LoadGameStepResource();

void InitializeGame();
// 从文件加载游戏。
void LoadGameFromFile(LPCSTR file);

// 游戏主循环。
extern LogicStep* g_stepGameUpdate_;

void GameOver();

void GenerateItem(int x, int y, ITEMSTYLE is);

// 返回是否对其到了其他障碍上。(意味着移动失败)
void AlignToBarriers(Tank* t, double xnew, double ynew);

extern GAME_DIFFICULTY g_gameDifficulty_;
// 当前波数。
extern int m_wave_;
// 时间剩余。
extern int m_time_;
extern int m_score_;
// 已激活的重生点数目。
extern int m_activeRespawn_;

extern Tank* m_playerTank_;
extern Tank* m_stronghold_;

extern Container* m_playerTankList_;
extern Container* m_playerBulletList_;
extern Container* m_enemyTankList_;
extern Container* m_enemyBulletList_;
extern Container* m_neutralTankList_;
// 玩家可拾起的道具列表。
extern Container* m_playerItemList_;
// 敌方坦克重生点列表。
extern Container* m_respawn_;