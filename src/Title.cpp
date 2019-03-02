#include <Windows.h>
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Scene.h"

#pragma comment(lib,"winmm.lib")

// 初始化游戏设置。
void Initialize() {
	LoadResources();
	LoadScene(SCENE_MAIN);
	initgraph(V6_WINDOWWIDTH, V6_WINDOWHEIGHT);
}

// 不要访问全局/成员变量。至少请不要修改它们。
int main() {
	DWORD dwBegin, dwEnd, t;
	Initialize();
	while (TRUE) {
		dwBegin = timeGetTime();

		LogicUpdate();
		RenderUpdate();

		dwEnd = timeGetTime();
		t = dwEnd - dwBegin;
		if(t < 20) Sleep(V6_UPDATEINTERVAL - t);
	}
	closegraph();
	return 0;
}