#pragma once

#include "Game.h"

// �������������������߰�������·�������Ϣʽ��λ��
void TankJunkAI(Tank* t);
// ɢ��ʽ��λ��
void TankFiveAI(Tank* t);
// ��������λ��(�������ƶ��ĸ��ʸ���)
void TankQuickAI(Tank* t);
// ����ʽ��λ��
void TankPrismAI(Tank* t);
// ����ʽ��λ��
void TankAttackAI(Tank* t);

void TankJunkAct(int t, Tank* tis);
void TankBigAct(int t, Tank* tis);
void TankPrismAct(int t, Tank* tis);
void TankFiveAct(int t, Tank* tis);
void TankQuickAct(int t, Tank* tis);
void TankSunAct(int t, Tank* tis);
void TankLanlingkingAct(int t, Tank* tis);
void NormalMoveAct(int t, Tank* tis);