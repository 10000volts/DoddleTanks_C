#pragma once

#include <easyx.h>
#include "stdafx.h"

typedef struct _KEYBOARDSTATE {
	// ���Ƿ񱻰��£��ϡ�����
	BOOLean left_down, up_down, right_down, down_down, shoot_down, set_wall_down, pause_down, esc_down;
	// ���Ƿ��𣿡���
	BOOLean left_up, right_up;
} KeyBoardState;

typedef struct _MOUSESTATE {
	LONG x, y;
	BOOLean left_down;
	BOOLean left_up;
} MouseState;

// �����û�������״̬��
void InputUpdate();

// ���µ����״̬��
extern MouseState g_mouseState_;
// ���µļ���״̬��
extern KeyBoardState g_keyboardState_;