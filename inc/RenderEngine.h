#pragma once

#include <easyx.h>
typedef struct _RenderSprite {
	IMAGE* m_image_;
	// ����ͼƬ��
	IMAGE* m_mask_;
} RenderSprite;

typedef struct _RenderButton {
	// Ĭ��ͼƬ��
	IMAGE* m_def_image_;
	// Ĭ������ͼƬ��
	IMAGE* m_def_mask_;
	// ����λ�ڸð�ť��ʱ���ֵ�ͼƬ��
	IMAGE* m_fcs_image_;
	// ����λ�ڸð�ť��ʱ���ֵ�����ͼƬ��
	IMAGE* m_fcs_mask_;
} RenderButton;

RenderSprite* CreateRenderSprite(IMAGE* image, IMAGE* mask);
RenderButton* CreateRenderButton(IMAGE* defimage, IMAGE* defmask, IMAGE* fcsimage, IMAGE* fcsmask);

void RenderUpdate();