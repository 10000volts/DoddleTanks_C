#include <stdlib.h>
#include "LinkedList.h"

Node* LinkedListInsertPrev(Node * n1, Node * n){
	if (n1->m_prev_ != NULL) {
		n1->m_prev_->m_next_ = n;
		n1->m_prev_ = n;
		return NULL;
	}
	n1->m_prev_ = n;
	return n;
}

void LinkedListInsertNext(Node * n1, Node * n){
	if (n1->m_next_ != NULL) {
		n1->m_next_->m_prev_ = n;
		n1->m_next_ = n;
	}
	else {
		n1->m_next_ = n;
	}
}

Node* LinkedListDelete(Node * head, Node * n){

}

void LinkedListFree(Node * head){
	while (head != NULL) {
		Node* n = head->m_next_;
		free(head);
		head = n;
	}
}
