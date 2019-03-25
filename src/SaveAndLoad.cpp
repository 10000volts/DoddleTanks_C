#include "SaveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include "AES.h"

void LoadData(void * buf, const char filename[]){
	FILE* f = fopen(filename, "r");
	if (f != NULL) {
		unsigned char str[2048];
		memset(str, 0, sizeof(str));
		int i;
		for (i = 0; !feof(f); ++i) {
			fread(&str[i], sizeof(unsigned char), 1, f);
		}
		memcpy(buf, str, i - 1);
		fclose(f);
	}
}

void SaveData(void * buf, const char filename[], size_t siz){
	FILE* f = fopen(filename, "w");
	void* t = malloc(siz);
	memcpy(t, buf, siz);
	fwrite(t, sizeof(unsigned char), siz, f);
	fclose(f);
	free(t);
}
