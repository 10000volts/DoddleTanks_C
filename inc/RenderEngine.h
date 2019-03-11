#pragma once

#include <easyx.h>

struct _LogicSprite;
typedef _LogicSprite LogicSprite;

typedef struct _RenderSprite {
	// �������κ��޸ġ�
	IMAGE* m_image_;
	// ����ͼƬ���������κ��޸ġ�
	IMAGE* m_mask_;
	// ԭͼƬ��
	IMAGE* m_src_image_;
	// ԭ����ͼƬ��
	IMAGE* m_src_mask_;
	void (*Render)(LogicSprite* e);
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

// ��ʼ����Ⱦ��������Ҫ���ھ����
void InitializeRenderEnv();

RenderSprite* CreateRenderSprite(IMAGE* image, IMAGE* mask, void(*render)(LogicSprite* e));
RenderButton* CreateRenderButton(IMAGE* defimage, IMAGE* defmask, IMAGE* fcsimage, IMAGE* fcsmask);

void RenderUpdate();

// ֻ����ԭͼ�񣬸���ԭ�����ء�
void RenderSimple(LogicSprite* ls);
void RenderWithScaling(LogicSprite* ls);
// ����͸���Ļ��ơ�
void RenderWithMask(LogicSprite* ls);
// ����͸������ת����Ⱦ��
void RenderWithRotation(LogicSprite* ls);