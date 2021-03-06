#pragma once

#define V6_RAND_MAX		1023
#define V6_RAND_MAX_F	1023.0

// 初始化随机数生成器。
void InitializeRandomEngine();

// 生成下一个随机数。(在0~V6_RAND_MAX范围内。)
int NextRand();