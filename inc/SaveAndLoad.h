#pragma once

#include <Windows.h>

// 将文件中的内容解密后读入。文件长度不能超过200016字节。
void LoadData(void* buf, const char filename[]);

// 将给定的内容加密后写入文件。siz必须是16的整数倍。
void SaveData(void* buf, const char filename[], size_t siz);