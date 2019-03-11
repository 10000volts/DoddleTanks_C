#include "Game.h"
#include "Scene.h"
#include "Tank.h"
#include "TankAction.h"
#include "ResourceManager.h"
#include "RandomEngine.h"
#include "Utility.h"

#define V6_GAME_TIME	1800000
#define V6_ADJUST_TIME	5000
typedef Tank Respawn;

static BOOLean m_valid_;
static void InitGameGUI();
static void InitializeGameField();

// 游戏是否暂停。
static BOOLean m_pause_;
// 下一波来临之前的调整时间。
static int m_adjustTime_;
// 距离下一次敌方坦克生成的时间。
static int m_generateCD_;

// 游戏主循环。
static void StepGameUpdate(int t, LogicStep* tis);
// 游戏事件判断。(比如进行下一波，出现新的怪物，游戏剩余时间更新)
static void StepCheckEvent(int t);
// 坦克决策。
static void StepTankStateUpdate(int t);
// 坦克决策。
static void StepTankDecide();
// 坦克行动。
static void StepTankAct(int t);
// 子弹行动。
static void StepBulletAct(int t);
// 碰撞判定。不会清除子弹和坦克。
static void StepCheckCollision();
// 清除死亡的坦克和子弹。
static void StepCheckValid();

// 在重生点的位置尝试生成新的敌人。
static void GenerateNewAt(Respawn* t);
// 没有作生成合法性判定。
static void SetWall(int x, int y);
// 没有作生成合法性判定。
static void SetIfWall(int x, int y);
// 没有作生成合法性判定。
// 设置敌方坦克重生点。
static void SetAnkhWall(int x, int y);
// 在指定位置生成指定坦克，没有作生成合法性判定。
static void SetNewTank(TANKSTYLE ts, int x, int y);
// 没有作生成合法性判定。
static void SetNewJunkTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewBigTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewPrismTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewFiveTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewQuickTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewSunTank(int x, int y);
// 没有作生成合法性判定。
static void SetNewLanlingkingTank(int x, int y);

// 判断在指定列表内的坦克元素是否与指定矩形有碰撞。
static BOOLean CheckPointLegal(int x, int y, int w, int h, Container* c);
// 判断子弹与坦克之间的碰撞并处理。
static void CollisionCheckWithLists(Container* bullets, Container* tanks);
// 判断子弹与中立坦克(墙体/据点)之间的碰撞并处理。
static void CollisionCheckWithWallList(Container* bullets, Container* tanks);
static void PlayerShootAct(Tank* tis);
static void PlayerInput(Tank* tis);
static void PlayerAct(int t, Tank* tis);
static void TankDeadFatal(Tank* t);

static void PlayerTankRotate(int t, LogicSprite* ls);
static void PlayerBatterySync(int t, LogicSprite* ls);
static void PlayerHPRender(int t, LogicSprite* ls);
static void RenderWithDirection(int t, LogicSprite* ls);
static void WallRender(int t, LogicSprite* ls);
static void RenderHiScore(LogicSprite* ls);
static void RenderScore(LogicSprite* ls);
static void RenderWallCount(LogicSprite* ls);
static void RenderWaveNum(LogicSprite* ls);
static void RenderTimeLeft(LogicSprite* ls);

static TankWave m_waveData_[V6_GAME_MAX_WAVE + 1];

// 当前最后被摧毁的敌方坦克的坐标，用于存储回合结束时触发事件的位置。
static int m_enemyLatestX, m_enemyLatestY;

void LoadGameStepResource()
{
	g_stepGameUpdate_ = CreateLogicStep((char*)"CheckEvent", StepGameUpdate);
}

void InitializeGame()
{
	if (!m_valid_) {
		int i;
		--m_wave_;
		m_adjustTime_ = V6_ADJUST_TIME;
		m_time_ = V6_GAME_TIME;
		m_score_ = 0;	
		memcpy(m_waveData_, g_waveData_[g_gameDifficulty_], sizeof(TankWave) * V6_GAME_MAX_WAVE);
		for (i = 1; i <= m_wave_; ++i) {
			if (m_waveData_[i].OnStart != NULL) m_waveData_[i].OnStart();
			memset(&(m_waveData_[i]), 0, sizeof(TankWave));
		}
		InitGameGUI();
		InitializeGameField();
		m_valid_ = TRUE;
	}

	AddElement(g_logicStepManager_, g_stepGameUpdate_);
}
void LoadMap() {
	SetAnkhWall(320, 0);
	SetAnkhWall(160, 0);
	SetAnkhWall(480, 0);
	SetAnkhWall(0, 80);
	SetAnkhWall(640, 80);
	if (g_gameDifficulty_ >= GDFT_VEASY) SetAnkhWall(320, 240);

	SetWall(80, 160);
	SetWall(160, 160);
	SetWall(320, 160);
	SetWall(480, 160);
	SetWall(560, 160);
	SetWall(240, 240);
	SetWall(400, 240);
	SetWall(80, 320);
	SetWall(240, 320);
	SetWall(320, 320);
	SetWall(400, 320);
	SetWall(560, 320);
	SetWall(0, 400);
	SetWall(80, 400);
	SetWall(160, 400);
	SetWall(480, 400);
	SetWall(560, 400);
	SetWall(640, 400);
	SetWall(80, 480);
	SetWall(560, 480);
	SetWall(320, 540);
	SetWall(240, 620);
	SetWall(320, 620);
	SetWall(400, 620);
	SetWall(160, 700);
	SetWall(240, 700);
	SetWall(400, 700);
	SetWall(480, 700);
}
void InitializeGameField() {
	LoadMap();

	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 320, 700, 80, 80, RenderSimple, &g_img_castle);
	AddElement(g_logicSpriteManager_, ls);
	Tank* t = CreateStronghold(ls);
	t->Dead = TankDeadFatal;
	AddElement(m_neutralTankList_, t);

	ls = CreateLogicSprite(NULL, PlayerTankRotate, 80, 700, 80, 80, RenderWithRotation, &g_img_bigTank, &g_img_bigTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	t = CreatePlayerTank(ls);
	t->Decide = PlayerInput;
	t->Act = PlayerAct;
	t->Dead = TankDeadFatal;
	m_playerTank_ = t;
	AddElement(m_playerTankList_, t);
	m_stronghold_ = t;
	ls = CreateLogicSprite(NULL, PlayerBatterySync, 160, 700, 80, 80, RenderWithRotation, &g_img_bigTankBattery, &g_img_bigTankBatteryMsk);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, PlayerHPRender, 750, 40, V6_PROGRESSBAR_WIDTH, 30, RenderWithScaling, &g_img_hp, &g_img_hpMsk);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 100, 100, 30, RenderHiScore, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 155, 100, 30, RenderScore, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 208, 100, 30, RenderWallCount, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 260, 100, 30, RenderWaveNum, NULL);
	AddElement(g_logicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 320, 100, 30, RenderTimeLeft, NULL);
	AddElement(g_logicSpriteManager_, ls);
}
static void InitGameGUI() {
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 0, 0, V6_WINDOWWIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_console);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 750, 40, 255, 30, RenderSimple, &g_img_emptyHp);
	AddElement(g_logicSpriteManager_, ls);
}

void LoadGameFromFile(LPCSTR file)
{
	m_valid_ = TRUE;
}

void GameOver()
{
	ClearContainer(m_respawn_);
	ClearContainer(m_playerItemList_);
	ClearContainer(m_neutralTankList_);
	ClearContainer(m_enemyBulletList_);
	ClearContainer(m_enemyTankList_);
	ClearContainer(m_playerBulletList_);
	ClearContainer(m_playerTankList_);
	m_valid_ = FALSE;
	LoadScene(SCENE_GAMEOVER);
}

LogicStep* g_stepGameUpdate_;
void StepGameUpdate(int t, LogicStep* tis){
	if (g_keyboardState_.pause_up) {
		m_pause_ = !m_pause_;
	}
	if (!m_pause_) {
		StepCheckEvent(t);
		StepTankStateUpdate(t);
		StepTankDecide();
		StepTankAct(t);
		StepBulletAct(t);
		StepCheckCollision();
		StepCheckValid();
	}
}
void StepCheckEvent(int ti) {
	int i;

	m_time_ -= ti;
	if (m_time_ <= 0) GameOver();

	if (m_adjustTime_ > ti) {
		m_adjustTime_ -= ti;
	}
	else if (m_adjustTime_ != 0) {
		++m_wave_;
		if (m_waveData_[m_wave_].OnStart != NULL) m_waveData_[m_wave_].OnStart();
		m_adjustTime_ = 0;
		m_generateCD_ = 0;
	}
	else if (m_waveData_[m_wave_].m_tank_total_count_ == 0 & m_enemyTankList_->m_count_ == 0) {
		if(m_waveData_[m_wave_].OnClear != NULL) m_waveData_[m_wave_].OnClear(m_enemyLatestX, m_enemyLatestY);
		m_adjustTime_ = V6_ADJUST_TIME;
	}
	else {
		m_generateCD_ -= ti;
		if (m_generateCD_ <= 0) {
			for (i = 0; i < m_activeRespawn_; ++i) {
				GenerateNewAt((Respawn*)m_respawn_->m_me_[i]);
			}
			m_generateCD_ += V6_GAME_GENERATE_CD;
		}
	}
}
void StepTankStateUpdate(int ti)
{
	Tank* t;
	for (int i = 0; i < m_playerTankList_->m_count_; ++i) {
		t = (Tank*)m_playerTankList_->m_me_[i];
		t->m_shootCD_ -= ti;
		if (t->m_shootCD_ < 0) t->m_shootCD_ = 0;
	}
	for (int i = 0; i < m_enemyTankList_->m_count_; ++i) {
		t = (Tank*)m_enemyTankList_->m_me_[i];
		t->m_shootCD_ -= ti;
		if (t->m_shootCD_ < 0) t->m_shootCD_ = 0;
	}
}
void StepTankDecide() {
	Tank* t;
	for (int i = 0; i < m_playerTankList_->m_count_; ++i) {
		t = (Tank*)m_playerTankList_->m_me_[i];
		t->Decide(t);
	}
	for (int i = 0; i < m_enemyTankList_->m_count_; ++i) {
		t = (Tank*)m_enemyTankList_->m_me_[i];
		t->Decide(t);
	}
}
void StepTankAct(int ti) {
	Tank* t;
	for (int i = 0; i < m_playerTankList_->m_count_; ++i) {
		t = (Tank*)m_playerTankList_->m_me_[i];
		t->Act(ti, t);
	}
	for (int i = 0; i < m_enemyTankList_->m_count_; ++i) {
		t = (Tank*)m_enemyTankList_->m_me_[i];
		t->Act(ti, t);
	}
}
void StepBulletAct(int ti) {
	int i;
	Bullet* b;
	for (i = 0; i < m_playerBulletList_->m_count_; ++i) {
		b = (Bullet*)(m_playerBulletList_->m_me_[i]);
		if (!b->m_valid_) continue;
		b->Update(ti, b);
		if (b->m_super_->m_x_ + b->m_super_->m_w_ < V6_GAMEFIELD_LEFT | b->m_super_->m_x_ > V6_GAMEFIELD_LEFT + V6_GAMEFIELD_WIDTH |
			b->m_super_->m_y_ + b->m_super_->m_h_ < V6_GAMEFIELD_TOP | b->m_super_->m_y_ > V6_GAMEFIELD_TOP + V6_GAMEFIELD_HEIGHT) b->m_valid_ = FALSE;
	}
	for (i = 0; i < m_enemyBulletList_->m_count_; ++i) {
		b = (Bullet*)(m_enemyBulletList_->m_me_[i]);
		if (!b->m_valid_) continue;
		b->Update(ti, b);
		if (b->m_super_->m_x_ + b->m_super_->m_w_ < V6_GAMEFIELD_LEFT | b->m_super_->m_x_ > V6_GAMEFIELD_LEFT + V6_GAMEFIELD_WIDTH |
			b->m_super_->m_y_ + b->m_super_->m_h_ < V6_GAMEFIELD_TOP | b->m_super_->m_y_ > V6_GAMEFIELD_TOP + V6_GAMEFIELD_HEIGHT) b->m_valid_ = FALSE;
	}
}
void StepCheckCollision() {
	int i;
	Item* it;
	for (i = 0; i < m_playerItemList_->m_count_; ++i) {
		it = (Item*)m_playerItemList_->m_me_[i];
		if (CollisionDetection(m_playerTank_->m_super_->m_x_ + m_playerTank_->m_collideW, m_playerTank_->m_super_->m_y_ + m_playerTank_->m_collideH,
			m_playerTank_->m_collideW, m_playerTank_->m_collideH,
			it->m_super_->m_x_, it->m_super_->m_y_, it->m_super_->m_w_, it->m_super_->m_h_)) {
			it->OnGet(m_playerTank_);
			it->m_valid_ = FALSE;
		}
	}

	CollisionCheckWithLists(m_playerBulletList_, m_enemyTankList_);
	CollisionCheckWithLists(m_playerBulletList_, m_neutralTankList_);
	CollisionCheckWithLists(m_enemyBulletList_, m_playerTankList_);
	CollisionCheckWithWallList(m_enemyBulletList_, m_neutralTankList_);
}
void CollisionCheckWithLists(Container* bullets, Container* tanks) {
	int i, j;
	Tank* t;
	Bullet* b;
	for (i = 0; i < tanks->m_count_; ++i) {
		t = (Tank*)tanks->m_me_[i];
		if (!t->m_valid_) continue;
		for (j = 0; j < bullets->m_count_; ++j) {
			b = (Bullet*)bullets->m_me_[j];
			if (!b->m_valid_) continue;
			if (CollisionDetectionWithCircle(
				t->m_super_->m_x_ + t->m_bulletCollideL, t->m_super_->m_y_ + t->m_bulletCollideT, t->m_bulletCollideW, t->m_bulletCollideH,
				b->m_super_->m_x_ + b->m_super_->m_w_ / 2, b->m_super_->m_y_ + b->m_super_->m_h_ / 2, b->m_super_->m_w_ / 2)) {
				t->m_HP -= b->m_atk_;
				if (t->m_HP <= 0) {
					if(b->m_sender_ == m_playerTank_) m_score_ += t->m_data_.m_score_;
					m_enemyLatestX = t->m_super_->m_x_;
					m_enemyLatestY = t->m_super_->m_y_;
					t->m_valid_ = FALSE;
					if (t->Dead != NULL) t->Dead(t);
				}
				b->m_valid_ = FALSE;
			}
		}
	}
}
void CollisionCheckWithWallList(Container * bullets, Container * tanks)
{
	int i, j;
	Tank* t;
	Bullet* b;
	for (i = 0; i < tanks->m_count_; ++i) {
		t = (Tank*)tanks->m_me_[i];
		if (!t->m_valid_) continue;
		for (j = 0; j < bullets->m_count_; ++j) {
			b = (Bullet*)bullets->m_me_[j];
			if (!b->m_valid_ | b->m_ignore_wall_) continue;
			if (CollisionDetectionWithCircle(
				t->m_super_->m_x_ + t->m_bulletCollideL, t->m_super_->m_y_ + t->m_bulletCollideT, t->m_bulletCollideW, t->m_bulletCollideH,
				b->m_super_->m_x_ + b->m_super_->m_w_ / 2, b->m_super_->m_y_ + b->m_super_->m_h_ / 2, b->m_super_->m_w_ / 2)) {
				t->m_HP -= (int)(b->m_atk_ * b->m_sender_->m_data_.m_damageRatioToBuilding_);
				if (t->m_HP <= 0) {
					if (b->m_sender_ == m_playerTank_) m_score_ += t->m_data_.m_score_;
					m_enemyLatestX = t->m_super_->m_x_;
					m_enemyLatestY = t->m_super_->m_y_;
					t->m_valid_ = FALSE;
					if (t->Dead != NULL) t->Dead(t);
				}
				b->m_valid_ = FALSE;
			}
		}
	}
}
void StepCheckValid() {
	int i;
	Bullet* b;
	Tank* tk;
	Item* it;
	for (i = 0; i < m_playerTankList_->m_count_; ++i) {
		tk = (Tank*)m_playerTankList_->m_me_[i];
		if (!tk->m_valid_) {
			RemoveElement(g_logicSpriteManager_, tk->m_super_);
			RemoveElement(m_playerTankList_, tk);
			--i;
		}
	}
	for (i = 0; i < m_enemyTankList_->m_count_; ++i) {
		tk = (Tank*)m_enemyTankList_->m_me_[i];
		if (!tk->m_valid_) {
			RemoveElement(g_logicSpriteManager_, tk->m_super_);
			RemoveElement(m_enemyTankList_, tk);
			--i;
		}
	}
	for (i = 0; i < m_neutralTankList_->m_count_; ++i) {
		tk = (Tank*)m_neutralTankList_->m_me_[i];
		if (!tk->m_valid_) {
			RemoveElement(g_logicSpriteManager_, tk->m_super_);
			RemoveElement(m_neutralTankList_, tk);
			--i;
		}
	}
	for (i = 0; i < m_playerBulletList_->m_count_; ++i) {
		b = (Bullet*)m_playerBulletList_->m_me_[i];
		if (!b->m_valid_) {
			RemoveElement(g_logicSpriteManager_, b->m_super_);
			RemoveElement(m_playerBulletList_, b);
			--i;
		}
	}
	for (i = 0; i < m_enemyBulletList_->m_count_; ++i) {
		b = (Bullet*)m_enemyBulletList_->m_me_[i];
		if (!b->m_valid_) {
			RemoveElement(g_logicSpriteManager_, b->m_super_);
			RemoveElement(m_enemyBulletList_, b);
			--i;
		}
	}
	for (i = 0; i < m_playerItemList_->m_count_; ++i) {
		it = (Item*)m_playerItemList_->m_me_[i];
		if (!it->m_valid_) {
			RemoveElement(g_logicSpriteManager_, it->m_super_);
			RemoveElement(m_playerItemList_, it);
			--i;
		}
	}
}

void GenerateNewAt(Respawn * t)
{
	int i, j;
	if (m_waveData_[m_wave_].m_tank_total_count_ == 0) return;
	if (!CheckPointLegal(t->m_super_->m_x_, t->m_super_->m_y_, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_playerTankList_)) return;
	if (!CheckPointLegal(t->m_super_->m_x_, t->m_super_->m_y_, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_enemyTankList_)) return;
	i = NextRand() % m_waveData_[m_wave_].m_tank_total_count_;
	for (j = 1; j < V6_TANKSTYLE_COUNT; ++j) {
		if (m_waveData_[m_wave_].m_tank_num_[j] == 0) continue;
		i -= m_waveData_[m_wave_].m_tank_num_[j];
		if (i <= 0) {
			--m_waveData_[m_wave_].m_tank_num_[j];
			--m_waveData_[m_wave_].m_tank_total_count_;
			SetNewTank((TANKSTYLE)j, t->m_super_->m_x_, t->m_super_->m_y_);
			return;
		}
	}
}
void SetWall(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, WallRender, 
		x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, RenderSimple, &g_img_wallWorn0);
	Tank* t = CreateTank(TANK_WALL, NULL, ls, g_wallData_, 0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, 0.0);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_neutralTankList_, t);
}
void SetIfWall(int x, int y)
{

}
void SetAnkhWall(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL,
		x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, RenderSimple, &g_img_ankhWall);
	Tank* t = CreateTank(TANK_WALL, NULL, ls, g_wallData_, 0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, 0.0);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_respawn_, t);
}
void SetNewTank(TANKSTYLE ts, int x, int y)
{
	switch (ts) {
	case TANK_JUNK:
		SetNewJunkTank(x, y);
		break;
	case TANK_BIG:
		SetNewBigTank(x, y);
		break;
	case TANK_PRISM:
		SetNewPrismTank(x, y);
		break;
	case TANK_FIVE:
		SetNewFiveTank(x, y);
		break;
	case TANK_QUICK:
		SetNewQuickTank(x, y);
		break;
	case TANK_SUN:
		SetNewSunTank(x, y);
		break;
	}
}
void SetNewJunkTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_junkTank, g_img_junkTankMsk);
	Tank* t = CreateJunkTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
}
void SetNewBigTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_vegetableTank, g_img_vegetableTankMsk);
	Tank* t = CreateBigTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
}
void SetNewPrismTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_prismTank, g_img_prismTankMsk);
	Tank* t = CreatePrismTank(ls);
	t->m_shoot_angle_ = (double)(NextRand() / 1023) * V6_PI * 2;
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
}
void SetNewFiveTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_fiveTank, g_img_fiveTankMsk);
	Tank* t = CreateFiveTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
}
void SetNewQuickTank(int x, int y)
{
}
void SetNewSunTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, &g_img_sunTank, &g_img_sunTankMsk);
	Tank* t = CreateSunTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
}
void SetNewLanlingkingTank(int x, int y)
{
}
void GenerateItem(int x, int y, ITEMSTYLE is)
{
	LogicSprite* ls = CreateItem(x, y, is);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_playerItemList_, ls->m_me_);
}

void PlayerInput(Tank* tis) {
	if (g_keyboardState_.right_down) tis->m_move_right_ = true; else tis->m_move_right_ = false;
	if (g_keyboardState_.up_down) tis->m_move_up_ = true; else tis->m_move_up_ = false;
	if (g_keyboardState_.left_down) tis->m_move_left_ = true; else tis->m_move_left_ = false;
	if (g_keyboardState_.down_down) tis->m_move_down_ = true; else tis->m_move_down_ = false;
	if (g_keyboardState_.shoot_down) tis->m_shoot_ = true; else tis->m_shoot_ = false;
	PlayerTank* t = (PlayerTank*)tis->m_extra_;
	if (g_keyboardState_.slow_down) t->m_slow_ = true; else t->m_slow_ = false;
	if (g_keyboardState_.set_wall_down) t->m_setWall_ = true; else t->m_setWall_ = false;
	tis->m_shoot_angle_ = CalAngle(tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - 5, tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - 10,
		g_mouseState_.x, g_mouseState_.y);
}

BOOLean CheckPointLegal(int x, int y, int w, int h, Container * c)
{
	Tank* t;
	int i;
	for (i = 0; i < c->m_count_; ++i) {
		t = (Tank*)c->m_me_[i];
		if (CollisionDetection(x, y, w, h, 
			t->m_super_->m_x_ + t->m_collideL, t->m_super_->m_y_ + t->m_collideT, t->m_collideW, t->m_collideH)) return FALSE;
	}
	return TRUE;
}
static PlayerTank* ex;
static LogicSprite* bc;
static TankData td;
static double dx;
void PlayerAct(int t, Tank* tis) {
	ex = (PlayerTank*)tis->m_extra_;
	bc = (LogicSprite*)tis->m_super_;
	td = tis->m_data_;
	dx = td.m_speed_ * t / 1000 / V6_SQRT2;
	if (ex->m_slow_) dx /= 2;
	if (tis->m_move_right_ == TRUE & tis->m_move_up_ == TRUE) {
		AlignToBarriers(tis, bc->m_x_ + dx, bc->m_y_ - dx);
		tis->m_orientation_ = V6_DRT_RIGHTUP;
	}
	else if (tis->m_move_left_ == TRUE & tis->m_move_up_ == TRUE) {
		AlignToBarriers(tis, bc->m_x_ - dx, bc->m_y_ - dx);
		tis->m_orientation_ = V6_DRT_LEFTUP;
	}
	else if (tis->m_move_left_ == TRUE & tis->m_move_down_ == TRUE) {
		AlignToBarriers(tis, bc->m_x_ - dx, bc->m_y_ + dx);
		tis->m_orientation_ = V6_DRT_LEFTDOWN;
	}
	else if (tis->m_move_right_ == TRUE & tis->m_move_down_ == TRUE) {
		AlignToBarriers(tis, bc->m_x_ + dx, bc->m_y_ + dx);
		tis->m_orientation_ = V6_DRT_RIGHTDOWN;
	}
	else {
		dx = td.m_speed_ * t / 1000;
		if (ex->m_slow_) dx /= 2;
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

	PlayerShootAct(tis);

	if (ex->m_setWall_ == TRUE & ex->m_wall_count_ > 0) {
		int i = round(tis->m_shoot_angle_ / V6_DRT_UP), ddx = bc->m_x_ + tis->m_collideL, ddy = bc->m_y_ + tis->m_collideT;
		switch (i) {
		case 0:
		case 4:
			ddx += tis->m_collideW + 10;
			break;
		case 1:
			ddy -= V6_TANK_HALF_EDGE_LENGTH * 2 + 10;
			break;
		case 2:
			ddx -= V6_TANK_HALF_EDGE_LENGTH * 2 + 10;
			break;
		case 3:
			ddy += tis->m_collideH + 10;
			break;
		}
		if (CheckPointLegal(ddx, ddy, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_enemyTankList_) &
			CheckPointLegal(ddx, ddy, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_neutralTankList_) &
			CheckPointLegal(ddx, ddy, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_respawn_) & 
			ddx >= 0 & ddy >= 0 & ddx + V6_TANK_HALF_EDGE_LENGTH * 2 <= V6_GAMEFIELD_WIDTH & 
			ddy + V6_TANK_HALF_EDGE_LENGTH * 2 <= V6_GAMEFIELD_HEIGHT) {
			--ex->m_wall_count_;
			SetWall(ddx, ddy);
		}
	}
}
void PlayerShootAct(Tank* tis) {
	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ == 0) {
		double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
		double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateSmallBullet(tis, tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_playerBulletList_, ls->m_me_);
		tis->m_shootCD_ = tis->m_data_.m_shootInterval_;
	}
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
void AlignToBarriers(Tank* t, double xnew, double ynew) {
	if (xnew + t->m_collideL + t->m_collideW > V6_GAMEFIELD_WIDTH | xnew + t->m_collideL < V6_GAMEFIELD_LEFT |
		ynew + t->m_collideT + t->m_collideH > V6_GAMEFIELD_HEIGHT | ynew + t->m_collideT < V6_GAMEFIELD_TOP) {
		return;
	}
	if (!AlignToBarrierList(t, xnew, ynew, m_playerTankList_)) return;
	if (!AlignToBarrierList(t, xnew, ynew, m_enemyTankList_)) return;
	if (!AlignToBarrierList(t, xnew, ynew, m_neutralTankList_)) return;
	t->m_super_->m_x_ = xnew;
	t->m_super_->m_y_ = ynew;
}

void TankDeadFatal(Tank * t)
{
	GameOver();
}

void PlayerTankRotate(int t, LogicSprite * ls)
{
	ex = (PlayerTank*)m_playerTank_->m_extra_;
	double a = ex->m_orientation_angle_, b = m_playerTank_->m_orientation_;
	if (a != b) {
		if (a < b) {
			if (b - a < a + V6_PI * 2 - b) {
				ex->m_orientation_angle_ += ex->m_palstance_ * t / 1000;
				if (ex->m_orientation_angle_ > b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
			else {
				b -= V6_PI * 2;
				ex->m_orientation_angle_ -= ex->m_palstance_ * t / 1000;
				if (ex->m_orientation_angle_ < b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
		}		
		else {
			if (a - b < b + V6_PI * 2 - a) {
				ex->m_orientation_angle_ -= ex->m_palstance_ * t / 1000;
				if (ex->m_orientation_angle_ < b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
			else {
				b += V6_PI * 2;
				ex->m_orientation_angle_ += ex->m_palstance_ * t / 1000;
				if (ex->m_orientation_angle_ > b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
		}
		ls->m_angle_ = ex->m_orientation_angle_;
	}
}
void PlayerBatterySync(int t, LogicSprite* ls)
{
	ls->m_x_ = m_playerTank_->m_super_->m_x_;
	ls->m_y_ = m_playerTank_->m_super_->m_y_;
	ls->m_angle_ = m_playerTank_->m_shoot_angle_;
}

void PlayerHPRender(int t, LogicSprite * ls)
{
	ls->m_w_ = double(m_playerTank_->m_HP) / double(m_playerTank_->m_data_.m_HPmax_) * V6_PROGRESSBAR_WIDTH;
}

void RenderWithDirection(int ti, LogicSprite * ls)
{
	Tank* t = (Tank*)ls->m_me_;
	int i = t->m_orientation_ / V6_DRT_UP;
	ls->m_body_->m_image_ = &(ls->m_body_->m_src_image_[i]);
	ls->m_body_->m_mask_ = &(ls->m_body_->m_src_mask_[i]);
}

void WallRender(int t, LogicSprite * ls)
{
	Tank* tk = (Tank*)ls->m_me_;
	if (tk->m_HP < tk->m_data_.m_HPmax_ / 4) 
		ls->m_body_->m_image_ = &g_img_wallWorn3;
	else if (tk->m_HP < tk->m_data_.m_HPmax_ / 2)
		ls->m_body_->m_image_ = &g_img_wallWorn2;
	else if (tk->m_HP < tk->m_data_.m_HPmax_ * 3 / 4)
		ls->m_body_->m_image_ = &g_img_wallWorn1;
	else
		ls->m_body_->m_image_ = &g_img_wallWorn0;
}

static RECT tr;
static TCHAR tstr[128];
void RenderHiScore(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), 0);
	drawtext(tstr, &tr, DT_SINGLELINE);
}
void RenderScore(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), m_score_);
	drawtext(tstr, &tr, DT_SINGLELINE);
}

void RenderWallCount(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), ((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_);
	drawtext(tstr, &tr, DT_SINGLELINE);
}

void RenderWaveNum(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), m_wave_);
	drawtext(tstr, &tr, DT_SINGLELINE);
}

void RenderTimeLeft(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), m_time_);
	drawtext(tstr, &tr, DT_SINGLELINE);
}

GAME_DIFFICULTY g_gameDifficulty_;

int m_wave_;
int m_time_;
int m_score_;
int m_activeRespawn_;

Tank* m_playerTank_;
Tank* m_stronghold_;

Container* m_playerTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_playerBulletList_ = CreateContainer(sizeof(Bullet), TRUE);
Container* m_enemyTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_enemyBulletList_ = CreateContainer(sizeof(Bullet), TRUE);
Container* m_neutralTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_playerItemList_ = CreateContainer(sizeof(Item), TRUE);
Container* m_respawn_ = CreateContainer(sizeof(Tank), TRUE);