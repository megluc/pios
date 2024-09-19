// list.h 

#ifndef LIST_H
#define LIST_H

  typedef struct ListNode {
	  struct ListNode *next; //points to the next node 
	  void *data;

}  ListNode;

  void list_add(ListNode **head, void *data);
  void *list_remove(ListNode **head);

#endif 
