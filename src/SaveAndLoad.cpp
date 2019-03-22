#include "SaveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include "AES.h"

void LoadData(void * buf, const char filename[]){
	FILE* f = fopen(filename, "r");
	unsigned char str[1024];
	int i;
	for (i = 0; !feof(f); ++i) {
		fread(&str[i], sizeof(unsigned char), 1, f);
	}
	DecMem(str, i);
	memcpy(buf, str, i);
}

void SaveData(void * buf, const char filename[], size_t siz){
	FILE* f = fopen(filename, "w");
	void* t;
	memcpy(t, buf, siz);
	EncMem(buf, siz);
	fwrite(buf, sizeof(char), siz, f);
	fclose(f);
}
