#pragma once

#include <easyx.h>
#include "stdafx.h"

typedef struct _KEYBOARDSTATE {
	// ���Ƿ񱻰��£��ϡ�����
	BOOLean left_down, up_down, right_down, down_down, shoot_down, set_wall_down, pause_down, esc_down, slow_down,
		num1_down, num2_down, num3_down;
	// ���Ƿ��𣿡���
	BOOLean left_up, right_up, num1_up, num2_up, num3_up;
} KeyBoardState;

typedef struct _MOUSESTATE {
	LONG x, y;
	BOOLean left_down;
	BOOLean left_up;
} MouseState;

// ��ʼ�����뻷������Ҫ���ھ����
void InitializeInputEnv();

// �����û�������״̬��
void InputUpdate();

// ���µ����״̬��
extern MouseState g_mouseState_;
// ���µļ���״̬��
extern KeyBoardState g_keyboardState_;