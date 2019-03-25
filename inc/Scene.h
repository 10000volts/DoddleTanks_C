#pragma once

struct _LogicStep;
typedef struct _LogicStep LogicStep;

typedef enum _Scene {
	SCENE_MAIN, SCENE_HELP, SCENE_RANKING, SCENE_GAME, SCENE_DIFFICULTY_CHOOSE, SCENE_WAVE_CHOOSE, SCENE_GAMEOVER,
	SCENE_STAGECLEAR, SCENE_SAVESCORE
} Scene;

// º”‘ÿ≥°æ∞°£
void LoadScene(Scene sc);

void LoadSceneStepResource();