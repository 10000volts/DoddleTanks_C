#include "RankingList.h"
#include "SaveAndLoad.h"

ScoreRecord g_top_scores_[V6_MAX_TOP_SCORES];

void ReadHighScore() {
	LoadData(g_top_scores_, "assets\\score.dat");
}

// �������ݻ���16�ֽ�ʹ��'@'���룬ֻ��������'A'~'Z', '0'~'9'������V6_MAX_TOP_SCORES����Ϊ16�ı������Բ���2333
void SaveScore(char name[], int sc) {
	int i;
	for (i = 0; i < V6_MAX_TOP_SCORES; ++i) {
		if (g_top_scores_->score < sc) {
			if (i != V6_MAX_TOP_SCORES - 1) {
				memcpy(&g_top_scores_[i + 1], &g_top_scores_[i], sizeof(ScoreRecord) * (V6_MAX_TOP_SCORES - i - 1));
				strcpy(g_top_scores_[i].name, name);
				g_top_scores_[i].score = sc;
			}
			else {
				strcpy(g_top_scores_[V6_MAX_TOP_SCORES - 1].name, name);
				g_top_scores_[V6_MAX_TOP_SCORES - 1].score = sc;
			}
		}
	}

	SaveData(g_top_scores_, "assets\\score.dat", sizeof(ScoreRecord) * V6_MAX_TOP_SCORES);
}