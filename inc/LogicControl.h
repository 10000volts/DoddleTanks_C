#pragma once

#include "LogicEngine.h"

typedef struct _RenderButton RenderButton;

typedef struct _Button {
	LogicSprite* ls_;
	void(*OnClick)(struct _Button* _this);
	void(*OnFocus)(struct _Button* _this);
	void(*OnLeave)(struct _Button* _this);

	RenderButton* m_body_;
} Button;

// ��ʼ�������߼�������Դ��
void LoadLogicControlStepResource();

void AddButton(Button* b);
void RemoveButton(Button* b);
void ClearButtons();

Button* CreateButton(LogicSprite* ls, void(*oc)(Button*), void(*of)(Button*), void(*ol)(Button*), 
	IMAGE* defimage, IMAGE* defmask = NULL, IMAGE* fcsimage = NULL, IMAGE* fcsmask = NULL);

// Ŀǰӵ�н���İ�ť��
extern Button* g_focusButton_;
// ȫ����ť�ļ��ϣ�����Ϊ������Ԫ�صĵ�ַ������vector�Ļ��ƽ����ڴ����
extern Button* g_buttonManager_;
extern int buttonCount_;
// ��ť��������
extern int buttonReserve_;

// LogicStep��Դ��
extern LogicStep* g_stepCheckFocus_;

void ButtonFocusDefault(Button* tis);
void ButtonLeaveDefault(Button* tis);