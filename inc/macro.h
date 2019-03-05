#pragma once

// 通用。
#define NULL					0
#define BOOLean					int
#define TRUE					1
#define FALSE					0
#define V6_PI					3.1415927

// 窗体信息。
#define V6_WINDOWWIDTH			1024
#define V6_WINDOWHEIGHT			768

// 逻辑引擎环境信息。
#define V6_UPDATEINTERVAL		16

// 渲染引擎环境信息。
#define V6_TRANSPARENT_COLOR	0x0000FF

// 输入信息。
#define V6_MOUSE_LEFT			VK_LBUTTON
#define V6_KEY_LEFT				65
#define V6_KEY_RIGHT			68
#define V6_KEY_UP				87
#define V6_KEY_DOWN				83
#define V6_KEY_SHOOT			V6_MOUSE_LEFT
#define V6_KEY_SET_WALL			88
#define V6_KEY_PAUSE			80
#define V6_KEY_ESC				27

// 游戏信息。
// 最大波数。
#define V6_GAME_MAX_WAVE		30