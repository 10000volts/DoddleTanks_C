#pragma once

// �������ڴ档size������16���������������Ĳ��ֽ����ᱻ����
void EncMem(void* buf, int size);
// ���ڴ��н��ܡ�size������16���������������Ĳ��ֽ����ᱻ����
void DecMem(void* buf, int size);
// �����ļ���size������16���������������Ĳ��ֽ����ᱻ����
int EncFile(const char* file);