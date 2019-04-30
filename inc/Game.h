#pragma once

#include "Item.h"
#include "Tank.h"

typedef struct _LogicStep LogicStep;
typedef struct _Container Container;

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void LoadGameStepResource();

void InitializeGame(BOOLean loadFromLife);

// 游戏主循环。
extern LogicStep* g_stepGameUpdate_;

void GameOver(BOOLean success = FALSE);

void GenerateItem(int x, int y, ITEMSTYLE is);

// 返回是否对齐到了其他障碍上。(意味着移动失败)
void AlignToBarriers(Tank* t, double xnew, double ynew);

// 延迟t毫秒后发生。不会因为炫彩特效而自卑。
void Delay(int t, void(*act)());
// 延迟t毫秒后发生。不会因为炫彩特效而自卑。
void DelayWithTwoArgs(int t, void(*act)(int, int), int exd1, int exd2);

extern GAME_DIFFICULTY g_gameDifficulty_;
// 游戏是否完成。
extern BOOLean m_success_;
// 游戏是否失败。
extern BOOLean m_failed_;

extern TankData m_tankData_[V6_TANKSTYLE_COUNT];

// 当前回合是否为特殊回合，若是，则不自动生成敌方坦克且不会判定默认的回合结束，存档也无法生效。
extern BOOLean m_special_wave_;
// 当前特殊回合是否结束。
extern BOOLean m_special_wave_end_;
// 当前波数。
extern int m_wave_;
// 时间剩余。
extern int m_time_;
extern int m_score_;
// 已激活的重生点数目。
extern int m_activeRespawn_;

// 是否正在播放黄金24K炫彩动画。期间其他的难看坦克和子弹因为自卑而无法动弹。
extern BOOLean m_playingAnimation_;

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
// 触发器列表。
extern Container* m_triggerList_;