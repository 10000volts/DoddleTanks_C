#include <Windows.h>
#include "LogicControl.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "DataModels.h"
#include "RandomEngine.h"

#pragma comment(lib,"winmm.lib")

// ��ʼ����Ϸ���á�
// ��Ҫ����ȫ��/��Ա�����������벻Ҫ�޸����ǡ�
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