#pragma once

#define V6_RAND_MAX		1023

// ��ʼ���������������
void InitializeRandomEngine();

// ������һ���������(��0~V6_RAND_MAX��Χ�ڡ�)
int NextRand();