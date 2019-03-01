#include "Scene.h"
#include "LogicControl.h"

void LoadMainScene();
void LoadOptionScene();
void LoadRankingScene();
void LoadGameScene();

// 清楚当前场景上的所有内容并重新加载场景。
void LoadScene(enum Scene sc) {
	ClearButtons();
	UnloadLogicEngine();
	switch (sc)
	{
	case SCENE_MAIN:
		LoadMainScene();
		break;
	case SCENE_OPTION:
		LoadOptionScene();
		break;
	case SCENE_RANKING:
		LoadRankingScene();
		break;
	case SCENE_GAME:
		LoadGameScene();
		break;
	default:
		break;
	}
}

void LoadMainScene()
{
	AddLogicStep(m_stepCheckFocus_);


}

void LoadOptionScene()
{

}

void LoadRankingScene()
{

}

void LoadGameScene()
{

}
