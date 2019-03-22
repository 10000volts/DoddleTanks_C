#include <Windows.h>
#include "InputEngine.h"
#include "LogicControl.h"
#include "RenderEngine.h"
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
	DWORD dwBegin, dwEnd, t = (DWORD)0;
	Initialize();
	while (TRUE) {
		dwBegin = timeGetTime();

		InputUpdate();
		LogicUpdate(t);
		RenderUpdate();

		dwEnd = timeGetTime();
		t = dwEnd - dwBegin;
		if(t > V6_UPDATE_MAX_INTERVAL) t = V6_UPDATE_MAX_INTERVAL;
	}
	closegraph();
	return 0;
}