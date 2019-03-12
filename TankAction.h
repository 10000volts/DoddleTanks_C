#pragma once

#include "Game.h"

// 混沌宇宙无穷熵增乱七八糟毫无章法令人窒息式走位。
void TankJunkAI(Tank* t);
// 散步式走位。
void TankFiveAI(Tank* t);
// 倾向性走位。(朝敌人移动的概率更大)
void TankQuickAI(Tank* t);
// 环绕式走位。
void TankPrismAI(Tank* t);
// 刺客式混沌宇宙无穷熵增乱七八糟毫无章法令人窒息式走位。
void TankLanlingkingAI(Tank* t);
// 锁定式走位。
void TankAttackAI(Tank* t);

void TankJunkAct(int t, Tank* tis);
void TankBigAct(int t, Tank* tis);
void TankPrismAct(int t, Tank* tis);
void TankFiveAct(int t, Tank* tis);
void TankQuickAct(int t, Tank* tis);
void TankSunAct(int t, Tank* tis);
void TankLanlingkingAct(int t, Tank* tis);
void TankAttackAct(int t, Tank* tis);
void NormalMoveAct(int t, Tank* tis);