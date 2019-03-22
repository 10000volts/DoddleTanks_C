#pragma once

#include "Bullet.h"

typedef struct _Tank {
	// ���ߡ�
	void (*Decide)(struct _Tank* tis);
	// ���ݾ����ж���
	void (*Act)(int t, struct _Tank* tis);
	// �����Ļص�������
	void (*Dead)(struct _Tank* tis);

	TANKSTYLE m_tankStyle_;
	// ����ʵ��Ķ�����Ϣ��
	void* m_extra_;
	// ����ָ�롣
	LogicSprite* m_super_;

	TankData m_data_;

	int m_HP;
	int m_shootCD_;

	// �ӵ���ײ�ж���������ꡣ(ԭʼֵ)
	double m_srcBulletCollideL, m_srcBulletCollideT, m_srcBulletCollideW, m_srcBulletCollideH;
	// ǽ����ײ�ж���������ꡣ(ԭʼֵ)
	double m_srcCollideL, m_srcCollideT, m_srcCollideW, m_srcCollideH;
	// �ӵ���ײ�ж���������ꡣ
	double m_bulletCollideL, m_bulletCollideT, m_bulletCollideW, m_bulletCollideH;
	// ǽ����ײ�ж���������ꡣ
	double m_collideL, m_collideT, m_collideW, m_collideH;
	// �ж�����֮�ı䡣����m_noOrientation_��ͬʱ����ΪFALSE��
	double m_orientation_;
	// �Ƿ�ʹ�ýǶȶ��ǳ�������ΪTRUEʱ�ı䳯�򲻻�ı��ж���
	BOOLean m_noOrientation_;

	// �Ƿ��������(���޺Ϸ����)��
	BOOLean m_move_right_, m_move_up_, m_move_left_, m_move_down_;
	// �Ƿ���������
	BOOLean m_shoot_;
	// ���ߵ�����Ƕȡ�
	double m_shoot_angle_;

	BOOLean m_valid_ = TRUE;
} Tank;

Tank* CreateTank(TANKSTYLE tankstyle, void* ex, LogicSprite* super, TankData td, 
	int bcl, int bct, int bcw, int bch,
	int cl, int ct, int cw, int ch,
	double ori, void(*decide)(Tank* tis) = NULL, void(*act)(int t, Tank* tis) = NULL, void(*dead)(Tank* tis) = NULL);

Tank* CreateStronghold(LogicSprite* ls);

Tank* CreatePlayerTank(LogicSprite* ls);
typedef struct _PlayerTank {
	// �Ƿ���١�
	BOOLean m_slow_;
	// �����Ƿ����ǽ��
	BOOLean m_setWall_;

	// ��ǰ����ǡ�(װ����)
	double m_orientation_angle_;
	// ת����ٶȡ�(װ����)
	double m_palstance_;

	// ʣ��ɷ��õ�ǽ��Ŀ��
	int m_wall_count_;
} PlayerTank;

Tank* CreateJunkTank(LogicSprite* ls);
Tank* CreateBigTank(LogicSprite* ls);
Tank* CreatePrismTank(LogicSprite* ls);
Tank* CreateFiveTank(LogicSprite* ls);
Tank* CreateQuickTank(LogicSprite* ls);
Tank* CreateSunTank(LogicSprite* ls);
Tank* CreateLanlingkingTank(LogicSprite* ls);
Tank* CreateAttackTank(LogicSprite* ls);

typedef struct _LanlingkingTank {
	// �������ʣ�������
	int m_contiousShootCountLeft_;
} LanlingkingTank;

typedef struct _AttackTank {
	// ׷�ٵ�Ŀ�ꡣ
	Tank* m_aim_;

	// �Ƿ�����׷��Ŀ�ꡣ
	BOOLean m_tracking_;
} AttackTank;


// ����̹�˵ĳ��򲢸���noOrientation�����ж��Ƿ�ݴ��޸���ײ�ж���
void ChangeTankOrientation(Tank* t, double ori);