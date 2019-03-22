#include "Game.h"
#include "TankAction.h"
#include "Tank.h"
#include "LogicEngine.h"
#include "RenderEngine.h"
#include "ResourceManager.h"
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

void TankLanlingkingAI(Tank * t)
{
	LanlingkingTank* ex = (LanlingkingTank*)t->m_extra_;
	if (ex->m_contiousShootCountLeft_ > 0 | Distance2(t->m_super_->m_x_, t->m_super_->m_y_, m_playerTank_->m_super_->m_x_, m_playerTank_->m_super_->m_y_) <= 160000) {
		t->m_move_down_ = FALSE;
		t->m_move_up_ = FALSE;
		t->m_move_left_ = FALSE;
		t->m_move_right_ = FALSE;
		t->m_shoot_ = TRUE;
	}
	else {
		int d = NextRand();
		if (d < 10) {
			t->m_move_right_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 20) {
			t->m_move_up_ = TRUE;
			t->m_move_right_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 30) {
			t->m_move_left_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_right_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 45) {
			t->m_move_down_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_right_ = FALSE;
		}

		t->m_shoot_ = FALSE;
	}
}

void TankAttackAI(Tank * t)
{
	int d = NextRand();
	AttackTank* ex = (AttackTank*)t->m_extra_;
	if (d < 5) {
		ex->m_tracking_ = FALSE;
	}
	else if (d < 15) {
		ex->m_tracking_ = TRUE;
	}
	else if (d < 17) {
		ex->m_aim_ = m_playerTank_;
	}
	else if (d < 19) {
		ex->m_aim_ = m_stronghold_;
	}

	if (ex->m_tracking_) {
		BOOLean *xd = NULL, *rxd = NULL, *yd = NULL, *ryd = NULL;
		if (t->m_super_->m_x_ < ex->m_aim_->m_super_->m_x_ - V6_TANK_HALF_EDGE_LENGTH) {
			xd = &t->m_move_right_;
			rxd = &t->m_move_left_;
		}
		else if (t->m_super_->m_x_ > ex->m_aim_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH) {
			xd = &t->m_move_left_;
			rxd = &t->m_move_right_;
		}
		if (t->m_super_->m_y_ < ex->m_aim_->m_super_->m_y_ - V6_TANK_HALF_EDGE_LENGTH) {
			yd = &t->m_move_down_;
			ryd = &t->m_move_up_;
		}
		else if (t->m_super_->m_y_ > ex->m_aim_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH) {
			yd = &t->m_move_up_;
			ryd = &t->m_move_down_;
		}

		if (xd == NULL) {
			if (yd != NULL) {
				*yd = TRUE;
				*ryd = FALSE;
			}
			t->m_move_left_ = FALSE;
			t->m_move_right_ = FALSE;
		}
		else if (yd == NULL) {
			*xd = TRUE;
			*rxd = FALSE;
			t->m_move_up_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else {
			if (d > 511) {
				*xd = TRUE; 
				*rxd = FALSE;
				t->m_move_up_ = FALSE;
				t->m_move_down_ = FALSE;
			} else {
				*yd = TRUE;
				*ryd = FALSE;
				t->m_move_left_ = FALSE;
				t->m_move_right_ = FALSE;
			} 
		}
	}
	else {
		d = NextRand();
		if (d < 10) {
			t->m_move_right_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 20) {
			t->m_move_up_ = TRUE;
			t->m_move_right_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 30) {
			t->m_move_left_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_right_ = FALSE;
			t->m_move_down_ = FALSE;
		}
		else if (d < 40) {
			t->m_move_down_ = TRUE;
			t->m_move_up_ = FALSE;
			t->m_move_left_ = FALSE;
			t->m_move_right_ = FALSE;
		}
	}

	t->m_shoot_ = TRUE;
}

void TankJunkAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
		double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void TankBigAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double s = tis->m_data_.m_bulletSpeed_ + (double)(NextRand() % 60 - 30);
		double sx = s * cos(tis->m_shoot_angle_);
		double sy = -s * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateBigBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void TankPrismAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sa = tis->m_shoot_angle_;
		int i;
		for (i = 0; i < 3; ++i) {
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, TRUE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			sa += V6_PI * 2 / 3;
		}
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
		tis->m_shoot_angle_ += 20.0 * double(t) / 1000.0;
		tis->m_shoot_angle_ = ConvertIntoStandard(tis->m_shoot_angle_);
	}
}

void TankFiveAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sa;
		int i;
		int stx = tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			sty = tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2;
		tis->m_shoot_angle_ = CalAngle(stx, sty, 
			m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH);
		for (i = -2; i != 3; ++i) {
			sa = tis->m_shoot_angle_ + (double)i * V6_PI / 9.0;
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateSmallBullet(tis, stx, sty,
				tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
		}
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void TankQuickAct(int t, Tank * tis)
{
	double s = tis->m_data_.m_bulletSpeed_ + NextRand() % tis->m_data_.m_exdata_;
	double sa = tis->m_shoot_angle_ + (NextRand() % V6_RAND_MAX) * V6_DRT_RIGHTUP - V6_DRT_RIGHTUP / 2;

	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sx = s * cos(sa);
		double sy = -s * sin(sa);
		int big = NextRand() % 5;
		LogicSprite* ls;
		if (big == 0) {
			ls = CreateBigBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		}
		else {
			ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_ * 3 / 2, TRUE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		}
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void TankSunAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sa = double(NextRand()) * V6_PI / V6_RAND_MAX_F;
		int i;
		for (i = 0; i < 8; ++i) {
			double sx = tis->m_data_.m_bulletSpeed_ * cos(sa);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(sa);
			LogicSprite* ls = CreateBigBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_BIGBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, TRUE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			sa += V6_PI / 4;
		}
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void TankLanlingkingAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		LanlingkingTank* ex = (LanlingkingTank*)tis->m_extra_;
		if (ex->m_contiousShootCountLeft_) {
			double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
			LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				tis->m_data_.m_atk_, TRUE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			--ex->m_contiousShootCountLeft_;
			if (ex->m_contiousShootCountLeft_ != 0)
				tis->m_shootCD_ += 75;
			else 
				tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
		}
		else {
			int stx = tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
				sty = tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2;
			tis->m_shoot_angle_ = CalAngle(stx, sty,
				m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH);
			double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
			double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
			LogicSprite* ls = CreateSmallBullet(tis, stx, sty,
				tis->m_data_.m_atk_, TRUE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
			AddElement(g_logicSpriteManager_, ls);
			AddElement(m_enemyBulletList_, ls->m_me_);
			tis->m_shootCD_ += 75;
			ex->m_contiousShootCountLeft_ = 4;
		}
	}
}

void TankAttackAct(int t, Tank * tis)
{
	NormalMoveAct(t, tis);

	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		int stx = tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			sty = tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2;
		double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
		double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_); 
		LogicSprite* ls;

		tis->m_shoot_angle_ = CalAngle(stx, sty,
			m_playerTank_->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH, m_playerTank_->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH);

		ls = CreateSmallBullet(tis, stx, sty,
			tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
	}
}

void NormalMoveAct(int t, Tank* tis) {
	LogicSprite* bc = (LogicSprite*)tis->m_super_;
	TankData td = tis->m_data_;
	double dx = td.m_speed_ * double(t) / 1000.0;
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