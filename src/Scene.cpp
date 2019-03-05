#include "Scene.h"
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Game.h"

void LoadMainScene();
void LoadHelpScene();
void LoadRankingScene();
void LoadGameScene();
void LoadDifficultyChooseScene();
void LoadWaveChooseScene();
void ButtonClickLoadMain(Button* tis);
void ButtonClickExit(Button* tis);
void ButtonClickLoadGame(Button* tis);
void ButtonClickLoadHelp(Button* tis);
void ButtonClickLoadRankingList(Button* tis);
void ButtonClickLoadDifficultyChoose(Button* tis);
void ButtonClickLoadFile(Button* tis);
void ButtonClickEasy(Button* tis);
void ButtonClickVeryEasy(Button* tis);
void ButtonClickExtremelyEasy(Button* tis); 
void StepWaveChange(LogicStep* tis);

LogicStep* g_stepWaveChange_;

// 清除当前场景上的所有内容并重新加载场景。
void LoadScene(Scene sc) {
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
	case SCENE_DIFFICULTY_CHOOSE:
		LoadDifficultyChooseScene();
		break;
	case SCENE_WAVE_CHOOSE:
		LoadWaveChooseScene();
		break;
	default:
		break;
	}
}

void LoadSceneStepResource()
{
	g_stepWaveChange_ = (LogicStep*)malloc(sizeof(LogicStep));
	memset(g_stepWaveChange_, 0, sizeof(LogicStep));
	strcpy(g_stepWaveChange_->m_stepName_, "WaveChange");
	g_stepWaveChange_->Update = StepWaveChange;
}

void LoadMainScene()
{
	LogicSprite * ls;
	Button * b;

	AddLogicStep(g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 140, 400, 105, 50, RenderSimple, &g_img_start);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadDifficultyChoose, ButtonFocusDefault, ButtonLeaveDefault, &g_img_start, NULL, &g_img_startFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 100, 480, 225, 50, RenderSimple, &g_img_rankingList);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadRankingList, ButtonFocusDefault, ButtonLeaveDefault, &g_img_rankingList, NULL, &g_img_rankingListFocus);
	AddButton(b);	
	
	ls = CreateLogicSprite(NULL, NULL, 140, 560, 225, 50, RenderSimple, &g_img_help);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadHelp, ButtonFocusDefault, ButtonLeaveDefault, &g_img_help, NULL, &g_img_helpFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 640, 105, 50, RenderSimple, &g_img_exit);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickExit, ButtonFocusDefault, ButtonLeaveDefault, &g_img_exit, NULL, &g_img_exitFocus);
	AddButton(b);
}

// LogicSpriteUpdate
void LSUDecorate(LogicSprite* tis) {
	tis->m_angle_ += 0.1;
	if (tis->m_angle_ >= 2 * V6_PI) tis->m_angle_ -= 2 * V6_PI;
}
void LoadHelpScene()
{
	LogicSprite * ls;
	Button * b;

	AddLogicStep(g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_helpDetail);
	AddLogicSprite(ls);	

	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 100, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddLogicSprite(ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 110, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddLogicSprite(ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 120, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddLogicSprite(ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 130, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddLogicSprite(ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 140, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 400, 640, 105, 50, RenderSimple, &g_img_back);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

void LoadRankingScene()
{
	
}

void LoadGameScene()
{
	InitializeGame();
}

void LoadDifficultyChooseScene() {
	LogicSprite * ls;
	Button * b;

	AddLogicStep(g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 100, 320, 200, 50, RenderSimple, &g_img_loadGame);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadFile, ButtonFocusDefault, ButtonLeaveDefault, &g_img_loadGame, NULL, &g_img_loadGameFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 400, 105, 50, RenderSimple, &g_img_easy);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_easy, NULL, &g_img_easyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 100, 480, 225, 50, RenderSimple, &g_img_veasy);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickVeryEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_veasy, NULL, &g_img_veasyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 560, 225, 50, RenderSimple, &g_img_eeasy);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickExtremelyEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_eeasy, NULL, &g_img_eeasyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 640, 105, 50, RenderSimple, &g_img_back);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

static int twave; 
static TCHAR str_twave[4]; 
static RECT tr;
static TCHAR tstr[128];
void WaveRender(LogicSprite* ls) {
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	wcscpy(tstr, _T("向右或向左移动以调整起始回合："));
	_stprintf(str_twave, _T("%d"), twave);
	wcscat(tstr, str_twave);
	drawtext(tstr, &tr, DT_SINGLELINE);
}
void LoadWaveChooseScene() {
	LogicSprite * ls;
	Button * b;

	AddLogicStep(g_stepCheckFocus_);
	AddLogicStep(g_stepWaveChange_);

	twave = 1;

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddLogicSprite(ls);

	ls = CreateLogicSprite(NULL, NULL, 440, 400, 225, 50, RenderSimple, &g_img_start);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadGame, ButtonFocusDefault, ButtonLeaveDefault, &g_img_start, NULL, &g_img_startFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 440, 480, 105, 50, RenderSimple, &g_img_back);
	AddLogicSprite(ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 310, 300, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, WaveRender, &g_img_mainTitle2);
	AddLogicSprite(ls);
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
	m_wave_ = twave;
	LoadScene(SCENE_GAME);
}

void ButtonClickLoadRankingList(Button* tis) {
	LoadScene(SCENE_RANKING);
}

void ButtonClickLoadDifficultyChoose(Button* tis) {
	LoadScene(SCENE_DIFFICULTY_CHOOSE);
}

void ButtonClickLoadFile(Button * tis)
{
	LoadGameFromFile("save.dat");
	LoadScene(SCENE_GAME);
}

void ButtonClickEasy(Button * tis)
{
	g_gameDifficulty_ = GDFT_EASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

void ButtonClickVeryEasy(Button * tis)
{
	g_gameDifficulty_ = GDFT_VEASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

void ButtonClickExtremelyEasy(Button * tis)
{
	g_gameDifficulty_ = GDFT_EEASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

static void StepWaveChange(LogicStep* tis) {
	if (g_keyboardState_.left_up) {
		if(twave > 1) --twave;
	}
	if (g_keyboardState_.right_up) {
		if (twave < V6_GAME_MAX_WAVE) ++twave;
	}
}