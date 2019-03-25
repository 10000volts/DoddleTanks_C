#include "Scene.h"
#include "InputEngine.h"
#include "LogicControl.h"
#include "RenderEngine.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameConsole.h"
#include "RankingList.h"
#include <string.h>

static void LoadMainScene();
static void LoadHelpScene();
static void LoadRankingScene();
static void LoadGameScene();
static void LoadDifficultyChooseScene();
static void LoadWaveChooseScene();
static void LoadGameOverScene();
static void LoadStageClearScene();
static void LoadSaveScoreScene();
static void ButtonClickLoadMain(Button* tis);
static void ButtonClickExit(Button* tis);
static void ButtonClickLoadGame(Button* tis);
static void ButtonClickLoadHelp(Button* tis);
static void ButtonClickLoadRankingList(Button* tis);
static void ButtonClickLoadDifficultyChoose(Button* tis);
static void ButtonClickLoadFile(Button* tis);
static void ButtonClickEasy(Button* tis);
static void ButtonClickVeryEasy(Button* tis);
static void ButtonClickExtremelyEasy(Button* tis);
static void ButtonClickSaveScore(Button* tis);
static void StepWaveChange(int t, LogicStep* tis);
static void StepNameInput(int t, LogicStep* tis);
static void StepChangePage(int t, LogicStep* tis);

static LogicStep* l_stepWaveChange_;
static LogicStep* l_stepNameInput_;
static LogicStep* l_stepChangePage_;

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
	case SCENE_GAMEOVER:
		LoadGameOverScene();
		break;
	case SCENE_STAGECLEAR:
		LoadStageClearScene();
		break;
	case SCENE_SAVESCORE:
		LoadSaveScoreScene();
		break;
	default:
		break;
	}
}

void LoadSceneStepResource(){
	l_stepWaveChange_ = CreateLogicStep((char*)"WaveChange", StepWaveChange);
	l_stepNameInput_ = CreateLogicStep((char*)"NameInput", StepNameInput);
	l_stepChangePage_ = CreateLogicStep((char*)"ChangePage", StepChangePage);
}

void LoadMainScene(){
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);
	AddElement(g_logicStepManager_, g_stepCheckActiveEasyMode_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 140, 400, 105, 50, RenderSimple, &g_img_start);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadDifficultyChoose, ButtonFocusDefault, ButtonLeaveDefault, &g_img_start, NULL, &g_img_startFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 100, 480, 225, 50, RenderSimple, &g_img_rankingList);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadRankingList, ButtonFocusDefault, ButtonLeaveDefault, &g_img_rankingList, NULL, &g_img_rankingListFocus);
	AddButton(b);	
	
	ls = CreateLogicSprite(NULL, NULL, 140, 560, 105, 50, RenderSimple, &g_img_help);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadHelp, ButtonFocusDefault, ButtonLeaveDefault, &g_img_help, NULL, &g_img_helpFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 640, 105, 50, RenderSimple, &g_img_exit);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickExit, ButtonFocusDefault, ButtonLeaveDefault, &g_img_exit, NULL, &g_img_exitFocus);
	AddButton(b);

	ReadHighScore();
	if (g_top_scores_[0].score >= 250000) {
		ls = CreateLogicSprite(NULL, NULL, 350, 70, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderWithMask, &g_img_bossTank, &g_img_bossTankMsk);
		AddElement(g_logicSpriteManager_, ls);
	}
}

// LogicSpriteUpdate
void LSUDecorate(int t, LogicSprite* tis) {
	tis->m_angle_ += 5.0 * double(t) / 1000.0;
	if (tis->m_angle_ >= 2 * V6_PI) tis->m_angle_ -= 2 * V6_PI;
}
void LoadHelpScene(){
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_helpDetail);
	AddElement(g_logicSpriteManager_, ls);	

	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 100, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 110, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 120, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 130, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, LSUDecorate, 750, 140, 100, 100, RenderWithRotation, g_img_prismTank, g_img_prismTankMsk);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 400, 640, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

static LogicSprite* l_ranking_list_;
static void RenderRankingPage2(LogicSprite* ls) {
	int i;
	RECT tr, tr2;
	TCHAR str[V6_NAME_MAX_LENGTH];

	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;
	tr2 = tr;
	tr2.left += 300;

	for (i = V6_MAX_TOP_SCORES / 2; i < V6_MAX_TOP_SCORES; ++i) {
		wcscpy(str, g_top_scores_[i].name);
		if (wcscmp(str, _T("")) != 0) {
			TCHAR tstr[128];
			memset(tstr, 0, sizeof(tstr));
			settextcolor(BLUE);
			_stprintf(tstr, _T("%s"), str);
			drawtext(tstr, &tr, DT_SINGLELINE);

			memset(tstr, 0, sizeof(tstr));
			settextcolor(BLACK);
			_stprintf(tstr, _T("%d"), g_top_scores_[i].score);
			drawtext(tstr, &tr2, DT_SINGLELINE);

			tr.top += 50;
			tr2.top += 50;
		}
		else {
			break;
		}
	}
}
static void RenderRankingPage1(LogicSprite* ls) {
	int i;
	RECT tr, tr2;
	TCHAR str[V6_NAME_MAX_LENGTH];

	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;
	tr2 = tr;
	tr2.left += 300;

	for (i = 0; i < V6_MAX_TOP_SCORES / 2; ++i) {
		wcscpy(str, g_top_scores_[i].name);
		if (wcscmp(str, _T("")) != 0) {
			TCHAR tstr[128];
			memset(tstr, 0, sizeof(tstr));
			_stprintf(tstr, _T("%s"), str);
			settextcolor(BLUE);
			drawtext(tstr, &tr, DT_SINGLELINE);

			memset(tstr, 0, sizeof(tstr));
			settextcolor(BLACK);
			_stprintf(tstr, _T("%d"), g_top_scores_[i].score);
			drawtext(tstr, &tr2, DT_SINGLELINE);

			tr.top += 50;
			tr2.top += 50;
		}
		else {
			break;
		}
	}
}
void StepChangePage(int t, LogicStep* tis) {
	if (g_keyboardState_.left_up) {
		l_ranking_list_->m_body_->Render = RenderRankingPage1;
	}
	else if (g_keyboardState_.right_up) {
		l_ranking_list_->m_body_->Render = RenderRankingPage2;
	}
}
void LoadRankingScene(){
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);
	AddElement(g_logicStepManager_, l_stepChangePage_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 400, 640, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);

	ReadHighScore();

	l_ranking_list_ = CreateLogicSprite(NULL, NULL, 300, 50, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderRankingPage1, NULL);
	AddElement(g_logicSpriteManager_, l_ranking_list_);
}

void LoadGameScene(){
	InitializeGame();
}

void LoadDifficultyChooseScene() {
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 100, 320, 200, 50, RenderSimple, &g_img_loadGame);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadFile, ButtonFocusDefault, ButtonLeaveDefault, &g_img_loadGame, NULL, &g_img_loadGameFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 400, 105, 50, RenderSimple, &g_img_easy);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_easy, NULL, &g_img_easyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 100, 480, 225, 50, RenderSimple, &g_img_veasy);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickVeryEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_veasy, NULL, &g_img_veasyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 560, 300, 50, RenderSimple, &g_img_eeasy);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickExtremelyEasy, ButtonFocusDefault, ButtonLeaveDefault, &g_img_eeasy, NULL, &g_img_eeasyFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 140, 640, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
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

	AddElement(g_logicStepManager_, g_stepCheckFocus_);
	AddElement(g_logicStepManager_, l_stepWaveChange_);

	twave = 1;

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 440, 400, 225, 50, RenderSimple, &g_img_start);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadGame, ButtonFocusDefault, ButtonLeaveDefault, &g_img_start, NULL, &g_img_startFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 440, 480, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 310, 300, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, WaveRender, NULL);
	AddElement(g_logicSpriteManager_, ls);
}

void ButtonClickSaveScore(Button * tis){
	LoadScene(SCENE_SAVESCORE);
}
void RenderScore(LogicSprite * ls){
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), m_score_);
	drawtext(tstr, &tr, DT_SINGLELINE);
}
void LoadGameOverScene(){
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_gameOver);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 180, 440, 100, 30, RenderScore, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, NULL, 140, 520, 105, 50, RenderSimple, &g_img_save);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickSaveScore, ButtonFocusDefault, ButtonLeaveDefault, &g_img_save, NULL, &g_img_saveFocus);
	AddButton(b);
	ls = CreateLogicSprite(NULL, NULL, 820, 680, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

void LoadStageClearScene() {
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_gameOver);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 180, 440, 100, 30, RenderScore, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(NULL, NULL, 140, 520, 105, 50, RenderSimple, &g_img_save);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickSaveScore, ButtonFocusDefault, ButtonLeaveDefault, &g_img_save, NULL, &g_img_saveFocus);
	AddButton(b);
	ls = CreateLogicSprite(NULL, NULL, 820, 680, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);
}

static TCHAR l_name[V6_NAME_MAX_LENGTH];
static int l_name_edil_pos;
static BOOLean l_keyDown[128];
static void ButtonClickSave(Button* tis) {
	SaveScore(l_name, m_score_);
	LoadScene(SCENE_MAIN);
}
void RenderName(LogicSprite * ls) {
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("键入您的姓名：%s"), l_name);
	drawtext(tstr, &tr, DT_SINGLELINE);
}
static void StepNameInputUpdateKey(int key) {
	if (GetAsyncKeyState(key)) {
		l_keyDown[key] = TRUE;
	}
	else {
		if (l_keyDown[key]) {
			if (key == VK_BACK) {
				if (l_name_edil_pos) {
					l_name[--l_name_edil_pos] = 0;
				}
			}
			else if (l_name_edil_pos < V6_NAME_MAX_LENGTH) {
				l_name[l_name_edil_pos++] = key;
			}
		}
		l_keyDown[key] = FALSE;
	}
}
void StepNameInput(int t, LogicStep* tis) {
	int key;
	for (key = '0'; key <= '9'; ++key) {
		StepNameInputUpdateKey(key);
	}
	for (key = 'A'; key <= 'Z'; ++key) {
		StepNameInputUpdateKey(key);
	}
	StepNameInputUpdateKey(VK_BACK);
}
void LoadSaveScoreScene(){
	LogicSprite * ls;
	Button * b;

	AddElement(g_logicStepManager_, g_stepCheckFocus_);
	AddElement(g_logicStepManager_, l_stepNameInput_);

	ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_mainTitle2);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 400, 640, 105, 50, RenderSimple, &g_img_back);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickLoadMain, ButtonFocusDefault, ButtonLeaveDefault, &g_img_back, NULL, &g_img_backFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 200, 300, 105, 50, RenderSimple, &g_img_save);
	AddElement(g_logicSpriteManager_, ls);
	b = CreateButton(ls, ButtonClickSave, ButtonFocusDefault, ButtonLeaveDefault, &g_img_save, NULL, &g_img_saveFocus);
	AddButton(b);

	ls = CreateLogicSprite(NULL, NULL, 150, 200, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderName, NULL);
	AddElement(g_logicSpriteManager_, ls);

	memset(l_keyDown, 0, sizeof(l_keyDown));
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

void ButtonClickLoadFile(Button * tis){
	LoadGameFromFile("save.dat");
	LoadScene(SCENE_GAME);
}

void ButtonClickEasy(Button * tis){
	g_gameDifficulty_ = GDFT_EASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

void ButtonClickVeryEasy(Button * tis){
	g_gameDifficulty_ = GDFT_VEASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

void ButtonClickExtremelyEasy(Button * tis){
	g_gameDifficulty_ = GDFT_EEASY;
	LoadScene(SCENE_WAVE_CHOOSE);
}

static void StepWaveChange(int t, LogicStep* tis) {
	if (g_keyboardState_.left_up) {
		if (twave > 1) --twave; else twave = V6_GAME_MAX_WAVE;
	}
	if (g_keyboardState_.right_up) {
		if (twave < V6_GAME_MAX_WAVE) ++twave; else twave = 1;
	}
}