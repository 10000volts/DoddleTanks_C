#include <stdlib.h>
#include <string.h>
#include "LogicControl.h"
#include "RenderEngine.h"
#include "MemoryManager.h"
#include "InputEngine.h"

static void StepCheckSpriteValid(int t, LogicStep* ls);

static LogicStep* tls;
static LogicSprite* tlsp;
void LogicUpdate(int t)
{
	int i;
	for (i = 0; i < g_logicStepManager_->m_count_; ++i) {
		tls = (LogicStep*)g_logicStepManager_->m_me_[i];
		tls->Update(t, tls);
		if (g_stepInvalid_) {
			g_stepInvalid_ = FALSE;
			return;
		}
	}
	for (i = 0; i < g_logicSpriteManager_->m_count_; ++i) {
		tlsp = (LogicSprite*)g_logicSpriteManager_->m_me_[i];
		if(tlsp->Update != NULL) tlsp->Update(t, tlsp);
	}
	for (i = 0; i < g_topLogicSpriteManager_->m_count_; ++i) {
		tlsp = (LogicSprite*)g_topLogicSpriteManager_->m_me_[i];
		if (tlsp->Update != NULL) tlsp->Update(t, tlsp);
	}
}

// �Ƴ������߼�������߼����顣
void UnloadLogicEngine()
{
	int i;
	LogicSprite* ls;
	for (i = 0; i < g_logicSpriteManager_->m_count_; ++i) {
		ls = (LogicSprite*)g_logicSpriteManager_->m_me_[i];
		free(ls->m_body_);
	}
	for (i = 0; i < g_topLogicSpriteManager_->m_count_; ++i) {
		ls = (LogicSprite*)g_topLogicSpriteManager_->m_me_[i];
		free(ls->m_body_);
	}
	ClearContainer(g_logicSpriteManager_);
	ClearContainer(g_topLogicSpriteManager_);
	ClearContainer(g_logicStepManager_);
}

LogicSprite * CreateLogicSprite(void * me, void(*update)(int t, LogicSprite *), int x, int y, int w, int h, void(*render)(LogicSprite* ls), IMAGE* im, IMAGE* msk)
{
	LogicSprite* r = (LogicSprite*)malloc(sizeof(LogicSprite));
	memset(r, 0, sizeof(LogicSprite));
	r->m_me_ = me;
	r->Update = update;
	r->m_x_ = x;
	r->m_y_ = y;
	r->m_w_ = w;
	r->m_h_ = h;
	r->m_body_ = CreateRenderSprite(im, msk, render);
	r->m_valid_ = TRUE;
	return r;
}

LogicStep* CreateLogicStep(char stepname[], void(*update)(int t, LogicStep* _this))
{
	LogicStep* r = (LogicStep*)malloc(sizeof(LogicStep));
	memset(r, 0, sizeof(LogicStep));
	strcpy(r->m_stepName_, stepname);
	r->Update = update;
	return r;
}

void LoadLogicSpriteStepResource(){
	g_stepCheckSpriteValid_ = CreateLogicStep((char*)"CheckSpriteValid", StepCheckSpriteValid);
}

BOOLean g_stepInvalid_;

Container* g_logicSpriteManager_ = CreateContainer(sizeof(LogicSprite), TRUE);
Container* g_topLogicSpriteManager_ = CreateContainer(sizeof(LogicSprite), TRUE);

static inline void LogicStepRemoved() {
	g_stepInvalid_ = TRUE;
}
Container* g_logicStepManager_ = CreateContainer(sizeof(LogicStep), FALSE, LogicStepRemoved);

LogicStep* g_stepCheckSpriteValid_;
void StepCheckSpriteValid(int t, LogicStep * ls)
{
	int i;
	for (i = 0; i < g_logicSpriteManager_->m_count_; ++i) {
		tlsp = (LogicSprite*)g_logicSpriteManager_->m_me_[i];
		if (!tlsp->m_valid_) {
			RemoveElement(g_logicSpriteManager_, tlsp);
		}
	}
}
