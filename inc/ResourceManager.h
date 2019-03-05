#pragma once

#include <easyx.h>

// 开辟新的内存空间以加载公共资源。这些资源不应该在运行中被释放。
void LoadResources();

typedef enum _DIRECTION {
	DRT_RIGHT, DRT_UP, DRT_LEFT, DRT_DOWN
} DIRECTION;

extern IMAGE g_img_bigTank[4];
extern IMAGE g_img_bigTankBattery;
extern IMAGE g_img_bossTank;
extern IMAGE g_img_castle;
extern IMAGE g_img_exit;
extern IMAGE g_img_fiveTank[4];
extern IMAGE g_img_vegetableTank[4];
extern IMAGE g_img_prismTank[4];
extern IMAGE g_img_junkTank[4];
extern IMAGE g_img_lanlingkingTank[4];
extern IMAGE g_img_mainTitle;
extern IMAGE g_img_start;
extern IMAGE g_img_wallWorn0;
extern IMAGE g_img_wallWorn1;
extern IMAGE g_img_wallWorn2;
extern IMAGE g_img_wallWorn3;
extern IMAGE g_img_ankhWall;
extern IMAGE g_img_wallingWall;
extern IMAGE g_img_ifWall;
extern IMAGE g_img_startFocus;
extern IMAGE g_img_exitFocus;
extern IMAGE g_img_rankingList;
extern IMAGE g_img_rankingListFocus;
extern IMAGE g_img_heart;
extern IMAGE g_img_hp;
extern IMAGE g_img_hpEmpty;
extern IMAGE g_img_help;
extern IMAGE g_img_helpFocus;
extern IMAGE g_img_helpDetail;
extern IMAGE g_img_back;
extern IMAGE g_img_backFocus;
extern IMAGE g_img_bigTankBatteryMsk;
extern IMAGE g_img_bigTankMsk[4];
extern IMAGE g_img_bossTankMsk;
extern IMAGE g_img_castleMsk;
extern IMAGE g_img_emptyHpMsk;
extern IMAGE g_img_fiveTankMsk[4];
extern IMAGE g_img_heartMsk;
extern IMAGE g_img_hpMsk;
extern IMAGE g_img_junkTankMsk[4];
extern IMAGE g_img_lanlingkingTankMsk[4];
extern IMAGE g_img_vegetableTankMsk[4];
extern IMAGE g_img_bigBulletMsk;
extern IMAGE g_img_prismTankMsk[4];
extern IMAGE g_img_smallBulletMsk;
extern IMAGE g_img_bigBullet;
extern IMAGE g_img_smallBullet;
extern IMAGE g_img_mainTitle2;
extern IMAGE g_img_save;
extern IMAGE g_img_saveFocus;
extern IMAGE g_img_loadGame;
extern IMAGE g_img_loadGameFocus;
extern IMAGE g_img_easy;
extern IMAGE g_img_easyFocus;
extern IMAGE g_img_veasy;
extern IMAGE g_img_veasyFocus;
extern IMAGE g_img_eeasy;
extern IMAGE g_img_eeasyFocus;