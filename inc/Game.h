#pragma once

#include "LogicControl.h"

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void InitializeGame();
// 从文件加载游戏。
void LoadGameFromFile(LPCSTR file);

extern GAME_DIFFICULTY g_gameDifficulty_;
// 当前波数。
extern int m_wave_;