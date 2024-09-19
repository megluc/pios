#include "list.h"
#include <stdlib.h> 

void list_add(ListNode **head, void *data){  // new memory for a new mode 
	if(new mode) { // checks if allocation worked 
		new_node->data = data; //assigns data
		new_node->next = *head; //links to the current node 
		*head = new_node; //makes it so that the head is updated
	}
}


void *list_remove(ListNode **head){
	if(!*head) return NULL; //returns NULL if the list is empty
	ListNode *temp = *head; // stores the current head in a temp 
	*head = temp->next; // moves head to point to next node 
	
	void *data = temp->data; // gets data from temp node
	free(temp); // the original head node 
        return data; 
}

