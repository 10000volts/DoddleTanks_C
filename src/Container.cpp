#include "Container.h"

typedef void* ADDRESS;

void ClearElementsAndFreeElements(Container* ch);
void ClearElementsOnly(Container * ch);

Container * CreateContainer(int siz, BOOLean possess, void(*removeHappened)())
{
	Container * ch = (Container*)malloc(sizeof(Container));
	memset(ch, 0, sizeof(Container));
	ch->m_me_ = (void**)malloc(sizeof(void*));
	memset(ch->m_me_, 0, sizeof(void*));
	// ch->m_count_ = ch->m_reserve_ = 0;
	ch->m_siz_ = siz;
	ch->m_possess_ = possess;
	ch->RemoveHappened = removeHappened;
	return ch;
}

void AddElement(Container * ch, void * e)
{
	if (ch->m_count_ >= ch->m_reserve_) {
		ch->m_reserve_ *= 2;
		++ch->m_reserve_;
		ch->m_me_ = (void**)ExtendAndCopy(ch->m_me_, ch->m_count_, ch->m_siz_, ch->m_reserve_);
	}
	ch->m_me_[ch->m_count_] = e;
	++ch->m_count_;
}

void RemoveElement(Container * ch, void * e)
{
	int i;
	for (i = 0; i < ch->m_count_; ++i) {
		ADDRESS* e2 = ch->m_me_ + i;
		if (*e2 == e) {
			if (i != ch->m_count_ - 1) {
				if (ch->m_possess_) {
					free(*e2);
				}
				memcpy(e2, ch->m_me_ + i + 1, (ch->m_count_ - i - 1) * ch->m_siz_);
			}
			--ch->m_count_;
			if (ch->RemoveHappened != NULL) ch->RemoveHappened();
		}
	}
}

void ClearContainer(Container* c)
{
	if (c->m_possess_) {
		ClearElementsAndFreeElements(c);
	}
	else {
		ClearElementsOnly(c);
	}
	if (c->RemoveHappened != NULL) c->RemoveHappened();
}

static void ClearElementsAndFreeElements(Container * ch)
{
	int i;
	for (i = 0; i < ch->m_count_; ++i) {
		free(ch->m_me_[i]);
		ch->m_me_[i] = NULL;
	}
	free(ch->m_me_);
	ch->m_count_= 0;
	ch->m_reserve_ = 0;
	ch->m_me_ = NULL;
}

static void ClearElementsOnly(Container * ch)
{
	free(ch->m_me_);
	ch->m_count_ = 0;
	ch->m_reserve_ = 0;
	ch->m_me_ = NULL;
}
