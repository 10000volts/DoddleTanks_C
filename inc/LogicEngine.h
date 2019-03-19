#pragma once

#include <string.h>
#include "stdafx.h"
#include "InputEngine.h"
#include "RenderEngine.h"
#include "Container.h"

typedef struct _LogicSprite {
	// 返回此逻辑精灵的真正实体。
	void* m_me_;
	// 更新回调函数。函数体中不能对Manager有任何修改的操作。
	void (*Update)(int t, struct _LogicSprite* _this);
	// 横坐标，纵坐标。
	double m_x_, m_y_, m_w_, m_h_;
	double m_angle_;

	RenderSprite* m_body_;

	BOOLean m_valid_;
} LogicSprite;

typedef struct _LogicStep {
	void (*Update)(int t, struct _LogicStep* _this);

	char m_stepName_[32];
} LogicStep;

// 逻辑更新。
void LogicUpdate(int t);
void UnloadLogicEngine();

LogicSprite* CreateLogicSprite(void* me, void(*update)(int t, LogicSprite*), int x, int y, int w, int h, void(*render)(LogicSprite* ls), IMAGE* im,
	IMAGE* msk = NULL);
LogicStep* CreateLogicStep(char stepname[], void(*update)(int t, LogicStep* _this));

// 初始化加载逻辑步骤资源。
void LoadLogicSpriteStepResource();

// 逻辑步骤在遍历执行时是否因为已经修改而无法继续执行。
extern BOOLean g_stepInvalid_;

// 全部逻辑精灵的集合。
extern Container* g_logicSpriteManager_;
// 全部逻辑步骤的集合。
extern Container* g_logicStepManager_;

// LogicStep资源。
extern LogicStep* g_stepCheckSpriteValid_;