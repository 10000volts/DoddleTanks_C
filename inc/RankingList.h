#pragma once

#define V6_MAX_TOP_SCORES	16

typedef struct _ScoreRecord {
	char name[16];
	int score;
} ScoreRecord;

// 从文件中读取最高分。文件长度最长为1024字节。
void ReadHighScore();

void SaveScore(char name[], int sc);

extern ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];