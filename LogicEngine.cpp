#include <stdlib.h>
#include <string.h>
#include "LogicControl.h"
#include "MemoryManager.h"

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
}

// ÒÆ³ýËùÓÐÂß¼­²½ÖèºÍÂß¼­¾«Áé¡£
void UnloadLogicEngine()
{
	ClearContainer(g_logicSpriteManager_);
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

BOOLean g_stepInvalid_;

Container* g_logicSpriteManager_ = CreateContainer(sizeof(LogicSprite), TRUE);
static inline void LogicStepRemoved() {
	g_stepInvalid_ = TRUE;
}
Container* g_logicStepManager_ = CreateContainer(sizeof(LogicStep), FALSE, LogicStepRemoved);