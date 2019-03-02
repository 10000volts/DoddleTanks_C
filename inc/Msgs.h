#pragma once

#include <easyx.h>
#include "stdafx.h"

typedef struct _KEYBOARDSTATE {
	// ���Ƿ񱻰��£��ϡ�����
	BOOLean left_down, up_down, right_down, down_down, shoot_down, set_wall_down, pause_down, esc_down;
} KeyBoardState;

typedef struct _MOUSESTATE {
	LONG x, y;
	BOOLean left_down;
} MouseState;

// �����û�������״̬��
void UpdateStates();

// ���µ����״̬��
extern MouseState g_mouseState_;
// ���µļ���״̬��
extern KeyBoardState g_keyboardState_;