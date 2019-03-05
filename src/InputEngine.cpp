#include <Windows.h>
#include <imm.h>
#include "stdafx.h"
#include "InputEngine.h"

#pragma comment (lib ,"imm32.lib")

typedef int KEY;

static POINT tp;
static LPPOINT tpos = &tp;
static BOOLean mouse_down_state_changed, keyleft_down_state_changed, keyright_down_state_changed,
			   num1_down_state_changed, num2_down_state_changed, num3_down_state_changed;

void CheckUpAndDown(KEY k, BOOLean* down_state_changed, BOOLean* checkup, BOOLean* checkdown) {
	*checkup = FALSE;
	if (GetAsyncKeyState(k)) {
		*checkdown = TRUE;
		*down_state_changed = TRUE;
	}
	else {
		*checkdown = FALSE;
		if (*down_state_changed) {
			*checkup = TRUE;
		}
		*down_state_changed = FALSE;
	}
}

void InitializeInputEnv()
{
	HIMC himc = ImmGetContext(GetHWnd());
	ImmAssociateContext(GetHWnd(), NULL);
	ImmReleaseContext(GetHWnd(), himc);
}

// 更新用户的输入状态。
void InputUpdate() {
	GetCursorPos(tpos);
	ScreenToClient(GetHWnd(), tpos);
	g_mouseState_.x = tpos->x;
	g_mouseState_.y = tpos->y;
	CheckUpAndDown(V6_MOUSE_LEFT, &mouse_down_state_changed, &g_mouseState_.left_up, &g_mouseState_.left_down);
	if (GetAsyncKeyState(V6_KEY_SHOOT)) g_keyboardState_.shoot_down = TRUE; else g_keyboardState_.shoot_down = FALSE;
	if (GetAsyncKeyState(V6_KEY_SET_WALL)) g_keyboardState_.set_wall_down = TRUE; else g_keyboardState_.set_wall_down = FALSE;
	if (GetAsyncKeyState(V6_KEY_SLOW)) g_keyboardState_.slow_down = TRUE; else g_keyboardState_.slow_down = FALSE;
	CheckUpAndDown(V6_KEY_LEFT, &keyleft_down_state_changed, &g_keyboardState_.left_up, &g_keyboardState_.left_down);
	if (GetAsyncKeyState(V6_KEY_UP)) g_keyboardState_.up_down = TRUE; else g_keyboardState_.up_down = FALSE;
	CheckUpAndDown(V6_KEY_RIGHT, &keyright_down_state_changed, &g_keyboardState_.right_up, &g_keyboardState_.right_down);
	if (GetAsyncKeyState(V6_KEY_DOWN)) g_keyboardState_.down_down = TRUE; else g_keyboardState_.down_down = FALSE;
 	if (GetAsyncKeyState(V6_KEY_PAUSE)) g_keyboardState_.pause_down = TRUE; else g_keyboardState_.pause_down = FALSE;
	if (GetAsyncKeyState(V6_KEY_ESC)) g_keyboardState_.esc_down = TRUE; else g_keyboardState_.esc_down = FALSE;
	CheckUpAndDown(V6_KEY_NUM1, &num1_down_state_changed, &g_keyboardState_.num1_up, &g_keyboardState_.num1_down);
	CheckUpAndDown(V6_KEY_NUM2, &num2_down_state_changed, &g_keyboardState_.num2_up, &g_keyboardState_.num2_down);
	CheckUpAndDown(V6_KEY_NUM3, &num3_down_state_changed, &g_keyboardState_.num3_up, &g_keyboardState_.num3_down);
}

// 最新的鼠标状态。
MouseState g_mouseState_;
// 最新的键盘状态。
KeyBoardState g_keyboardState_;