#pragma once

#include "stdafx.h"

typedef struct Node {
	void* m_value_;

	Node* m_next_ = NULL;
	Node* m_prev_ = NULL;
} LinkedList;

// ��ָ���Ľڵ�֮ǰ�����µĽڵ㡣 ����ֵ���µ�ͷ�ڵ㡣���û�и���ͷ�ڵ��򷵻�NULL��
Node* LinkedListInsertPrev(Node* n1, Node* n);
void LinkedListInsertNext(Node* n1, Node* n);
Node* LinkedListDelete(Node* head, Node* n);
void LinkedListFree(Node* head);