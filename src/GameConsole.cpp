#include "GameConsole.h"
#include "InputEngine.h"
#include "LogicEngine.h"

static int easyProcess;
static void StepCheckActiveEasyMode(int t, LogicStep* tis) {
	if (g_keyboardState_.right_up == TRUE) {
		++easyProcess;
	}
	else if (g_keyboardState_.left_up == TRUE | g_keyboardState_.set_wall_up == TRUE) {
		easyProcess = 0;
	}
	if (easyProcess == 3) {
		g_easyMode_ = TRUE;
	}
}

void LoadGameConsoleStepResource() {
	g_stepCheckActiveEasyMode_ = CreateLogicStep((char*)"CheckActiveEasyMode", StepCheckActiveEasyMode);
}

BOOLean g_easyMode_;

// 检测是否开启简单模式。
LogicStep* g_stepCheckActiveEasyMode_;