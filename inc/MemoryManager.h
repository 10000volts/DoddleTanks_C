#pragma once

#include <string.h>
#include <stdlib.h>

struct _Container;
typedef _Container Container;

// 扩容并复制。p: 准备进行扩容的指针。 siz: 需复制的元素个数。 elesiz: 每个元素占用空间的大小。 exsiz: 扩容后的空间能容纳的元素数。
void* ExtendAndCopy(void* p, size_t siz, size_t elesiz, size_t exsiz);

extern Container* g_extraContainer;