#pragma once

#include "stdafx.h"
#include "MemoryManager.h"

typedef struct _Container {
	// ָ��洢Ԫ��ָ������������ָ�롣
	void** m_me_;
	// �����д�ŵ�Ԫ����Ŀ����Ҫ�޸ġ�
	int m_count_;
	// �����п����ɵ�Ԫ����Ŀ����Ҫ�޸ġ�
	int m_reserve_;
	// ����Ԫ��ռ�õĿռ䡣��Ҫ�޸ġ�
	int m_siz_;
	// �Ƿ��ռ��Դ��
	BOOLean m_possess_;
	// ����ɾ���Ļص�������
	void (*RemoveHappened)();
} Container;

Container* CreateContainer(int siz, BOOLean possess, void (*removeHappened)() = NULL);

void AddElement(Container* c, void* e);
void RemoveElement(Container* c, void* e);
void ClearContainer(Container* c);