// ��Ϸ����̨���������ڵ�����Ϸ���ݡ�(����)

#pragma once

#include "stdafx.h"

typedef struct _LogicStep LogicStep;

void LoadGameConsoleStepResource();

// �Ƿ�����ģʽ��(��������10000���÷֡�1/10)
extern BOOLean g_easyMode_;

// ����Ƿ�����ģʽ��
extern LogicStep* g_stepCheckActiveEasyMode_;