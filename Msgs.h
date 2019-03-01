#pragma once

#include <easyx.h>
#include "stdafx.h"

typedef struct _KEYBOARDSTATE {
	// ���Ƿ񱻰��£��ϡ�����
	BOOLean left_down, up_down, right_down, down_down, pause_down, esc_down;
} KeyBoardState;

// �����û�������״̬��
void UpdateStates();

// ���µ����״̬��
extern MOUSEMSG g_mouseState_;
// ���µļ���״̬��
extern KeyBoardState g_keyboardState_;