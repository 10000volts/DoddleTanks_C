#include "ResourceManager.h"
#include "LogicControl.h"

void LoadImages();

void LoadResources()
{
	LoadLogicControlStepResource();
	LoadImages();
}

void LoadImages() {
	loadimage(&g_img_bigTank, _T("assets\\bigtank.png"));
	loadimage(&g_img_bigTankBattery, _T("assets\\bigtank_battery.png"));
	loadimage(&g_img_bossTank, _T("assets\\bosstank.png"));
	loadimage(&g_img_castle, _T("assets\\castle.png"));
	loadimage(&g_img_exit, _T("assets\\exit.png"));
	loadimage(&g_img_fiveTank, _T("assets\\fivetank.png"));
	loadimage(&g_img_junkTank, _T("assets\\junktank.png"));
	loadimage(&g_img_lanlingkingTank, _T("assets\\lanlingkingtank.png"));
	loadimage(&g_img_mainTitle, _T("assets\\maintitle.png"));
	loadimage(&g_img_prismTank, _T("assets\\prismtank.png"));
	loadimage(&g_img_start, _T("assets\\start.png"));
	loadimage(&g_img_vegetableTank, _T("assets\\vegetabletank.png"));
	loadimage(&g_img_wallWorn0, _T("assets\\wallworn0.png"));
	loadimage(&g_img_wallWorn1, _T("assets\\wallworn1.png"));
	loadimage(&g_img_wallWorn2, _T("assets\\wallworn0.png"));
	loadimage(&g_img_wallWorn3, _T("assets\\wallworn0.png"));
	loadimage(&g_img_ankhWall, _T("assets\\ankhwall.png"));
	loadimage(&g_img_wallingWall, _T("assets\\wailingwall.png"));
	loadimage(&g_img_ifWall, _T("assets\\ifwall.png"));
	loadimage(&g_img_startFocus, _T("assets\\startfocus.png"));
	loadimage(&g_img_exitFocus, _T("assets\\exitfocus.png"));
	loadimage(&g_img_rankingList, _T("assets\\rankinglist.png"));
	loadimage(&g_img_rankingListFocus, _T("assets\\rankinglistfocus.png"));
	loadimage(&g_img_heart, _T("assets\\heart.png"));
	loadimage(&g_img_hp, _T("assets\\hp.png"));
	loadimage(&g_img_hpEmpty, _T("assets\\hpempty.png"));
	loadimage(&g_img_help, _T("assets\\help.png"));
	loadimage(&g_img_helpFocus, _T("assets\\helpfocus.png"));
	loadimage(&g_img_helpDetail, _T("assets\\helpdetail.png"));
	loadimage(&g_img_back, _T("assets\\back.png"));
	loadimage(&g_img_backFocus, _T("assets\\backfocus.png"));
}

IMAGE g_img_bigTank;
IMAGE g_img_bigTankBattery;
IMAGE g_img_bossTank;
IMAGE g_img_castle;
IMAGE g_img_exit;
IMAGE g_img_fiveTank;
IMAGE g_img_vegetableTank;
IMAGE g_img_prismTank;
IMAGE g_img_junkTank;
IMAGE g_img_lanlingkingTank;
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
IMAGE g_img_hpEmpty;
IMAGE g_img_help;
IMAGE g_img_helpFocus;
IMAGE g_img_helpDetail;
IMAGE g_img_back;
IMAGE g_img_backFocus;