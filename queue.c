#include <stdio.h>
#include <stdlib.h>

typedef struct element {
    int value;
    struct element *next;
} ele;

void print_queue(ele* head);

// Enqueues an element in a given queue. 
void enqueue(ele* queue, ele* new) {
    ele* tmp = queue;

    while (tmp->next) { // Fix: Change condition to just check for next
        tmp = tmp->next;
    }
    tmp->next = new; // Fix: Link the new node to the end
    new->next = NULL; // Ensure the new node's next pointer is NULL
}

// A method to dequeue an element from the queue.
ele* dequeue(ele* queue) {
    if (queue) {
        ele* temp = queue; // Fix: Use a temp variable to hold the head
        queue = queue->next; // Move to the next element
        free(temp); // Free the old head
        return queue; // Return the new head
    } else {
        return NULL; // Fix: Return NULL if the queue is empty
    }
}

// Method for printing out each element of a queue
void print_queue(ele* head) {
    if (head == NULL) {
        printf("QUEUE EMPTY\n"); // Fix: Added newline for better formatting
    }
    ele* tmp = head;
    while (tmp) {
        printf("%d\t", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

// Initializes a queue structure
ele* create_element(int val) {
    ele* tmp = malloc(sizeof(ele));
    tmp->value = val;
    tmp->next = NULL; // Fix: Only set next to NULL
    return tmp;
}

// Main function
int main() { // Fix: Change void main() to int main()
    // Initializes elements
    ele* head = create_element(0);
    ele* a = create_element(1);
    ele* b = create_element(2);
    ele* c = create_element(3);
    
    // Enqueues elements
    print_queue(head);
    enqueue(head, a);
    print_queue(head);
    enqueue(head, b);
    print_queue(head);
    enqueue(head, c);
    print_queue(head);

    // Empties queue
    head = dequeue(head);
    print_queue(head);
    head = dequeue(head);
    print_queue(head);
    head = dequeue(head);
    print_queue(head);
    head = dequeue(head);
    print_queue(head);

    return 0; // Added return statement for main
}

   
