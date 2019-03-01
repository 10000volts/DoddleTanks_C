#include "Scene.h"
#include "LogicControl.h"

void LoadMainScene();
void LoadOptionScene();
void LoadRankingScene();
void LoadGameScene();

// �����ǰ�����ϵ��������ݲ����¼��س�����
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
