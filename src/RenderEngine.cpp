#include "LogicControl.h"

RenderSprite * CreateRenderSprite(IMAGE * image, IMAGE * mask)
{
	RenderSprite* r = (RenderSprite*)malloc(sizeof(RenderSprite));
	r->m_image_ = image;
	r->m_mask_ = mask;
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

void RenderUpdate()
{
	int i;
	IMAGE t;

	BeginBatchDraw();
	cleardevice();
	for (i = 0; i < logicSpriteCount_; ++i) {
		LogicSprite e = m_logicSpriteManager_[i];
		if (e.m_body_->m_mask_ != NULL) {
			rotateimage(&t, e.m_body_->m_mask_, e.m_angle_, BLACK, TRUE);
			putimage(e.m_x_, e.m_y_, &t, NOTSRCERASE);
			rotateimage(&t, e.m_body_->m_image_, e.m_angle_, WHITE, TRUE);
			putimage(e.m_x_, e.m_y_, &t, SRCINVERT);
		}
		else {
			putimage(e.m_x_, e.m_y_, e.m_body_->m_image_);
		}
	}
	EndBatchDraw();
}
