#include "LogicControl.h"
#include "MemoryManager.h"
#include "Utility.h"

static void StepCheckFocus(int t, LogicStep*);

void LoadLogicControlStepResource()
{
	g_stepCheckFocus_ = CreateLogicStep((char*)"CheckFocus", StepCheckFocus);
}

void AddButton(Button * b)
{
	if (buttonCount_ >= buttonReserve_) {
		buttonReserve_ *= 2;
		++buttonReserve_;
		g_buttonManager_ = (Button*)ExtendAndCopy(g_buttonManager_, buttonCount_, sizeof(Button), buttonReserve_);
	}
	g_buttonManager_[buttonCount_] = *b;
	++buttonCount_;
}

void RemoveButton(Button * ls)
{
	int i;
	for (i = 0; i < buttonCount_; ++i) {
		if (g_buttonManager_ + i == ls) {
			if(i != buttonCount_ - 1) memcpy(g_buttonManager_ + i, g_buttonManager_ + i + 1, buttonCount_ - i - 1);
			if (g_focusButton_ == ls) g_focusButton_ = NULL;
			--buttonCount_;
		}
	}
}

void ClearButtons()
{
	g_focusButton_ = NULL;
	free(g_buttonManager_);
	buttonCount_ = 0;
	buttonReserve_ = 0;
	g_buttonManager_ = NULL;
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

Button* g_focusButton_ = NULL;
Button* g_buttonManager_ = NULL;
int buttonCount_ = 0;
// 按钮的容量。
int buttonReserve_ = 0;

LogicStep* g_stepCheckFocus_ = NULL;
// 逻辑步骤：检查焦点状态。
void StepCheckFocus(int t, LogicStep* tis) {
	int i;
	LONG mx = g_mouseState_.x, my = g_mouseState_.y;
	LogicSprite* ls;
	for (i = 0; i < buttonCount_; ++i) {
		ls = g_buttonManager_[i].ls_;
		if (IsIn(mx, my, ls->m_x_, ls->m_y_, ls->m_w_, ls->m_h_)) {
			if (g_focusButton_ != NULL) {
				if (g_focusButton_ == &g_buttonManager_[i]) goto onclick;
				if (g_focusButton_->OnLeave != NULL) g_focusButton_->OnLeave(g_focusButton_);
			}
			g_focusButton_ = &g_buttonManager_[i];
			if(g_focusButton_->OnFocus != NULL) g_focusButton_->OnFocus(g_focusButton_);
		onclick:
			g_focusButton_ = &g_buttonManager_[i];
			if (g_mouseState_.left_up) {
				g_focusButton_->OnClick(g_focusButton_);
			}
			return;
		}
	}
	if (g_focusButton_ != NULL) {
		if (g_focusButton_->OnLeave != NULL) g_focusButton_->OnLeave(g_focusButton_);
	}
	g_focusButton_ = NULL;
}