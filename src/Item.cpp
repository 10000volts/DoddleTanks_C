#include "Item.h"
#include "LogicEngine.h"
#include "Tank.h"

static void ItemHeartGot(Tank* t);

LogicSprite* CreateItem(int x, int y, ITEMSTYLE is)
{
	IMAGE *im = NULL, *msk = NULL;
	LogicSprite* ls;
	Item* it = (Item*)malloc(sizeof(Item));
	memset(it, 0, sizeof(Item));

	it->m_item_style_ = is;
	switch (is) {
	case ITEM_HEART:
		im = &g_img_heart;
		msk = &g_img_heartMsk;
		it->OnGet = ItemHeartGot;
		break;
	}
	ls = CreateLogicSprite(it, NULL, x, y, V6_TANK_HALF_EDGE_LENGTH * 2, V6_TANK_HALF_EDGE_LENGTH * 2,
		RenderWithMask, im, msk);
	it->m_super_ = ls;
	it->m_valid_ = TRUE;
	return ls;
}

void ItemHeartGot(Tank * t)
{
	t->m_HP = t->m_data_.m_HPmax_;
}
