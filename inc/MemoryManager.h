#pragma once

#include <string.h>
#include <stdlib.h>

struct _Container;
typedef _Container Container;

// ���ݲ����ơ�p: ׼���������ݵ�ָ�롣 siz: �踴�Ƶ�Ԫ�ظ����� elesiz: ÿ��Ԫ��ռ�ÿռ�Ĵ�С�� exsiz: ���ݺ�Ŀռ������ɵ�Ԫ������
void* ExtendAndCopy(void* p, size_t siz, size_t elesiz, size_t exsiz);