#pragma once

#include <Windows.h>

// ���ļ��е����ݽ��ܺ���롣�ļ����Ȳ��ܳ���1024�ֽڡ�
void LoadData(void* buf, const char filename[]);

// �����������ݼ��ܺ�д���ļ���siz������16����������
void SaveData(void* buf, const char filename[], size_t siz);