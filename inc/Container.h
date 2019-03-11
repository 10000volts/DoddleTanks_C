#pragma once

#include "stdafx.h"
#include "MemoryManager.h"

typedef struct _Container {
	// 指向存储元素指针的容器本体的指针。
	void** m_me_;
	// 容器中存放的元素数目。不要修改。
	int m_count_;
	// 容器中可容纳的元素数目。不要修改。
	int m_reserve_;
	// 单个元素占用的空间。不要修改。
	int m_siz_;
	// 是否独占资源。
	BOOLean m_possess_;
	// 发生删除的回调函数。
	void (*RemoveHappened)();
} Container;

Container* CreateContainer(int siz, BOOLean possess, void (*removeHappened)() = NULL);

void AddElement(Container* c, void* e);
void RemoveElement(Container* c, void* e);
void ClearContainer(Container* c);