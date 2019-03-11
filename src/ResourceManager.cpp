#include "ResourceManager.h"
#include "LogicControl.h"
#include "Scene.h"
#include "Game.h"

void LoadImages();

// 从右朝向的图片加载其他方向的图片。
void LoadDirections(IMAGE* imr) {
	rotateimage(&imr[DRT_UP], imr, V6_PI / 2, WHITE);
	rotateimage(&imr[DRT_LEFT], imr, V6_PI, WHITE);
	rotateimage(&imr[DRT_DOWN], imr, V6_PI * 3 / 2, WHITE);
}

void LoadResources()
{
	LoadLogicControlStepResource();
	LoadSceneStepResource();
	LoadGameStepResource();
	LoadImages();
}

void LoadImages() {
	loadimage(&g_img_bigTank, _T("assets\\bigtank.png"));
	loadimage(&g_img_bigTankBattery, _T("assets\\bigtank_battery.png"));
	loadimage(&g_img_bossTank, _T("assets\\bosstank.png"));
	loadimage(&g_img_castle, _T("assets\\castle.png"));
	loadimage(&g_img_exit, _T("assets\\exit.png"));
	loadimage(&g_img_fiveTank[0], _T("assets\\fivetank.png"));
	LoadDirections(g_img_fiveTank);
	loadimage(&g_img_junkTank[0], _T("assets\\junktank.png"));
	LoadDirections(g_img_junkTank);
	loadimage(&g_img_lanlingkingTank[0], _T("assets\\lanlingkingtank.png"));
	LoadDirections(g_img_lanlingkingTank);
	loadimage(&g_img_mainTitle, _T("assets\\maintitle.png"));
	loadimage(&g_img_prismTank[0], _T("assets\\prismtank.png"));
	LoadDirections(g_img_prismTank);
	loadimage(&g_img_start, _T("assets\\start.png"));
	loadimage(&g_img_vegetableTank[0], _T("assets\\vegetabletank.png"));
	LoadDirections(g_img_vegetableTank);
	loadimage(&g_img_wallWorn0, _T("assets\\wallworn0.png"));
	loadimage(&g_img_wallWorn1, _T("assets\\wallworn1.png"));
	loadimage(&g_img_wallWorn2, _T("assets\\wallworn2.png"));
	loadimage(&g_img_wallWorn3, _T("assets\\wallworn3.png"));
	loadimage(&g_img_ankhWall, _T("assets\\ankhwall.png"));
	loadimage(&g_img_wallingWall, _T("assets\\wailingwall.png"));
	loadimage(&g_img_ifWall, _T("assets\\ifwall.png"));
	loadimage(&g_img_startFocus, _T("assets\\startfocus.png"));
	loadimage(&g_img_exitFocus, _T("assets\\exitfocus.png"));
	loadimage(&g_img_rankingList, _T("assets\\rankinglist.png"));
	loadimage(&g_img_rankingListFocus, _T("assets\\rankinglistfocus.png"));
	loadimage(&g_img_heart, _T("assets\\heart.png"));
	loadimage(&g_img_hp, _T("assets\\hp.png"));
	loadimage(&g_img_emptyHp, _T("assets\\emptyHp.png"));
	loadimage(&g_img_help, _T("assets\\help.png"));
	loadimage(&g_img_helpFocus, _T("assets\\helpfocus.png"));
	loadimage(&g_img_helpDetail, _T("assets\\helpdetail.png"));
	loadimage(&g_img_back, _T("assets\\back.png"));
	loadimage(&g_img_backFocus, _T("assets\\backfocus.png"));
	loadimage(&g_img_bigTankBatteryMsk, _T("assets\\bigTank_Battery_Msk.png"));
	loadimage(&g_img_bigTankMsk, _T("assets\\bigTank_Msk.png"));
	loadimage(&g_img_bossTankMsk, _T("assets\\bossTank_Msk.png"));
	loadimage(&g_img_castleMsk, _T("assets\\castle_Msk.png"));
	loadimage(&g_img_emptyHpMsk, _T("assets\\emptyHp_Msk.png"));
	loadimage(&g_img_fiveTankMsk[0], _T("assets\\fiveTank_Msk.png"));
	LoadDirections(g_img_fiveTankMsk);
	loadimage(&g_img_heartMsk, _T("assets\\heart_Msk.png"));
	loadimage(&g_img_hpMsk, _T("assets\\hp_Msk.png"));
	loadimage(&g_img_junkTankMsk[0], _T("assets\\junkTank_Msk.png"));
	LoadDirections(g_img_junkTankMsk);
	loadimage(&g_img_lanlingkingTankMsk[0], _T("assets\\lanlingkingTank_Msk.png"));
	LoadDirections(g_img_lanlingkingTankMsk);
	loadimage(&g_img_vegetableTankMsk[0], _T("assets\\vegetableTank_Msk.png"));
	LoadDirections(g_img_vegetableTankMsk);
	loadimage(&g_img_bigBulletMsk, _T("assets\\bigBullet_Msk.png"));
	loadimage(&g_img_prismTankMsk[0], _T("assets\\prismTank_Msk.png"));
	LoadDirections(g_img_prismTankMsk);
	loadimage(&g_img_smallBulletMsk, _T("assets\\smallBullet_Msk.png"));
	loadimage(&g_img_bigBullet, _T("assets\\bigBullet.png"));
	loadimage(&g_img_smallBullet, _T("assets\\smallbullet.png"));
	loadimage(&g_img_mainTitle2, _T("assets\\maintitle2.png"));
	loadimage(&g_img_save, _T("assets\\save.png"));
	loadimage(&g_img_saveFocus, _T("assets\\saveFocus.png"));
	loadimage(&g_img_loadGame, _T("assets\\loadGame.png"));
	loadimage(&g_img_loadGameFocus, _T("assets\\loadGameFocus.png"));
	loadimage(&g_img_easy, _T("assets\\easy.png"));
	loadimage(&g_img_easyFocus, _T("assets\\easyFocus.png"));
	loadimage(&g_img_veasy, _T("assets\\veasy.png"));
	loadimage(&g_img_veasyFocus, _T("assets\\veasyFocus.png"));
	loadimage(&g_img_eeasy, _T("assets\\eeasy.png"));
	loadimage(&g_img_eeasyFocus, _T("assets\\eeasyFocus.png"));
	loadimage(&g_img_attackTank[0], _T("assets\\attackTank.png"));
	LoadDirections(g_img_attackTank);
	loadimage(&g_img_attackTankMsk[0], _T("assets\\attackTank_Msk.png"));
	LoadDirections(g_img_attackTankMsk);
	loadimage(&g_img_sunTank, _T("assets\\sunTank.png"));
	loadimage(&g_img_sunTankMsk, _T("assets\\sunTank_Msk.png"));
	loadimage(&g_img_quickTank[0], _T("assets\\quickTank.png"));
	LoadDirections(g_img_quickTank);
	loadimage(&g_img_quickTankMsk[0], _T("assets\\quickTank_Msk.png"));
	LoadDirections(g_img_quickTankMsk);
	loadimage(&g_img_console, _T("assets\\console.png"));
	loadimage(&g_img_gameOver, _T("assets\\gameOver.png"));
}

IMAGE g_img_bigTank;
IMAGE g_img_bigTankBattery;
IMAGE g_img_bossTank;
IMAGE g_img_castle;
IMAGE g_img_exit;
IMAGE g_img_fiveTank[4];
IMAGE g_img_vegetableTank[4];
IMAGE g_img_prismTank[4];
IMAGE g_img_junkTank[4];
IMAGE g_img_lanlingkingTank[4];
IMAGE g_img_mainTitle;
IMAGE g_img_start;
IMAGE g_img_wallWorn0;
IMAGE g_img_wallWorn1;
IMAGE g_img_wallWorn2;
IMAGE g_img_wallWorn3;
IMAGE g_img_ankhWall;
IMAGE g_img_wallingWall;
IMAGE g_img_ifWall;
IMAGE g_img_startFocus;
IMAGE g_img_exitFocus;
IMAGE g_img_rankingList;
IMAGE g_img_rankingListFocus;
IMAGE g_img_heart;
IMAGE g_img_hp;
IMAGE g_img_emptyHp;
IMAGE g_img_help;
IMAGE g_img_helpFocus;
IMAGE g_img_helpDetail;
IMAGE g_img_back;
IMAGE g_img_backFocus;
IMAGE g_img_bigTankBatteryMsk;
IMAGE g_img_bigTankMsk;
IMAGE g_img_bossTankMsk;
IMAGE g_img_castleMsk;
IMAGE g_img_emptyHpMsk;
IMAGE g_img_fiveTankMsk[4];
IMAGE g_img_heartMsk;
IMAGE g_img_hpMsk;
IMAGE g_img_junkTankMsk[4];
IMAGE g_img_lanlingkingTankMsk[4];
IMAGE g_img_vegetableTankMsk[4];
IMAGE g_img_bigBulletMsk;
IMAGE g_img_prismTankMsk[4];
IMAGE g_img_smallBulletMsk;
IMAGE g_img_bigBullet;
IMAGE g_img_smallBullet;
IMAGE g_img_mainTitle2;
IMAGE g_img_save;
IMAGE g_img_saveFocus;
IMAGE g_img_loadGame;
IMAGE g_img_loadGameFocus;
IMAGE g_img_easy;
IMAGE g_img_easyFocus;
IMAGE g_img_veasy;
IMAGE g_img_veasyFocus;
IMAGE g_img_eeasy;
IMAGE g_img_eeasyFocus;
IMAGE g_img_attackTank[4];
IMAGE g_img_attackTankMsk[4];
IMAGE g_img_sunTank;
IMAGE g_img_sunTankMsk;
IMAGE g_img_quickTank[4];
IMAGE g_img_quickTankMsk[4];
IMAGE g_img_console; 
IMAGE g_img_gameOver;
