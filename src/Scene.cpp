#include "Scene.h"
#include "LogicControl.h"
#include "ResourceManager.h"

void LoadMainScene();
void LoadHelpScene();
void LoadRankingScene();
void LoadGameScene(); 
void ButtonClickLoadMain(Button* tis);
void ButtonClickExit(Button* tis);
void ButtonClickLoadGame(Button* tis);
void ButtonClickLoadHelp(Button* tis);
void ButtonClickLoadRankingList(Button* tis);

// 清除当前场景上的所有内容并重新加载场景。
void LoadScene(enum Scene sc) {
	ClearButtons();
	UnloadLogicEngine();
	switch (sc)
	{
	case SCENE_MAIN:
		LoadMainScene();
		break;
	case SCENE_HELP:
		LoadHelpScene();
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
	LogicSprite * ls;
	Button * b;

	AddLogicStep(m_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, &g_img_mainTitle);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 140, 400, 105, 50, &g_img_start);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadGame, ButtonFocusDefault, ButtonLeaveDefault, &g_img_start, NULL, &g_img_startFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 100, 480, 225, 50, &g_img_rankingList);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadRankingList, ButtonFocusDefault, ButtonLeaveDefault, &g_img_rankingList, NULL, &g_img_rankingListFocus);
	AddButton(b);	
	
	ls = CreateLogicSprite(NULL, NULL, 140, 560, 225, 50, &g_img_help);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadHelp, ButtonFocusDefault, ButtonLeaveDefault, &g_img_help, NULL, &g_img_helpFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 640, 105, 50, &g_img_exit);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickExit, ButtonFocusDefault, ButtonLeaveDefault, &g_img_exit, NULL, &g_img_exitFocus);
	AddButton(b);
}

void LoadHelpScene()
{
	LogicSprite * ls;
	Button * b;

	AddLogicStep(m_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, &g_img_helpDetail);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 400, 600, 105, 50, &g_img_back);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

void LoadRankingScene()
{

}

void LoadGameScene()
{

}

void ButtonClickExit(Button* tis) {
	exit(0);
}

void ButtonClickLoadMain(Button* tis) {
	LoadScene(SCENE_MAIN);
}

void ButtonClickLoadHelp(Button* tis) {
	LoadScene(SCENE_HELP);
}

void ButtonClickLoadGame(Button* tis) {
	LoadScene(SCENE_GAME);
}

void ButtonClickLoadRankingList(Button* tis) {
	LoadScene(SCENE_RANKING);
}