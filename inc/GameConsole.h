// 游戏控制台。可以用于调节游戏数据。(作弊)

#pragma once

#include "stdafx.h"

typedef struct _LogicStep LogicStep;

void LoadGameConsoleStepResource();

// 是否开启简单模式。(生命上限10000，得分×1/10)
extern BOOLean g_easyMode_;

// 检测是否开启简单模式。
extern LogicStep* g_stepCheckActiveEasyMode_;