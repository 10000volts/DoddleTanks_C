#pragma once

#include <Windows.h>
#include "stdafx.h"

#define V6_MAX_TOP_SCORES	16
#define V6_NAME_MAX_LENGTH	16

typedef struct _ScoreRecord {
	TCHAR name[V6_NAME_MAX_LENGTH];
	int score;
} ScoreRecord;

// ���ļ��ж�ȡ��߷֡��ļ������Ϊ1024�ֽڡ�
void ReadHighScore();

// ���ش洢�Ƿ�ɹ���
BOOLean SaveScore(const TCHAR name[], int sc);

extern ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];