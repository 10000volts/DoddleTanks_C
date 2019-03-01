#include <Windows.h>
#include "stdafx.h"
#include "Msgs.h"

// 更新用户的输入状态。
void UpdateStates() {
	if (MouseHit()) {
		g_mouseState_ = GetMouseMsg();
	}
	g_keyboardState_.left_down = false;
	g_keyboardState_.up_down = false;
	g_keyboardState_.right_down = false;
	g_keyboardState_.down_down = false;
	g_keyboardState_.pause_down = false;
	g_keyboardState_.esc_down = false;
	if (GetAsyncKeyState(V6_KEY_LEFT)) g_keyboardState_.left_down = true;
	if (GetAsyncKeyState(V6_KEY_UP)) g_keyboardState_.up_down = true;
	if (GetAsyncKeyState(V6_KEY_RIGHT)) g_keyboardState_.right_down = true;
	if (GetAsyncKeyState(V6_KEY_DOWN)) g_keyboardState_.down_down = true;
	if (GetAsyncKeyState(V6_KEY_PAUSE)) g_keyboardState_.pause_down = true;
	if (GetAsyncKeyState(V6_KEY_ESC)) g_keyboardState_.esc_down = true;
}

// 最新的鼠标状态。
MOUSEMSG g_mouseState_;
// 最新的键盘状态。
KeyBoardState g_keyboardState_;