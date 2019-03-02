#pragma once

#include <string.h>
#include "stdafx.h"
#include "Msgs.h"
#include "RenderEngine.h"

typedef struct _LogicSprite {
	// ���ش��߼����������ʵ�塣
	void* m_me_;
	void (*Update)(void* _this, int t);
	// �����꣬�����ꡣ
	int m_x_, m_y_, m_w_, m_h_;
	double m_angle_;

	RenderSprite* m_body_;
} LogicSprite;

typedef struct _LogicStep {
	void (*Update)(struct _LogicStep* _this);

	char m_stepName_[32];
} LogicStep;

// �߼����¡�
void LogicUpdate();
void UnloadLogicEngine();

void AddLogicSprite(LogicSprite* ls);
void RemoveLogicSprite(LogicSprite* ls);
void ClearLogicSprites();
void AddLogicStep(LogicStep* ls);
void RemoveLogicStep(LogicStep* ls);
void ClearLogicSteps();

LogicSprite* CreateLogicSprite(void* me, void(*update)(void*, int), int x, int y, int w, int h, IMAGE* im, IMAGE* msk = NULL);
LogicStep* CreateLogicStep(char stepname[], void(*update)(LogicStep* _this));

// �����Ƿ�Ӧ�ü������С�
extern BOOLean g_running_;

// ȫ���߼�����ļ��ϣ�����Ϊ������Ԫ�صĵ�ַ������vector�Ļ��ƽ����ڴ����
extern LogicSprite* m_logicSpriteManager_;
extern int logicSpriteCount_;
// �߼����鼯�ϵ�������
extern int logicSpriteReserve_;
// ȫ���߼�����ļ��ϣ�����Ϊ������Ԫ�صĵ�ַ������vector�Ļ��ƽ����ڴ����
extern LogicStep** m_logicStepManager_;
extern int logicStepCount_;
// �߼����輯�ϵ�������
extern int logicStepReserve_;