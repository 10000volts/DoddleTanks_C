#include <stdlib.h>
#include <string.h>
#include "LogicControl.h"
#include "MemoryManager.h"

void LogicUpdate()
{
	int i;
	for (i = 0; i < logicStepCount_; ++i) {
		g_logicStepManager_[i]->Update(g_logicStepManager_[i]);
	}
	for (i = 0; i < logicSpriteCount_; ++i) {
		if(g_logicSpriteManager_[i].Update != NULL) g_logicSpriteManager_[i].Update(&g_logicSpriteManager_[i]);
	}
}

// ÒÆ³ýËùÓÐÂß¼­²½ÖèºÍÂß¼­¾«Áé¡£
void UnloadLogicEngine()
{
	ClearLogicSteps();
	ClearLogicSprites();
}

void AddLogicSprite(LogicSprite * ls)
{
	if (logicSpriteCount_ >= logicSpriteReserve_) {
		logicSpriteReserve_ *= 2;
		++logicSpriteReserve_;
		g_logicSpriteManager_ = (LogicSprite*)ExtendAndCopy(g_logicSpriteManager_, logicSpriteCount_, sizeof(LogicSprite), logicSpriteReserve_);
	}
	g_logicSpriteManager_[logicSpriteCount_] = *ls;
	++logicSpriteCount_;
}

void AddLogicStep(LogicStep * ls)
{
	if (logicStepCount_ >= logicStepReserve_) {
		logicStepReserve_ *= 2;
		++logicStepReserve_;
		g_logicStepManager_ = (LogicStep**)ExtendAndCopy(g_logicStepManager_, logicStepCount_, sizeof(LogicStep*), logicStepReserve_);
	}
	g_logicStepManager_[logicStepCount_] = ls;
	++logicStepCount_;
}

void RemoveLogicSprite(LogicSprite * ls)
{
	int i;
	for (i = 0; i < logicSpriteCount_; ++i) {
		if (g_logicSpriteManager_ + i == ls) {
			if (i != logicSpriteCount_ - 1) {
				memcpy(g_logicSpriteManager_ + i, g_logicSpriteManager_ + i + 1, logicSpriteCount_ - i - 1);
			}
			--logicSpriteCount_;
		}
	}
}

void RemoveLogicStep(LogicStep * ls)
{
	int i;
	for (i = 0; i < logicStepCount_; ++i) {
		if (*(g_logicStepManager_ + i) == ls) {
			if(i != logicStepCount_ - 1) memcpy(g_logicStepManager_ + i, g_logicStepManager_ + i + 1, logicStepCount_ - i - 1);
			--logicStepCount_;
		}
	}
}

void ClearLogicSprites()
{
	free(g_logicSpriteManager_);
	logicSpriteCount_ = 0;
	logicSpriteReserve_ = 0;
	g_logicSpriteManager_ = NULL;
}

void ClearLogicSteps()
{
	free(g_logicStepManager_);
	logicStepCount_ = 0;
	logicStepReserve_ = 0;
	g_logicStepManager_ = NULL;
}

LogicSprite * CreateLogicSprite(void * me, void(*update)(LogicSprite *), int x, int y, int w, int h, void(*render)(LogicSprite* ls), IMAGE* im, IMAGE* msk)
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

LogicStep* CreateLogicStep(char stepname[], void(*update)(LogicStep* _this))
{
	LogicStep* r = (LogicStep*)malloc(sizeof(LogicStep));
	memset(r, 0, sizeof(LogicStep));
	strcpy(r->m_stepName_, stepname);
	r->Update = update;
	return r;
}

BOOLean g_running_ = true;

LogicSprite* g_logicSpriteManager_ = NULL;
LogicStep** g_logicStepManager_ = NULL;
int logicSpriteCount_ = 0;
int logicSpriteReserve_ = 0;
int logicStepCount_ = 0;
int logicStepReserve_ = 0;

LogicStep* g_stepCheckFocus_ = NULL;