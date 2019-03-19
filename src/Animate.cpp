#include "Animate.h"
#include "LogicEngine.h"

static void RenderFadeOut(LogicSprite* ls);
void PlayFadeOutAnimation(int t, void(*func)()){
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderFadeOut, NULL);
}
void RenderFadeOut(LogicSprite* ls) {

}