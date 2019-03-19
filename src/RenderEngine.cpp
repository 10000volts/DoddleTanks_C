#include "LogicControl.h"

void InitializeRenderEnv()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWeight = 700;
	_tcscpy(f.lfFaceName, _T("свт╡"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
}

RenderSprite * CreateRenderSprite(IMAGE * image, IMAGE * mask, void(*render)(LogicSprite* e))
{
	RenderSprite* r = (RenderSprite*)malloc(sizeof(RenderSprite));
	memset(r, 0, sizeof(RenderSprite));
	r->m_image_ = image;
	r->m_src_image_ = image;
	r->m_mask_ = mask;
	r->m_src_mask_ = mask;
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
static LogicSprite* tls;
void RenderUpdate()
{
	int i;

	BeginBatchDraw();
	cleardevice();
	for (i = 0; i < g_logicSpriteManager_->m_count_; ++i) {
		tls = (LogicSprite *)g_logicSpriteManager_->m_me_[i];
		tls->m_body_->Render(tls);
	}
	EndBatchDraw();
}


void RenderSimple(LogicSprite* ls) {
	putimage(ls->m_x_, ls->m_y_, ls->m_body_->m_image_);
}

void RenderWithScaling(LogicSprite * ls)
{
	putimage(ls->m_x_, ls->m_y_, ls->m_w_, ls->m_h_, ls->m_body_->m_mask_, 0, 0, NOTSRCERASE);
	putimage(ls->m_x_, ls->m_y_, ls->m_w_, ls->m_h_, ls->m_body_->m_image_, 0, 0, SRCINVERT);
}

void RenderWithMask(LogicSprite * ls)
{
	putimage(ls->m_x_, ls->m_y_, ls->m_body_->m_mask_, NOTSRCERASE);
	putimage(ls->m_x_, ls->m_y_, ls->m_body_->m_image_, SRCINVERT);
}

void RenderWithRotation(LogicSprite* ls) {
	rotateimage(&t, ls->m_body_->m_mask_, ls->m_angle_, BLACK);
	putimage(ls->m_x_, ls->m_y_, &t, NOTSRCERASE);
	rotateimage(&t, ls->m_body_->m_image_, ls->m_angle_, WHITE);
	putimage(ls->m_x_, ls->m_y_, &t, SRCINVERT);
}