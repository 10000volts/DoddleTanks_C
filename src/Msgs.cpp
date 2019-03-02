#include <Windows.h>
#include "stdafx.h"
#include "Msgs.h"

POINT tp;
LPPOINT tpos = &tp;
// 更新用户的输入状态。
void UpdateStates() {
	GetCursorPos(tpos);
	ScreenToClient(GetHWnd(), tpos);
	g_mouseState_.x = tpos->x;
	g_mouseState_.y = tpos->y;
	if (GetAsyncKeyState(V6_MOUSE_LEFT)) g_mouseState_.left_down = true; else g_mouseState_.left_down = false;
	if (GetAsyncKeyState(V6_KEY_SHOOT)) g_keyboardState_.shoot_down = true; else g_keyboardState_.shoot_down = false;
	if (GetAsyncKeyState(V6_KEY_SET_WALL)) g_keyboardState_.set_wall_down = true; else g_keyboardState_.set_wall_down = false;
	if (GetAsyncKeyState(V6_KEY_LEFT)) g_keyboardState_.left_down = true; else g_keyboardState_.left_down = false;
	if (GetAsyncKeyState(V6_KEY_UP)) g_keyboardState_.up_down = true; else g_keyboardState_.up_down = false;
	if (GetAsyncKeyState(V6_KEY_RIGHT)) g_keyboardState_.right_down = true; else g_keyboardState_.right_down = false;
	if (GetAsyncKeyState(V6_KEY_DOWN)) g_keyboardState_.down_down = true; else g_keyboardState_.down_down = false;
 	if (GetAsyncKeyState(V6_KEY_PAUSE)) g_keyboardState_.pause_down = true; else g_keyboardState_.pause_down = false;
	if (GetAsyncKeyState(V6_KEY_ESC)) g_keyboardState_.esc_down = true; else g_keyboardState_.esc_down = false;
}

// 最新的鼠标状态。
MouseState g_mouseState_;
// 最新的键盘状态。
KeyBoardState g_keyboardState_;