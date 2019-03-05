#pragma once

#include "LogicControl.h"

typedef enum _GAME_DIFFICULTY {
	GDFT_EASY, GDFT_VEASY, GDFT_EEASY
} GAME_DIFFICULTY;

void InitializeGame();
// ���ļ�������Ϸ��
void LoadGameFromFile(LPCSTR file);

extern GAME_DIFFICULTY g_gameDifficulty_;
// ��ǰ������
extern int m_wave_;