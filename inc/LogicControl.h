#pragma once

#include "LogicEngine.h"

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
extern Button* m_focusButton_;
// ȫ����ť�ļ��ϣ�����Ϊ������Ԫ�صĵ�ַ������vector�Ļ��ƽ����ڴ����
extern Button* m_buttonManager_;
extern int buttonCount_;
// ��ť��������
extern int buttonReserve_;

// LogicStep��Դ��
extern LogicStep* m_stepCheckFocus_;

inline void ButtonFocusDefault(Button* tis) {
	tis->ls_->m_body_->m_image_ = tis->m_body_->m_fcs_image_;
	tis->ls_->m_body_->m_mask_ = tis->m_body_->m_fcs_mask_;
}
inline void ButtonLeaveDefault(Button* tis) {
	tis->ls_->m_body_->m_image_ = tis->m_body_->m_def_image_;
	tis->ls_->m_body_->m_mask_ = tis->m_body_->m_def_mask_;
}