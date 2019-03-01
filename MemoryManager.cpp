#include "MemoryManager.h"

// ���ݲ����ơ�p: ׼���������ݵ�ָ�롣 siz: �踴�Ƶ�Ԫ�ظ����� elesiz: ÿ��Ԫ��ռ�ÿռ�Ĵ�С�� exsiz: ���ݺ�Ŀռ������ɵ�Ԫ������
void* ExtendAndCopy(void* p, size_t siz, size_t elesiz, size_t exsiz) {
	void* r = malloc(exsiz * elesiz);
	memset(r, 0, exsiz * elesiz);
	if (p != NULL) {
		memcpy(r, p, siz * elesiz);
	}
	free(p);
	return r;
}