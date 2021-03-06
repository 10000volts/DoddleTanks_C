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

// 初始化加载逻辑步骤资源。
void LoadLogicControlStepResource();

void AddButton(Button* b);
void RemoveButton(Button* b);
void ClearButtons();

Button* CreateButton(LogicSprite* ls, void(*oc)(Button*), void(*of)(Button*), void(*ol)(Button*), 
	IMAGE* defimage, IMAGE* defmask = NULL, IMAGE* fcsimage = NULL, IMAGE* fcsmask = NULL);

// 目前拥有焦点的按钮。
extern Button* g_focusButton_;
// 全部按钮的集合，本身为其中首元素的地址。仿照vector的机制进行内存管理。
extern Button* g_buttonManager_;
extern int buttonCount_;
// 按钮的容量。
extern int buttonReserve_;

// LogicStep资源。
extern LogicStep* g_stepCheckFocus_;

void ButtonFocusDefault(Button* tis);
void ButtonLeaveDefault(Button* tis);