#pragma once

#define V6_RAND_MAX		1023
#define V6_RAND_MAX_F	1023.0

// ��ʼ���������������
void InitializeRandomEngine();

// ������һ���������(��0~V6_RAND_MAX��Χ�ڡ�)
int NextRand();