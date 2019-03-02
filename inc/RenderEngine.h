#pragma once

#include <easyx.h>
typedef struct _RenderSprite {
	// 原本的图片。不能作任何修改。
	IMAGE* m_image_;
	// 原本的掩码图片。不能作任何修改。
	IMAGE* m_mask_;
} RenderSprite;

typedef struct _RenderButton {
	// 默认图片。
	IMAGE* m_def_image_;
	// 默认掩码图片。
	IMAGE* m_def_mask_;
	// 焦点位于该按钮上时呈现的图片。
	IMAGE* m_fcs_image_;
	// 焦点位于该按钮上时呈现的掩码图片。
	IMAGE* m_fcs_mask_;
} RenderButton;

RenderSprite* CreateRenderSprite(IMAGE* image, IMAGE* mask);
RenderButton* CreateRenderButton(IMAGE* defimage, IMAGE* defmask, IMAGE* fcsimage, IMAGE* fcsmask);

void RenderUpdate();