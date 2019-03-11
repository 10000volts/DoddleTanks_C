#include "TankAction.h"
#include "Tank.h"
#include "RandomEngine.h"
#include <math.h>
#include "Utility.h"

void TankJunkAI(Tank * t)
{
	int d = NextRand();
	if (d < 10) {
		t->m_move_right_ = TRUE;
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_RIGHT;
	}
	else if (d < 20) {
		t->m_move_up_ = TRUE;
		t->m_move_right_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_UP;
	}
	else if (d < 30) {
		t->m_move_left_ = TRUE;
		t->m_move_up_ = FALSE;
		t->m_move_right_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_LEFT;
	}
	else if (d < 40) {
		t->m_move_down_ = TRUE;
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_right_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_DOWN;
	}

	t->m_shoot_ = TRUE;
}

void TankFiveAI(Tank * t)
{
	int d = NextRand();
	if (d < 7) {
		t->m_move_right_ = TRUE;
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_RIGHT;
	}
	else if (d < 14) {
		t->m_move_up_ = TRUE;
		t->m_move_right_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_UP;
	}
	else if (d < 21) {
		t->m_move_left_ = TRUE;
		t->m_move_up_ = FALSE;
		t->m_move_right_ = FALSE;
		t->m_move_down_ = FALSE;
		t->m_shoot_angle_ = V6_DRT_LEFT;
	}
	else if (d < 28) {
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_right_ = FALSE;
		t->m_move_down_ = TRUE;
		t->m_shoot_angle_ = V6_DRT_DOWN;
	}
	else if (d < 35) {
		t->m_move_down_ = FALSE;
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_right_ = FALSE;
	}

	if (t->m_super_->m_y_ > V6_GAMEFIELD_HEIGHT / 2) t->m_move_down_ = FALSE;
	t->m_shoot_ = TRUE;
}

void TankQuickAI(Tank * t)
{

}

inline BOOLean AlignToBarrierList(Tank* t, int xnew, int ynew, Container* bl) {
	int i;
	Tank* tk;
	for (i = 0; i < bl->m_count_; ++i) {
		if (t == bl->m_me_[i]) continue;
		tk = (Tank*)bl->m_me_[i];
		if (CollisionDetection(
			xnew + t->m_collideL, ynew + t->m_collideT, t->m_collideW, t->m_collideH,
			tk->m_super_->m_x_ + tk->m_collideL, tk->m_super_->m_y_ + tk->m_collideT, tk->m_collideW, tk->m_collideH))
			return FALSE;
	}
	return TRUE;
}
BOOLean CheckPrismTurn(Tank* t, int xnew, int ynew) {
	if (xnew + t->m_collideL + t->m_collideW > V6_GAMEFIELD_WIDTH | xnew + t->m_collideL < V6_GAMEFIELD_LEFT |
		ynew + t->m_collideT + t->m_collideH > V6_GAMEFIELD_HEIGHT | ynew + t->m_collideT < V6_GAMEFIELD_TOP) {
		return TRUE;
	}
	if (!AlignToBarrierList(t, xnew, ynew, m_playerTankList_))
		return TRUE;
	if (!AlignToBarrierList(t, xnew, ynew, m_enemyTankList_)) 
		return TRUE;
	if (!AlignToBarrierList(t, xnew, ynew, m_neutralTankList_)) 
		return TRUE;
	return FALSE;
}
void TankPrismAI(Tank * tis)
{
	LogicSprite* bc = (LogicSprite*)tis->m_super_;
	double x = bc->m_x_, y = bc->m_y_;
	int d = (int)(tis->m_orientation_ / V6_DRT_UP);
	double dx = tis->m_data_.m_speed_ * V6_UPDATE_MAX_INTERVAL / 1000;
	int i;

	for (i = 0; i < 4; ++i) {
		switch (d) {
		case DRT_RIGHT:
			if (CheckPrismTurn(tis, x + dx, y)) {
				d = NextRand() % 4;
				continue;
			}
			else {
				tis->m_move_right_ = TRUE;
				tis->m_move_up_ = FALSE;
				tis->m_move_left_ = FALSE;
				tis->m_move_down_ = FALSE;
				goto lbl_move_comp;
			}
		case DRT_UP:
			if (CheckPrismTurn(tis, x, y - dx)) {
				d = NextRand() % 4;
				continue;
			}
			else {
				tis->m_move_right_ = FALSE;
				tis->m_move_up_ = TRUE;
				tis->m_move_left_ = FALSE;
				tis->m_move_down_ = FALSE;
				goto lbl_move_comp;
			}
		case DRT_LEFT:
			if (CheckPrismTurn(tis, x - dx, y)) {
				d = NextRand() % 4;
				continue;
			}
			else {
				tis->m_move_right_ = FALSE;
				tis->m_move_up_ = FALSE;
				tis->m_move_left_ = TRUE;
				tis->m_move_down_ = FALSE;
				goto lbl_move_comp;
			}
		case DRT_DOWN:
			if (tis->m_super_->m_y_ >= V6_GAMEFIELD_HEIGHT / 2 || CheckPrismTurn(tis, x, y + dx)) {
				d = NextRand() % 4;
				continue;
			}
			else {
				tis->m_move_right_ = FALSE;
				tis->m_move_up_ = FALSE;
				tis->m_move_left_ = FALSE;
				tis->m_move_down_ = TRUE;
				goto lbl_move_comp;
			}
		}
	}

lbl_move_comp:
	tis->m_shoot_ = TRUE;
}

void TankAttackAI(Tank * t)
{
}

void TankJunkAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
		double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
	}
}

void TankBigAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double s = tis->m_data_.m_bulletSpeed_ + (double)(NextRand() % 60 - 30);
		double sx = s * cos(tis->m_shoot_angle_);
		double sy = -s * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateBigBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
	}
}

void TankPrismAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double sa = tis->m_shoot_angle_;
		int i;
		for (i = 0; i < 3; ++i) {
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			sa += V6_PI * 2 / 3;
		}
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
		tis->m_shoot_angle_ += 0.2;
	}
}

void TankFiveAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double sa;
		int i;
		int stx = tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			sty = tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2;
		tis->m_shoot_angle_ = CalAngle(stx, sty, 
			m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH);
		for (i = -2; i != 3; ++i) {
			sa = tis->m_shoot_angle_ + (double)i * V6_PI / 9;
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateSmallBullet(tis, stx, sty,
				tis->m_data_.m_atk_, FALSE, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
		}
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
	}
}

void TankQuickAct(int t, Tank * tis)
{
}

void TankSunAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double sa = double(NextRand()) * V6_PI / 1023;
		int i;
		for (i = 0; i < 8; ++i) {
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateBigBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, TRUE, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			sa += V6_PI / 4;
		}
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
	}
}

void TankLanlingkingAct(int t, Tank * tis)
{
}

void NormalMoveAct(int t, Tank* tis) {
	LogicSprite* bc = (LogicSprite*)tis->m_super_;
	TankData td = tis->m_data_;
	double dx = td.m_speed_ * t / 1000;
	if (tis->m_move_right_) {
		AlignToBarriers(tis, bc->m_x_ + dx, bc->m_y_);
		ChangeTankOrientation(tis, V6_DRT_RIGHT);
	}
	else if (tis->m_move_up_) {
		AlignToBarriers(tis, bc->m_x_, bc->m_y_ - dx);
		ChangeTankOrientation(tis, V6_DRT_UP);
	}
	else if (tis->m_move_left_) {
		AlignToBarriers(tis, bc->m_x_ - dx, bc->m_y_);
		ChangeTankOrientation(tis, V6_DRT_LEFT);
	}
	else if (tis->m_move_down_) {
		AlignToBarriers(tis, bc->m_x_, bc->m_y_ + dx);
		ChangeTankOrientation(tis, V6_DRT_DOWN);
	}
}