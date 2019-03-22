#pragma once

// 加密至内存。size必须是16的整数，否则多余的部分将不会被处理。
void EncMem(void* buf, int size);
// 从内存中解密。size必须是16的整数，否则多余的部分将不会被处理。
void DecMem(void* buf, int size);
// 加密文件。size必须是16的整数，否则多余的部分将不会被处理。
int EncFile(const char* file);