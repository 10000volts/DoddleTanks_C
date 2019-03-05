#include <Windows.h>
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Scene.h"

#pragma comment(lib,"winmm.lib")

// 初始化游戏设置。
// 不要访问全局/成员变量。至少请不要修改它们。
void Initialize() {
	LoadResources();
	LoadScene(SCENE_MAIN);
	initgraph(V6_WINDOWWIDTH, V6_WINDOWHEIGHT);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWeight = 700;
	_tcscpy(f.lfFaceName, _T("幼圆"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
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