#include "LogicControl.h"

RenderSprite * CreateRenderSprite(IMAGE * image, IMAGE * mask, void(*render)(LogicSprite* e))
{
	RenderSprite* r = (RenderSprite*)malloc(sizeof(RenderSprite));
	r->m_image_ = image;
	r->m_mask_ = mask;
	r->Render = render;
	return r;
}

RenderButton * CreateRenderButton(IMAGE * defimage, IMAGE * defmask, IMAGE * fcsimage, IMAGE * fcsmask)
{
	RenderButton* r = (RenderButton*)malloc(sizeof(RenderButton));
	r->m_def_image_ = defimage;
	r->m_def_mask_ = defmask;
	r->m_fcs_image_ = fcsimage;
	r->m_fcs_mask_ = fcsmask;
	return r;
}

static IMAGE t;
void RenderUpdate()
{
	int i;

	BeginBatchDraw();
	cleardevice();
	for (i = 0; i < logicSpriteCount_; ++i) {
		g_logicSpriteManager_[i].m_body_->Render(&g_logicSpriteManager_[i]);
	}
	EndBatchDraw();
}


void RenderSimple(LogicSprite* ls) {
	putimage(ls->m_x_, ls->m_y_, ls->m_body_->m_image_);
}

void RenderWithRotation(LogicSprite* ls) {
	rotateimage(&t, ls->m_body_->m_mask_, ls->m_angle_, BLACK);
	putimage(ls->m_x_, ls->m_y_, &t, NOTSRCERASE);
	rotateimage(&t, ls->m_body_->m_image_, ls->m_angle_, WHITE);
	putimage(ls->m_x_, ls->m_y_, &t, SRCINVERT);
}

void RenderWithDirection(LogicSprite* ls) {
	putimage(ls->m_x_, ls->m_y_, &t, NOTSRCERASE);
	putimage(ls->m_x_, ls->m_y_, &t, SRCINVERT);
}