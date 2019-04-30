#include "Game.h"
#include "Scene.h"
#include "TankAction.h"
#include "InputEngine.h"
#include "LogicEngine.h"
#include "RenderEngine.h"
#include "ResourceManager.h"
#include "RandomEngine.h"
#include "Utility.h"
#include "Trigger.h"
#include "GameConsole.h"
#include "SaveAndLoad.h"
#include "RankingList.h"

#include <cstdio>

#define V6_GAME_TIME	2700000
#define V6_ADJUST_TIME	5000
typedef Tank Respawn;

// 从文件加载游戏。
static void LoadGameFromFile(const char file[]);
static void InitGameGUI();
static void InitializePlayer();
static void LoadMap();

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
// 检查并执行触发器。
void StepRunTrigger(int t);
// 清除死亡的坦克和子弹。
static void StepCheckValid();

// 保存当前游戏进度。
static void SaveGame();

// 在重生点的位置尝试生成新的敌人。
static void GenerateNewAt(Respawn* t);
// 没有作生成合法性判定。
static Tank* SetWall(int x, int y);
// 没有作生成合法性判定。
static void SetIfWall(int x, int y);
// 没有作生成合法性判定。
// 设置敌方坦克重生点。
static void SetAnkhWall(int x, int y);
// 在指定位置生成指定坦克，没有作生成合法性判定。
static Tank* SetNewTank(TANKSTYLE ts, int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewJunkTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewBigTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewPrismTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewFiveTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewQuickTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewSunTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewLanlingkingTank(int x, int y);
// 没有作生成合法性判定。
static Tank* SetNewAttackTank(int x, int y);

// 判断在指定列表内的坦克元素是否与指定矩形有碰撞。若有，返回该坦克。
static Tank* CheckRectWithTankList(int x, int y, int w, int h, Container* c);
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

TankData m_tankData_[V6_TANKSTYLE_COUNT];
static TankWave m_waveData_[V6_GAME_MAX_WAVE + 1];

// 当前最后被摧毁的敌方坦克的坐标，用于存储回合结束时触发事件的位置。
static int m_enemyLatestX, m_enemyLatestY;

void LoadGameStepResource() {
	g_stepGameUpdate_ = CreateLogicStep((char*)"CheckEvent", StepGameUpdate);
}

void InitializeGame(BOOLean loadFromLife)
{
	m_success_ = FALSE;
	m_failed_ = FALSE;
	if (loadFromLife) {
		LoadGameFromFile("save.dat");
	}
	else {
		int i;
		--m_wave_;
		m_special_wave_ = FALSE;
		m_special_wave_end_ = FALSE;
		m_adjustTime_ = V6_ADJUST_TIME;
		m_time_ = V6_GAME_TIME;
		m_score_ = 0;
		m_playingAnimation_ = FALSE;
		memcpy(m_tankData_, g_tankData_[g_gameDifficulty_], sizeof(TankData) * V6_TANKSTYLE_COUNT);
		if (g_easyMode_) {
			for (i = 0; i < V6_TANKSTYLE_COUNT; ++i) {
				m_tankData_[i].m_score_ /= 10;
			}
		}
		memcpy(m_waveData_, g_waveData_[g_gameDifficulty_], sizeof(TankWave) * (V6_GAME_MAX_WAVE + 1));
		for (i = 1; i <= m_wave_; ++i) {
			if (m_waveData_[i].OnStart != NULL) m_waveData_[i].OnStart();
			memset(&(m_waveData_[i]), 0, sizeof(TankWave));
		}
		InitGameGUI();
		LoadMap();
		InitializePlayer();
	}

	AddElement(g_logicStepManager_, g_stepGameUpdate_);
}
static void LoadRespawn() {
	SetAnkhWall(320, 0);
	SetAnkhWall(160, 0);
	SetAnkhWall(480, 0);
	SetAnkhWall(0, 80);
	SetAnkhWall(640, 80);
	if (g_gameDifficulty_ >= GDFT_VEASY) SetAnkhWall(320, 240);
}
void LoadMap() {
	LoadRespawn();

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
// 据点必须是序号为0的中立坦克。
void InitializePlayer() {
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, 320, 700, 80, 80, RenderSimple, &g_img_castle);
	AddElement(g_logicSpriteManager_, ls);
	Tank* t = CreateStronghold(ls);
	t->Dead = TankDeadFatal;
	m_stronghold_ = t;
	AddElement(m_neutralTankList_, t);

	ls = CreateLogicSprite(NULL, PlayerTankRotate, 80, 700, 80, 80, RenderWithRotation, &g_img_bigTank, &g_img_bigTankMsk);
	AddElement(g_logicSpriteManager_, ls);
	t = CreatePlayerTank(ls);
	t->Decide = PlayerInput;
	t->Act = PlayerAct;
	t->Dead = TankDeadFatal;
	if (g_easyMode_) {
		t->m_data_.m_HPmax_ = 10000; 
		t->m_HP = 10000;
	}
	m_playerTank_ = t;
	AddElement(m_playerTankList_, t);
	ls = CreateLogicSprite(NULL, PlayerBatterySync, 160, 700, 80, 80, RenderWithRotation, &g_img_bigTankBattery, &g_img_bigTankBatteryMsk);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, PlayerHPRender, 750, 40, V6_PROGRESSBAR_WIDTH, 30, RenderWithScaling, &g_img_hp, &g_img_hpMsk);
	AddElement(g_topLogicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 100, 100, 30, RenderHiScore, NULL);
	AddElement(g_topLogicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 155, 100, 30, RenderScore, NULL);
	AddElement(g_topLogicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 208, 100, 30, RenderWallCount, NULL);
	AddElement(g_topLogicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 260, 100, 30, RenderWaveNum, NULL);
	AddElement(g_topLogicSpriteManager_, ls);
	ls = CreateLogicSprite(t, NULL, 870, 317, 100, 30, RenderTimeLeft, NULL);
	AddElement(g_topLogicSpriteManager_, ls);
}
static void InitGameGUI() {
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, V6_GAMEFIELD_LEFT, V6_GAMEFIELD_TOP,
		V6_GAMEFIELD_WIDTH, V6_WINDOWHEIGHT, RenderSimple, &g_img_gamebg);
	AddElement(g_logicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, V6_GAMEFIELD_LEFT + V6_GAMEFIELD_WIDTH, V6_GAMEFIELD_TOP,
		V6_WINDOWWIDTH - V6_GAMEFIELD_WIDTH, V6_WINDOWHEIGHT - V6_WINDOWHEIGHT, RenderSimple, &g_img_console);
	AddElement(g_topLogicSpriteManager_, ls);

	ls = CreateLogicSprite(NULL, NULL, 750, 40, 255, 30, RenderSimple, &g_img_emptyHp);
	AddElement(g_topLogicSpriteManager_, ls);
}

void LoadGameFromFile(const char file[])
{
	/*
		存档文件结构：
		当前波次 最大波次 最大坦克种类(用于保持可扩展性，以适应之后还可能添加的新的坦克/新的波次模式) adjustTime
		波次数据
		当前分数
		剩余时间
		剩余的可放置墙数目
		玩家坦克(HP 位置.xy shootCD)
		据点坦克(HP 位置.xy)
		敌方坦克数目
		敌方坦克(HP 位置.xy shootCD)
		中立坦克数目
		中立坦克(HP 位置.xy shootCD)
		我方子弹数目
		我方子弹(子弹种类 位置.xy 速度.xy)
		敌方子弹数目
		敌方子弹
		道具数目
		道具(种类 位置.xy)
	*/

	int i, j;
	int n, m;
	int a, b, c;
	double d, e, f, g, h;
	LogicSprite* ls;
	if (freopen(file, "r", stdin) == NULL) {
		return;
	}
	m_special_wave_ = FALSE;
	m_special_wave_end_ = FALSE;

	memcpy(m_waveData_, g_waveData_[g_gameDifficulty_], sizeof(TankWave) * (V6_GAME_MAX_WAVE + 1));
	scanf("%d%d%d%d", &m_wave_, &n, &m, &m_adjustTime_);
	for (i = 1; i <= n; ++i) {
		m_waveData_[i].m_tank_total_count_ = 0;
		for (j = 0; j < m; ++j) {
			scanf("%d", &m_waveData_[i].m_tank_num_[j]);
			m_waveData_[i].m_tank_total_count_ += m_waveData_[i].m_tank_num_[j];
		}
	}

	scanf("%d", &m_score_);
	scanf("%d", &m_time_);
	m_playingAnimation_ = FALSE;
	scanf("%d", &m);

	memcpy(m_tankData_, g_tankData_[g_gameDifficulty_], sizeof(TankData) * V6_TANKSTYLE_COUNT);
	if (g_easyMode_) {
		for (i = 0; i < V6_TANKSTYLE_COUNT; ++i) {
			m_tankData_[i].m_score_ /= 10;
		}
	}

	for (i = 1; i <= m_wave_; ++i) {
		if (m_waveData_[i].OnStart != NULL) m_waveData_[i].OnStart();
	}
	if (m_adjustTime_ > 0) {
		m_waveData_[m_wave_].OnClear = NULL;
	}

	InitGameGUI();
	LoadRespawn();
	InitializePlayer();
	
	scanf("%d%lf%lf%d", &m_playerTank_->m_HP, &m_playerTank_->m_super_->m_x_, &m_playerTank_->m_super_->m_y_,
		  &m_playerTank_->m_shootCD_);
	((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_ = m;
	scanf("%d%lf%lf", &m_stronghold_->m_HP, &m_stronghold_->m_super_->m_x_, &m_stronghold_->m_super_->m_y_);
	
	scanf("%d", &n);
	for (i = 0; i < n; ++i) {
		scanf("%d%d%lf%lf%d", &a, &b, &e, &f, &c);
		Tank* t = SetNewTank((TANKSTYLE)a, e, f);
		t->m_HP = b;
		t->m_shootCD_ = c;
	}
	scanf("%d", &n);
	for (i = 0; i < n; ++i) {
		scanf("%d%d%lf%lf%d", &a, &b, &e, &f, &c);
		Tank* t = SetNewTank((TANKSTYLE)a, e, f);
		t->m_HP = b;
		t->m_shootCD_ = c;
	}

	scanf("%d", &n);
	for (i = 0; i < n; ++i) {
		scanf("%d%lf%lf%d%d%lf%lf%lf", &a, &e, &f, &b, &c, &d, &g, &h);
		ls = CreateBullet((BULLETSTYLE)a, e, f, b, c, d, g, h, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_playerBulletList_, ls->m_me_);
	}
	scanf("%d", &n);
	for (i = 0; i < n; ++i) {
		scanf("%d%lf%lf%d%d%lf%lf%lf", &a, &e, &f, &b, &c, &d, &g, &h);
		ls = CreateBullet((BULLETSTYLE)a, e, f, b, c, d, g, h, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_enemyBulletList_, ls->m_me_);
	}
	
	scanf("%d", &n);
	for (i = 0; i < n; ++i) {
		scanf("%d%lf%lf", &a, &d, &e);
		ls = CreateItem(d, e, (ITEMSTYLE)a);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_playerItemList_, ls->m_me_);
	}
}

void GameOver(BOOLean success)
{
	ClearContainer(m_triggerList_);
	ClearContainer(m_respawn_);
	ClearContainer(m_playerItemList_);
	ClearContainer(m_neutralTankList_);
	ClearContainer(m_enemyBulletList_);
	ClearContainer(m_enemyTankList_);
	ClearContainer(m_playerBulletList_);
	ClearContainer(m_playerTankList_);
	if (!success) {
		LoadScene(SCENE_GAMEOVER);
	}
	else {
		LoadScene(SCENE_STAGECLEAR);
	}
}

LogicStep* g_stepGameUpdate_;
void StepGameUpdate(int t, LogicStep* tis){
	if (g_keyboardState_.pause_up) {
		m_pause_ = !m_pause_;
		if(!m_special_wave_)
			SaveGame();
	}
	if (!m_pause_) {
		if (!m_playingAnimation_) {
			StepCheckEvent(t);
			StepTankStateUpdate(t);
			StepTankDecide();
			StepTankAct(t);
			StepBulletAct(t);
			StepCheckCollision();
		}
		StepRunTrigger(t);
		StepCheckValid();
	}
}
void StepCheckEvent(int ti) {
	int i;

	m_time_ -= ti;
	if (m_time_ <= 0) GameOver();

	// 新的回合到来倒计时。
	if (m_adjustTime_ > ti) {
		m_adjustTime_ -= ti;
	}
	// 新的回合到来。
	else if (m_adjustTime_ != 0) {
		++m_wave_;
		if (m_waveData_[m_wave_].OnStart != NULL) m_waveData_[m_wave_].OnStart();
		m_adjustTime_ = 0;
		m_generateCD_ = 0;
	}
	else if (m_special_wave_) {
		if (m_special_wave_end_) {
			if (m_waveData_[m_wave_].OnClear != NULL) m_waveData_[m_wave_].OnClear(m_enemyLatestX, m_enemyLatestY);
			m_adjustTime_ = V6_ADJUST_TIME;
			m_special_wave_ = FALSE;
			m_special_wave_end_ = FALSE;
		}
	}
	// 开始新的回合到来倒计时。
	else if (m_waveData_[m_wave_].m_tank_total_count_ == 0 & m_enemyTankList_->m_count_ == 0) {
		if(m_waveData_[m_wave_].OnClear != NULL) m_waveData_[m_wave_].OnClear(m_enemyLatestX, m_enemyLatestY);
		m_adjustTime_ = V6_ADJUST_TIME;
	}
	// 产生新的敌人。
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
		if(t->m_shootCD_ > 0) t->m_shootCD_ -= ti;
	}
	for (int i = 0; i < m_enemyTankList_->m_count_; ++i) {
		t = (Tank*)m_enemyTankList_->m_me_[i];
		if (t->m_shootCD_ > 0) t->m_shootCD_ -= ti;
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
		if (b->m_super_->m_x_ + b->m_super_->m_w_ < V6_GAMEFIELD_LEFT + b->m_validXMin_ | b->m_super_->m_x_ > V6_GAMEFIELD_LEFT + b->m_validXMax_ |
			b->m_super_->m_y_ + b->m_super_->m_h_ < V6_GAMEFIELD_TOP + b->m_validYMin_ | b->m_super_->m_y_ > V6_GAMEFIELD_TOP + b->m_validYMax_) {
			Invalid(b);
		}
	}
	for (i = 0; i < m_enemyBulletList_->m_count_; ++i) {
		b = (Bullet*)(m_enemyBulletList_->m_me_[i]);
		if (!b->m_valid_) continue;
		b->Update(ti, b);
		if (b->m_super_->m_x_ + b->m_super_->m_w_ < V6_GAMEFIELD_LEFT + b->m_validXMin_ | b->m_super_->m_x_ > V6_GAMEFIELD_LEFT + b->m_validXMax_ |
			b->m_super_->m_y_ + b->m_super_->m_h_ < V6_GAMEFIELD_TOP + b->m_validYMin_ | b->m_super_->m_y_ > V6_GAMEFIELD_TOP + b->m_validYMax_) {
			Invalid(b);
		}
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
					//if(b->m_sender_ == m_playerTank_) 
					m_score_ += t->m_data_.m_score_;
					m_enemyLatestX = t->m_super_->m_x_;
					m_enemyLatestY = t->m_super_->m_y_;
					t->m_valid_ = FALSE;
					if (t->Dead != NULL) 
						t->Dead(t);
				}
				Invalid(b);
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

				t->m_HP -= (int)(b->m_atk_ * b->m_damageRatioToBuilding_);
				if (t->m_HP <= 0) {
					// if (b->m_sender_ == m_playerTank_) m_score_ += t->m_data_.m_score_;
					t->m_valid_ = FALSE;
					if (t->Dead != NULL)
						t->Dead(t);
				}
				Invalid(b);
			}
		}
	}
}
void StepRunTrigger(int t) {
	int i;
	Trigger* tr;
	for (i = 0; i < m_triggerList_->m_count_; ++i) {
		tr = (Trigger*)m_triggerList_->m_me_[i];
		if (tr->Check(tr, t)) {
			tr->Act(tr, t);
			if (tr->m_trigger_max_times_ != V6_TRIGGER_INFINITE) {
				if (tr->m_trigger_max_times_ < 2) tr->m_valid_ = FALSE;
				--tr->m_trigger_max_times_;
			}
		}
	}
}
void StepCheckValid() {
	if (m_success_) {
		GameOver(V6_GAMEOVER_SUCCESS);
	}
	else if (m_failed_) {
		GameOver();
	}
	else {
		int i;
		Bullet* b;
		Tank* tk;
		Item* it;
		Trigger* t;
		for (i = 0; i < m_playerTankList_->m_count_; ++i) {
			tk = (Tank*)m_playerTankList_->m_me_[i];
			if (!tk->m_valid_) {
				if (tk->m_extra_ != NULL) free(tk->m_extra_);
				RemoveElement(g_logicSpriteManager_, tk->m_super_);
				RemoveElement(m_playerTankList_, tk);
				--i;
			}
		}
		for (i = 0; i < m_enemyTankList_->m_count_; ++i) {
			tk = (Tank*)m_enemyTankList_->m_me_[i];
			if (!tk->m_valid_) {
				if (tk->m_extra_ != NULL) free(tk->m_extra_);
				RemoveElement(g_logicSpriteManager_, tk->m_super_);
				RemoveElement(m_enemyTankList_, tk);
				--i;
			}
		}
		for (i = 0; i < m_neutralTankList_->m_count_; ++i) {
			tk = (Tank*)m_neutralTankList_->m_me_[i];
			if (!tk->m_valid_) {
				if (tk->m_extra_ != NULL) free(tk->m_extra_);
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
		for (i = 0; i < m_triggerList_->m_count_; ++i) {
			t = (Trigger*)m_triggerList_->m_me_[i];
			if (!t->m_valid_) {
				RemoveElement(m_triggerList_, t);
				--i;
			}
		}
	}
}

void SaveGame() {
	char s[100008];
	char ts[100008];
	memset(s, 0, sizeof(s));
	memset(ts, 0, sizeof(s));
	int i, j;
	Tank* t;
	Bullet* b;
	Item* item;
	/*
		存档文件结构：
		当前波次 最大波次 最大坦克种类(用于保持可扩展性，以适应之后还可能添加的新的坦克/新的波次模式) adjustTime
		波次数据
		当前分数
		剩余时间
		剩余的可放置墙数目
		玩家坦克(HP 位置.xy shootCD)
		据点坦克(HP 位置.xy)
		敌方坦克数目
		敌方坦克(HP 位置.xy shootCD)
		中立坦克数目
		中立坦克(HP 位置.xy shootCD)
		我方子弹数目
		我方子弹(子弹种类 位置.xy 速度.xy)
		敌方子弹数目
		敌方子弹
		道具数目
		道具(种类 位置.xy)
	*/
	sprintf(s, "%d %d %d %d\n", m_wave_, V6_GAME_MAX_WAVE, V6_TANKSTYLE_COUNT, m_adjustTime_);
	for (i = 1; i <= V6_GAME_MAX_WAVE; ++i) {
		for (j = 0; j < V6_TANKSTYLE_COUNT - 1; ++j) {
			sprintf(ts, "%d ", m_waveData_[i].m_tank_num_[j]);
			strcat(s, ts);
		}
		sprintf(ts, "%d\n", m_waveData_[i].m_tank_num_[j]);
		strcat(s, ts);
	}
	sprintf(ts, "%d\n%d\n%d\n", m_score_, m_time_, ((PlayerTank*)m_playerTank_->m_extra_)->m_wall_count_);
	strcat(s, ts);

	sprintf(ts, "%d %lf %lf %d\n", m_playerTank_->m_HP, m_playerTank_->m_super_->m_x_, m_playerTank_->m_super_->m_y_, m_playerTank_->m_shootCD_);
	strcat(s, ts);
	sprintf(ts, "%d %lf %lf\n", m_stronghold_->m_HP, m_stronghold_->m_super_->m_x_, m_stronghold_->m_super_->m_y_);
	strcat(s, ts);

	sprintf(ts, "%d\n", m_enemyTankList_->m_count_);
	strcat(s, ts);
	for (i = 0; i < m_enemyTankList_->m_count_; ++i) {
		t = (Tank*)m_enemyTankList_->m_me_[i];
		sprintf(ts, "%d %d %lf %lf %d\n", t->m_tankStyle_, t->m_HP, t->m_super_->m_x_, t->m_super_->m_y_, t->m_shootCD_);
		strcat(s, ts);
	}
	// -1: 减去己方据点。
	sprintf(ts, "%d\n", m_neutralTankList_->m_count_ - 1);
	strcat(s, ts);
	for (i = 0; i < m_neutralTankList_->m_count_; ++i) {
		t = (Tank*)m_neutralTankList_->m_me_[i];
		if (t != m_stronghold_) {
			sprintf(ts, "%d %d %lf %lf %d\n", t->m_tankStyle_, t->m_HP, t->m_super_->m_x_, t->m_super_->m_y_, t->m_shootCD_);
			strcat(s, ts);
		}
	}
	sprintf(ts, "%d\n", m_playerBulletList_->m_count_);
	strcat(s, ts);
	for (i = 0; i < m_playerBulletList_->m_count_; ++i) {
		b = (Bullet*)m_playerBulletList_->m_me_[i];
		sprintf(ts, "%d %lf %lf %d %d %lf %lf %lf\n", b->m_bulletStyle_, b->m_super_->m_x_, b->m_super_->m_y_,
				b->m_atk_, b->m_ignore_wall_, b->m_damageRatioToBuilding_, b->m_speedX_, b->m_speedY_);
		strcat(s, ts);
	}
	sprintf(ts, "%d\n", m_enemyBulletList_->m_count_);
	strcat(s, ts);
	for (i = 0; i < m_enemyBulletList_->m_count_; ++i) {
		b = (Bullet*)m_enemyBulletList_->m_me_[i];
		sprintf(ts, "%d %lf %lf %d %d %lf %lf %lf\n", b->m_bulletStyle_, b->m_super_->m_x_, b->m_super_->m_y_,
				b->m_atk_, b->m_ignore_wall_, b->m_damageRatioToBuilding_, b->m_speedX_, b->m_speedY_);
		strcat(s, ts);
	}

	sprintf(ts, "%d\n", m_playerItemList_->m_count_);
	strcat(s, ts);
	for (i = 0; i < m_playerItemList_->m_count_; ++i) {
		item = (Item*)m_playerItemList_->m_me_[i];
		sprintf(ts, "%d %lf %lf\n", item->m_item_style_, item->m_super_->m_x_, item->m_super_->m_y_);
		strcat(s, ts);
	}

	SaveData(s, "save.dat", (strlen(s) / 16 + 1) * 16 * sizeof(char));
}

void GenerateNewAt(Respawn * t)
{
	int i, j;
	if (m_waveData_[m_wave_].m_tank_total_count_ == 0) return;
	if (CheckRectWithTankList(t->m_super_->m_x_, t->m_super_->m_y_, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_playerTankList_)
		!= NULL) return;
	if (CheckRectWithTankList(t->m_super_->m_x_, t->m_super_->m_y_, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_enemyTankList_)
		!= NULL) return;
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
Tank* SetWall(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, WallRender, 
		x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, RenderSimple, &g_img_wallWorn0);
	Tank* t = CreateTank(TANK_WALL, NULL, ls, m_tankData_[TANK_WALL], 0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, 0.0);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_neutralTankList_, t);
	return t;
}
void SetIfWall(int x, int y)
{

}
void SetAnkhWall(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL,
		x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, RenderSimple, &g_img_ankhWall);
	Tank* t = CreateTank(TANK_WALL, NULL, ls, m_tankData_[TANK_WALL], 0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		0, 0, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, 0.0);
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_respawn_, t);
}
Tank* SetNewTank(TANKSTYLE ts, int x, int y)
{
	switch (ts) {
	case TANK_WALL:
		return SetWall(x, y);
	case TANK_JUNK:
		return SetNewJunkTank(x, y);
	case TANK_BIG:
		return SetNewBigTank(x, y);
	case TANK_PRISM:
		return SetNewPrismTank(x, y);
	case TANK_FIVE:
		return SetNewFiveTank(x, y);
	case TANK_QUICK:
		return SetNewQuickTank(x, y);
	case TANK_SUN:
		return SetNewSunTank(x, y);
	case TANK_LANLINGKING:
		return SetNewLanlingkingTank(x, y);
	case TANK_ATTACK:
		return SetNewAttackTank(x, y);
	}
	return NULL;
}
Tank* SetNewJunkTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_junkTank, g_img_junkTankMsk);
	Tank* t = CreateJunkTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewBigTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_vegetableTank, g_img_vegetableTankMsk);
	Tank* t = CreateBigTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewPrismTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_prismTank, g_img_prismTankMsk);
	Tank* t = CreatePrismTank(ls);
	t->m_shoot_angle_ = (double)(NextRand()) / V6_RAND_MAX_F * V6_PI * 2;
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewFiveTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_fiveTank, g_img_fiveTankMsk);
	Tank* t = CreateFiveTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewQuickTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, RenderWithDirection, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_quickTank, g_img_quickTankMsk);
	Tank* t = CreateQuickTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewSunTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, &g_img_sunTank, &g_img_sunTankMsk);
	Tank* t = CreateSunTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewLanlingkingTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_lanlingkingTank, g_img_lanlingkingTankMsk);
	Tank* t = CreateLanlingkingTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
}
Tank* SetNewAttackTank(int x, int y)
{
	LogicSprite* ls = CreateLogicSprite(NULL, NULL, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, g_img_attackTank, g_img_attackTankMsk);
	Tank* t = CreateAttackTank(ls);
	ls->m_me_ = t;
	AddElement(g_logicSpriteManager_, ls);
	AddElement(m_enemyTankList_, t);
	return t;
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
	if (g_keyboardState_.set_wall_up) 
		t->m_setWall_ = true;
	else
		t->m_setWall_ = false;
	tis->m_shoot_angle_ = CalAngle(tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - 5, tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - 10,
		g_mouseState_.x, g_mouseState_.y);
}

Tank * CheckRectWithTankList(int x, int y, int w, int h, Container * c)
{
	Tank* t;
	int i;
	for (i = 0; i < c->m_count_; ++i) {
		t = (Tank*)c->m_me_[i];
		if (CollisionDetection(x, y, w, h,
			t->m_super_->m_x_ + t->m_collideL, t->m_super_->m_y_ + t->m_collideT, t->m_collideW, t->m_collideH)) return t;
	}
	return NULL;
}
static PlayerTank* ex;
static LogicSprite* bc;
static TankData td;
static double dx;
void PlayerAct(int t, Tank* tis) {
	ex = (PlayerTank*)tis->m_extra_;
	bc = (LogicSprite*)tis->m_super_;
	td = tis->m_data_;
	dx = td.m_speed_ * double(t) / 1000.0 / V6_SQRT2;
	if (ex->m_slow_) dx /= 1.4;
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
		dx = td.m_speed_ * double(t) / 1000.0;
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
		int i = round(tis->m_shoot_angle_ / V6_DRT_UP), ddx, ddy;
		switch (i) {
		case 0:
		case 4:
			ddx = ceil((bc->m_x_ + tis->m_collideL + tis->m_collideW) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			ddy = round((bc->m_y_ + tis->m_collideT) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			break;
		case 1:
			ddy = floor((bc->m_y_ + tis->m_collideT) / V6_TANK_HALF_EDGE_LENGTH / 2.0 - 1.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			ddx = round((bc->m_x_ + tis->m_collideL) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			break;
		case 2:
			ddx = floor((bc->m_x_ + tis->m_collideL) / V6_TANK_HALF_EDGE_LENGTH / 2.0 - 1.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			ddy = round((bc->m_y_ + tis->m_collideT) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			break;
		case 3:
			ddy = ceil((bc->m_y_ + tis->m_collideT + tis->m_collideH) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			ddx = round((bc->m_x_ + tis->m_collideL) / V6_TANK_HALF_EDGE_LENGTH / 2.0) * V6_TANK_HALF_EDGE_LENGTH * 2;
			break;
		}
		Tank* tk = CheckRectWithTankList(ddx + 1, ddy + 1, V6_TANK_HALF_EDGE_LENGTH * 2 - 2, V6_TANK_HALF_EDGE_LENGTH * 2 - 2, m_neutralTankList_);
		if (tk != NULL) {
			tk->m_HP += 800;
			--ex->m_wall_count_;
		}
		else if (CheckRectWithTankList(ddx, ddy, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2, m_enemyTankList_) == NULL &
			ddx >= 0 & ddy >= 0 & ddx + V6_TANK_HALF_EDGE_LENGTH * 2 <= V6_GAMEFIELD_WIDTH &
			ddy + V6_TANK_HALF_EDGE_LENGTH * 2 <= V6_GAMEFIELD_HEIGHT) {
			SetWall(ddx, ddy);
			--ex->m_wall_count_;
		}
	}
}
void PlayerShootAct(Tank* tis) {
	if (tis->m_shoot_ == TRUE & tis->m_shootCD_ <= 0) {
		double sx = tis->m_data_.m_bulletSpeed_ * cos(tis->m_shoot_angle_);
		double sy = -tis->m_data_.m_bulletSpeed_ * sin(tis->m_shoot_angle_);
		LogicSprite* ls = CreateSmallBullet(tis->m_super_->m_x_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_super_->m_y_ + V6_TANK_HALF_EDGE_LENGTH - V6_SMALLBULLET_EDGE_LENGTH / 2,
			tis->m_data_.m_atk_, FALSE, tis->m_data_.m_damageRatioToBuilding_, sx, sy, BulletNormalUpdate);
		AddElement(g_logicSpriteManager_, ls);
		AddElement(m_playerBulletList_, ls->m_me_);
		tis->m_shootCD_ += tis->m_data_.m_shootInterval_;
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
	m_failed_ = TRUE;
}

static BOOLean TriggerCheckDelay(Trigger* tis, int t) {
	if (tis->m_exdata1_ > t) {
		tis->m_exdata1_ -= t;
		return FALSE;
	}
	else
		return TRUE;
}
static void TriggerActDelay(Trigger* tis, int t) {
	((void(*)())(tis->m_binging_obj_))();
}
void Delay(int t, void(*act)()) {
	Trigger* tr = CreateTrigger(TriggerCheckDelay, TriggerActDelay, 1, act);
	AddElement(m_triggerList_, tr);
}
static void TriggerActDelayWithTwoArgs(Trigger* tis, int t) {
	((void(*)(int, int))(tis->m_binging_obj_))(tis->m_exdata3_, tis->m_exdata4_);
}
void DelayWithTwoArgs(int t, void(*act)(int, int), int exd1, int exd2)
{
	Trigger* tr = CreateTrigger(TriggerCheckDelay, TriggerActDelayWithTwoArgs, 1, act, t, 0, exd1, exd2);
	AddElement(m_triggerList_, tr);
}

void PlayerTankRotate(int t, LogicSprite * ls)
{
	ex = (PlayerTank*)m_playerTank_->m_extra_;
	double a = ex->m_orientation_angle_, b = m_playerTank_->m_orientation_;
	if (a != b) {
		if (a < b) {
			if (b - a < a + V6_PI * 2 - b) {
				ex->m_orientation_angle_ += ex->m_palstance_ * double(t) / 1000.0;
				if (ex->m_orientation_angle_ > b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
			else {
				b -= V6_PI * 2;
				ex->m_orientation_angle_ -= ex->m_palstance_ * double(t) / 1000.0;
				if (ex->m_orientation_angle_ < b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
		}		
		else {
			if (a - b < b + V6_PI * 2 - a) {
				ex->m_orientation_angle_ -= ex->m_palstance_ * double(t) / 1000.0;
				if (ex->m_orientation_angle_ < b) ex->m_orientation_angle_ = b;
				ex->m_orientation_angle_ = ConvertIntoStandard(ex->m_orientation_angle_);
			}
			else {
				b += V6_PI * 2;
				ex->m_orientation_angle_ += ex->m_palstance_ * double(t) / 1000.0;
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
	if (tk->m_HP <= 600) 
		ls->m_body_->m_image_ = &g_img_wallWorn3;
	else if (tk->m_HP <= 1200)
		ls->m_body_->m_image_ = &g_img_wallWorn2;
	else if (tk->m_HP <= 1800)
		ls->m_body_->m_image_ = &g_img_wallWorn1;
	else if (tk->m_HP <= 3200)
		ls->m_body_->m_image_ = &g_img_wallWorn0;
	else if (tk->m_HP <= 6400)
		ls->m_body_->m_image_ = &g_img_wailingWall1;
	else
		ls->m_body_->m_image_ = &g_img_wailingWall2;
}

static RECT tr;
static TCHAR tstr[128];
void RenderHiScore(LogicSprite * ls)
{
	tr.left = ls->m_x_;
	tr.top = ls->m_y_;
	tr.right = ls->m_x_ + ls->m_w_;
	tr.bottom = ls->m_y_ + ls->m_h_;

	_stprintf(tstr, _T("%d"), g_top_scores_[0].score);
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

BOOLean m_success_;
BOOLean m_failed_; 
BOOLean m_special_wave_;
BOOLean m_special_wave_end_;
int m_wave_;
int m_time_;
int m_score_;
int m_activeRespawn_;

BOOLean m_playingAnimation_;

Tank* m_playerTank_;
Tank* m_stronghold_;

Container* m_playerTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_playerBulletList_ = CreateContainer(sizeof(Bullet), TRUE);
Container* m_enemyTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_enemyBulletList_ = CreateContainer(sizeof(Bullet), TRUE);
Container* m_neutralTankList_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_playerItemList_ = CreateContainer(sizeof(Item), TRUE);
Container* m_respawn_ = CreateContainer(sizeof(Tank), TRUE);
Container* m_triggerList_ = CreateContainer(sizeof(Trigger), TRUE);