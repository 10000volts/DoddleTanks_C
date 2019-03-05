#include "Game.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "RandomEngine.h"

static BOOLean m_valid_;
void InitGameView();

void InitializeGame()
{
	if (!m_valid_) {

	}

	InitGameView();
}
static void InitGameView() {
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_console);
	AddLogicSprite(ls);
}

void LoadGameFromFile(LPCSTR file)
{
	m_valid_ = TRUE;
}

GAME_DIFFICULTY g_gameDifficulty_;
int m_wave_;