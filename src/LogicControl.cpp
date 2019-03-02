#include "LogicControl.h"
#include "MemoryManager.h"
#include "Utility.h"

void StepCheckFocus(LogicStep*);

void LoadLogicControlStepResource()
{
	m_stepCheckFocus_ = (LogicStep*)malloc(sizeof(LogicStep));
	memset(m_stepCheckFocus_, 0, sizeof(LogicStep));
	strcpy(m_stepCheckFocus_->m_stepName_, "CheckFocus");
	m_stepCheckFocus_->Update = StepCheckFocus;
}

void AddButton(Button * b)
{
	if (buttonCount_ >= buttonReserve_) {
		buttonReserve_ *= 2;
		++buttonReserve_;
		m_buttonManager_ = (Button*)ExtendAndCopy(m_buttonManager_, buttonCount_, sizeof(Button), buttonReserve_);
	}
	m_buttonManager_[buttonCount_] = *b;
	++buttonCount_;
}

void RemoveButton(Button * ls)
{
	int i;
	for (i = 0; i < buttonCount_; ++i) {
		if (m_buttonManager_ + i == ls) {
			if(i != buttonCount_ - 1) memcpy(m_buttonManager_ + i, m_buttonManager_ + i + 1, buttonCount_ - i - 1);
			--buttonCount_;
		}
	}
}

void ClearButtons()
{
	m_focusButton_ = NULL;
	free(m_buttonManager_);
	buttonCount_ = 0;
	buttonReserve_ = 0;
	m_buttonManager_ = NULL;
}

Button * CreateButton(LogicSprite * ls, void(*oc)(Button*), void(*of)(Button*), void(*ol)(Button*),
	IMAGE* defimage, IMAGE* defmask, IMAGE* fcsimage, IMAGE* fcsmask)
{
	Button* r = (Button*)malloc(sizeof(Button));
	memset(r, 0, sizeof(Button));
	r->ls_ = ls;
	r->OnClick = oc;
	r->OnFocus = of;
	r->OnLeave = ol;
	r->m_body_ = CreateRenderButton(defimage, defmask, fcsimage, fcsmask);
	return r;
}

Button* m_focusButton_ = NULL;
Button* m_buttonManager_ = NULL;
int buttonCount_ = 0;
// 按钮的容量。
int buttonReserve_ = 0;

// 逻辑步骤：检查焦点状态。
void StepCheckFocus(LogicStep* tis) {
	int i;
	LONG mx = g_mouseState_.x, my = g_mouseState_.y;
	LogicSprite* ls;
	for (i = 0; i < buttonCount_; ++i) {
		ls = m_buttonManager_[i].ls_;
		if (IsIn(mx, my, ls->m_x_, ls->m_y_, ls->m_w_, ls->m_h_)) {
			if (m_focusButton_ != NULL) {
				if (m_focusButton_ == &m_buttonManager_[i]) goto onclick;
				if (m_focusButton_->OnLeave != NULL) m_focusButton_->OnLeave(m_focusButton_);
			}
			m_focusButton_ = &m_buttonManager_[i];
			if(m_focusButton_->OnFocus != NULL) m_focusButton_->OnFocus(m_focusButton_);
		onclick:
			m_focusButton_ = &m_buttonManager_[i];
			if (g_mouseState_.left_down) {
				m_focusButton_->OnClick(m_focusButton_);
			}
			return;
		}
	}
	if (m_focusButton_ != NULL) {
		if (m_focusButton_->OnLeave != NULL) m_focusButton_->OnLeave(m_focusButton_);
	}
	m_focusButton_ = NULL;
}