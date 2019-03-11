#pragma once

#include <easyx.h>

struct _LogicSprite;
typedef _LogicSprite LogicSprite;

typedef struct _RenderSprite {
	// 不能作任何修改。
	IMAGE* m_image_;
	// 掩码图片。不能作任何修改。
	IMAGE* m_mask_;
	// 原图片。
	IMAGE* m_src_image_;
	// 原掩码图片。
	IMAGE* m_src_mask_;
	void (*Render)(LogicSprite* e);
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

// 初始化渲染环境。需要窗口句柄。
void InitializeRenderEnv();

RenderSprite* CreateRenderSprite(IMAGE* image, IMAGE* mask, void(*render)(LogicSprite* e));
RenderButton* CreateRenderButton(IMAGE* defimage, IMAGE* defmask, IMAGE* fcsimage, IMAGE* fcsmask);

void RenderUpdate();

// 只绘制原图像，覆盖原有像素。
void RenderSimple(LogicSprite* ls);
void RenderWithScaling(LogicSprite* ls);
// 带有透明的绘制。
void RenderWithMask(LogicSprite* ls);
// 带有透明、旋转的渲染。
void RenderWithRotation(LogicSprite* ls);