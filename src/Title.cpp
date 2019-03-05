#include <Windows.h>
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "DataModels.h"
#include "RandomEngine.h"

#pragma comment(lib,"winmm.lib")

// 初始化游戏设置。
// 不要访问全局/成员变量。至少请不要修改它们。
void Initialize() {
	LoadResources();
	FillData();
	InitializeRandomEngine();
	LoadScene(SCENE_MAIN);
	initgraph(V6_WINDOWWIDTH, V6_WINDOWHEIGHT);
	InitializeInputEnv();
	InitializeRenderEnv();
}

int main() {
	DWORD dwBegin, dwEnd, t;
	Initialize();
	while (TRUE) {
		dwBegin = timeGetTime();

		InputUpdate();
		LogicUpdate();
		RenderUpdate();

		dwEnd = timeGetTime();
		t = dwEnd - dwBegin;
		if(t < V6_UPDATEINTERVAL) Sleep(V6_UPDATEINTERVAL - t);
	}
	closegraph();
	return 0;
}