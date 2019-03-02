#pragma once

#include <easyx.h>
#include "stdafx.h"

typedef struct _KEYBOARDSTATE {
	// 左是否被按下？上……？
	BOOLean left_down, up_down, right_down, down_down, shoot_down, set_wall_down, pause_down, esc_down;
} KeyBoardState;

typedef struct _MOUSESTATE {
	LONG x, y;
	BOOLean left_down;
} MouseState;

// 更新用户的输入状态。
void UpdateStates();

// 最新的鼠标状态。
extern MouseState g_mouseState_;
// 最新的键盘状态。
extern KeyBoardState g_keyboardState_;