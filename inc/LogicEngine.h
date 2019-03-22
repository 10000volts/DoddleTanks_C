#pragma once

#include <string.h>
#include "stdafx.h"
#include "Container.h"

typedef struct _RenderSprite RenderSprite;
class IMAGE;

typedef struct _LogicSprite {
	// ���ش��߼����������ʵ�塣
	void* m_me_;
	// ���»ص��������������в��ܶ�Manager���κ��޸ĵĲ�����
	void (*Update)(int t, struct _LogicSprite* _this);
	// �����꣬�����ꡣ
	double m_x_, m_y_, m_w_, m_h_;
	double m_angle_;

	RenderSprite* m_body_;

	BOOLean m_valid_;
} LogicSprite;

typedef struct _LogicStep {
	void (*Update)(int t, struct _LogicStep* _this);

	char m_stepName_[32];
} LogicStep;

// �߼����¡�
void LogicUpdate(int t);
void UnloadLogicEngine();

LogicSprite* CreateLogicSprite(void* me, void(*update)(int t, LogicSprite*), int x, int y, int w, int h, void(*render)(LogicSprite* ls), IMAGE* im,
	IMAGE* msk = NULL);
LogicStep* CreateLogicStep(char stepname[], void(*update)(int t, LogicStep* _this));

// ��ʼ�������߼�������Դ��
void LoadLogicSpriteStepResource();

// �߼������ڱ���ִ��ʱ�Ƿ���Ϊ�Ѿ��޸Ķ��޷�����ִ�С�
extern BOOLean g_stepInvalid_;

// ȫ���߼�����ļ��ϡ�
extern Container* g_logicSpriteManager_;
extern Container* g_topLogicSpriteManager_;
// ȫ���߼�����ļ��ϡ�
extern Container* g_logicStepManager_;

// LogicStep��Դ��
extern LogicStep* g_stepCheckSpriteValid_;