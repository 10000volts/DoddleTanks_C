#include <stdlib.h>
#include <string.h>
#include "LogicControl.h"
#include "MemoryManager.h"

void LogicUpdate()
{
	int i;

	UpdateStates();
	for (i = 0; i < logicStepCount_; ++i) {
		m_logicStepManager_[i]->Update(m_logicStepManager_[i]);
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
		m_logicSpriteManager_ = (LogicSprite*)ExtendAndCopy(m_logicSpriteManager_, logicSpriteCount_, sizeof(LogicSprite), logicSpriteReserve_);
	}
	m_logicSpriteManager_[logicSpriteCount_] = *ls;
	++logicSpriteCount_;
}

void AddLogicStep(LogicStep * ls)
{
	if (logicStepCount_ >= logicStepReserve_) {
		logicStepReserve_ *= 2;
		++logicStepReserve_;
		m_logicStepManager_ = (LogicStep**)ExtendAndCopy(m_logicStepManager_, logicStepCount_, sizeof(LogicStep*), logicStepReserve_);
	}
	m_logicStepManager_[logicStepCount_] = ls;
	++logicStepCount_;
}

void RemoveLogicSprite(LogicSprite * ls)
{
	int i;
	for (i = 0; i < logicSpriteCount_; ++i) {
		if (m_logicSpriteManager_ + i == ls) {
			if (i != logicSpriteCount_ - 1) memcpy(m_logicSpriteManager_ + i, m_logicSpriteManager_ + i + 1, logicSpriteCount_ - i - 1);
			--logicSpriteCount_;
		}
	}
}

void RemoveLogicStep(LogicStep * ls)
{
	int i;
	for (i = 0; i < logicStepCount_; ++i) {
		if (*(m_logicStepManager_ + i) == ls) {
			if(i != logicStepCount_ - 1) memcpy(m_logicStepManager_ + i, m_logicStepManager_ + i + 1, logicStepCount_ - i - 1);
			--logicStepCount_;
		}
	}
}

void ClearLogicSprites()
{
	free(m_logicSpriteManager_);
	logicSpriteCount_ = 0;
	logicSpriteReserve_ = 0;
	m_logicSpriteManager_ = NULL;
}

void ClearLogicSteps()
{
	free(m_logicStepManager_);
	logicStepCount_ = 0;
	logicStepReserve_ = 0;
	m_logicStepManager_ = NULL;
}

LogicSprite * CreateLogicSprite(void * me, void(*update)(void *, int), int x, int y, int w, int h, IMAGE* im, IMAGE* msk)
{
	LogicSprite* r = (LogicSprite*)malloc(sizeof(LogicSprite));
	memset(r, 0, sizeof(LogicSprite));
	r->m_me_ = me;
	r->Update = update;
	r->m_x_ = x;
	r->m_y_ = y;
	r->m_w_ = w;
	r->m_h_ = h;
	r->m_body_ = CreateRenderSprite(im, msk);
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

LogicSprite* m_logicSpriteManager_ = NULL;
LogicStep** m_logicStepManager_ = NULL;
int logicSpriteCount_ = 0;
int logicSpriteReserve_ = 0;
int logicStepCount_ = 0;
int logicStepReserve_ = 0;

LogicStep* m_stepCheckFocus_ = NULL;