#include <tchar.h>
#include "RankingList.h"
#include "SaveAndLoad.h"

ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];

void ReadHighScore() {
	LoadData(g_top_scores_, "assets\\score.dat");
}

// 整体数据会向16字节使用'@'对齐，只允许输入'A'~'Z', '0'~'9'。不过V6_MAX_TOP_SCORES本身即为16的倍数所以不用2333
BOOLean SaveScore(const TCHAR name[], int sc) {
	if(wcscmp(name, _T(""))) return FALSE;
	int i;
	for (i = 0; i < V6_MAX_TOP_SCORES; ++i) {
		if (wcscmp(g_top_scores_[i].name, name) == 0) {
			if (sc > g_top_scores_[i].score) {
				memcpy(&g_top_scores_[i], &g_top_scores_[i + 1], sizeof(ScoreRecord) * (V6_MAX_TOP_SCORES - i - 1));
				break;
			}
			return FALSE;
		}
	}
	for (i = 0; i < V6_MAX_TOP_SCORES; ++i) {
		if (g_top_scores_[i].score < sc) {
			if (i != V6_MAX_TOP_SCORES - 1) {
				memcpy(&g_top_scores_[i + 1], &g_top_scores_[i], sizeof(ScoreRecord) * (V6_MAX_TOP_SCORES - i - 1));
				wcscpy(g_top_scores_[i].name, name);
				g_top_scores_[i].score = sc;
				SaveData(g_top_scores_, "assets\\score.dat", sizeof(ScoreRecord) * V6_MAX_TOP_SCORES);
				return TRUE;
			}
			else {
				wcscpy(g_top_scores_[V6_MAX_TOP_SCORES - 1].name, name);
				g_top_scores_[V6_MAX_TOP_SCORES - 1].score = sc;
				SaveData(g_top_scores_, "assets\\score.dat", sizeof(ScoreRecord) * V6_MAX_TOP_SCORES);
				return TRUE;
			}
		}
	}

	SaveData(g_top_scores_, "assets\\score.dat", sizeof(ScoreRecord) * V6_MAX_TOP_SCORES);
	return FALSE;
}