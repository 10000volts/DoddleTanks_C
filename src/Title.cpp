#include <Windows.h>
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Scene.h"

#pragma comment(lib,"winmm.lib")

// ��ʼ����Ϸ���á�
void Initialize() {
	LoadResources();
	LoadScene(SCENE_MAIN);
	initgraph(V6_WINDOWWIDTH, V6_WINDOWHEIGHT);
}

// ��Ҫ����ȫ��/��Ա�����������벻Ҫ�޸����ǡ�
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