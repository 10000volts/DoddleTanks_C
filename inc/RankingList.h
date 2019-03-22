#pragma once

#define V6_MAX_TOP_SCORES	16

typedef struct _ScoreRecord {
	char name[16];
	int score;
} ScoreRecord;

// ���ļ��ж�ȡ��߷֡��ļ������Ϊ1024�ֽڡ�
void ReadHighScore();

void SaveScore(char name[], int sc);

extern ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];