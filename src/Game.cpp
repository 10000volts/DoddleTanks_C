#include "Game.h"

static BOOLean m_valid_;

void InitializeGame()
{
	if (!m_valid_) {

	}
}

void LoadGameFromFile(LPCSTR file)
{
	m_valid_ = TRUE;
}

GAME_DIFFICULTY g_gameDifficulty_;
int m_wave_;