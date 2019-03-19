#pragma once

// 播放淡出动画。经过时间t后结束动画并执行回调函数。
void PlayFadeOutAnimation(int t, void(*func)());