#pragma once

#include "stdafx.h"

typedef struct Node {
	void* m_value_;

	Node* m_next_ = NULL;
	Node* m_prev_ = NULL;
} LinkedList;

// 在指定的节点之前插入新的节点。 返回值：新的头节点。如果没有更改头节点则返回NULL。
Node* LinkedListInsertPrev(Node* n1, Node* n);
void LinkedListInsertNext(Node* n1, Node* n);
Node* LinkedListDelete(Node* head, Node* n);
void LinkedListFree(Node* head);