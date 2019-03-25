#pragma once

#include <Windows.h>
#include "stdafx.h"

#define V6_MAX_TOP_SCORES	16
#define V6_NAME_MAX_LENGTH	16

typedef struct _ScoreRecord {
	TCHAR name[V6_NAME_MAX_LENGTH];
	int score;
} ScoreRecord;

// 从文件中读取最高分。文件长度最长为1024字节。
void ReadHighScore();

// 返回存储是否成功。
BOOLean SaveScore(const TCHAR name[], int sc);

extern ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];