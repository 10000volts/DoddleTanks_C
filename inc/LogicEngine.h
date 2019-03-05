#pragma once

#include <string.h>
#include "stdafx.h"
#include "InputEngine.h"
#include "RenderEngine.h"

typedef struct _LogicSprite {
	// 返回此逻辑精灵的真正实体。
	void* m_me_;
	void (*Update)(struct _LogicSprite* _this);
	// 横坐标，纵坐标。
	int m_x_, m_y_, m_w_, m_h_;
	double m_angle_;

	RenderSprite* m_body_;
} LogicSprite;

typedef struct _LogicStep {
	void (*Update)(struct _LogicStep* _this);

	char m_stepName_[32];
} LogicStep;

// 逻辑更新。
void LogicUpdate();
void UnloadLogicEngine();

// 将ls的副本添加至集合，之后释放ls。
void AddLogicSprite(LogicSprite* ls);
void RemoveLogicSprite(LogicSprite* ls);
void ClearLogicSprites();
void AddLogicStep(LogicStep* ls);
void RemoveLogicStep(LogicStep* ls);
void ClearLogicSteps();

LogicSprite* CreateLogicSprite(void* me, void(*update)(LogicSprite*), int x, int y, int w, int h, void(*render)(LogicSprite* ls), IMAGE* im,
	IMAGE* msk = NULL);
LogicStep* CreateLogicStep(char stepname[], void(*update)(LogicStep* _this));

// 程序是否应该继续运行。
extern BOOLean g_running_;

// 全部逻辑精灵的集合，本身为其中首元素的地址。仿照vector的机制进行内存管理。
extern LogicSprite* g_logicSpriteManager_;
extern int logicSpriteCount_;
// 逻辑精灵集合的容量。
extern int logicSpriteReserve_;
// 全部逻辑步骤的集合，本身为其中首元素的地址。仿照vector的机制进行内存管理。
extern LogicStep** g_logicStepManager_;
extern int logicStepCount_;
// 逻辑步骤集合的容量。
extern int logicStepReserve_;