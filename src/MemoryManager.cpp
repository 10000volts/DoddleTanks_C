#include "MemoryManager.h"
#include "Container.h"

// 扩容并复制。p: 准备进行扩容的指针。 siz: 需复制的元素个数。 elesiz: 每个元素占用空间的大小。 exsiz: 扩容后的空间能容纳的元素数。
void* ExtendAndCopy(void* p, size_t siz, size_t elesiz, size_t exsiz) {
	void* r = malloc(exsiz * elesiz);
	memset(r, 0, exsiz * elesiz);
	if (p != NULL) {
		memcpy(r, p, siz * elesiz);
	}
	free(p);
	return r;
}

Container* g_extraContainer = CreateContainer(sizeof(void*), TRUE);