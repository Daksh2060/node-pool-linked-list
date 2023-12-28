#include <stdio.h>
#include "list.h"

// Define the maximum number of nodes and heads for the list
Node nodes[LIST_MAX_NUM_NODES];
List lists[LIST_MAX_NUM_HEADS];

// Initialize stack-related variables and constants
static int stackTop = -1;
static bool stackInitialized = false;
static bool lists_set = false;
static int end1 = LIST_MAX_NUM_NODES + 1;
static int end2 = LIST_MAX_NUM_NODES + 2;

// Create a stack within nodes array to help manage free nodes
static void initializeStack() {
    // Initialize each node in the array, setting the next_free pointer and initialization status
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        nodes[i].next_free = i;
        nodes[i].initialized = false;
    }
    // Set the stack top to the maximum node index
    stackTop = LIST_MAX_NUM_NODES - 1;
}

// Pop from custom stack
static int popFromStack() {
    if (stackTop >= 0) {
        return nodes[stackTop--].next_free;
    } else {
        return -1;
    }
}

// Push to the stack
static void pushToStack(int nodeIndex) {
    if (stackTop < LIST_MAX_NUM_NODES - 1) {
        nodes[++stackTop].next_free = nodeIndex;
    } else {
        printf("No more free nodes\n");
    }
}

// Initialize lists
static void setlist() {
    // Set the initialization status of each list to false
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        lists[i].initialized = false;
    }
}

// Makes a new, empty list, and returns its reference on success.
// Returns a NULL pointer on failure.
List* List_create() {
    int index_list = -1;

    // Initialize lists if not done already
    if (lists_set == false) {
        setlist();
        lists_set = true;
    }

    // Initialize stack if not done already
    if (stackInitialized == false) {
        initializeStack();
        stackInitialized = true;
    }

    // Find the first uninitialized list
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        if (lists[i].initialized == false) {
            index_list = i;
            break;
        }
    }

    // Return NULL if all lists are initialized
    if (index_list == -1) {
        return NULL;
    }

    // Initialize the found list
    lists[index_list].initialized = true;
    lists[index_list].size = 0;
    lists[index_list].current_node = NULL;
    lists[index_list].current_state = LIST_OOB_START;
    lists[index_list].stack_tracker = end1;

    return &lists[index_list];
}

// Returns the number of items in pList.
int List_count(List* pList) {
    return pList->size;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if the list is empty.
void* List_first(List* pList) {
    if (pList == NULL || pList->size == 0) {
        return NULL;
    }
    // Set the current node and stack tracker to the head of the list
    pList->current_node = &nodes[pList->head];
    pList->stack_tracker = pList->head;
    return pList->current_node->data;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if the list is empty.
void* List_last(List* pList) {
    if (pList == NULL || pList->size == 0) {
        return NULL;
    }
    // Set the current node and stack tracker to the tail of the list
    pList->current_node = &nodes[pList->tail];
    pList->stack_tracker = pList->tail;
    return pList->current_node->data;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond the end of pList.
void* List_next(List* pList) {
    if (pList->initialized == false || (pList->current_node == NULL && pList->current_state == LIST_OOB_END)) {
        return NULL;
    }

    // If the current node is before the list
    if (pList->current_node == NULL && pList->current_state == LIST_OOB_START) {
        return List_first(pList);
    }

    // If the list has 1 item, or the current is tail
    if (pList->stack_tracker == pList->tail || pList->size == 1) {
        pList->stack_tracker = end2;
        pList->current_node = NULL;
        pList->current_state = LIST_OOB_END;
        return NULL;
    }

    // Move to the next node and update the current node and stack tracker
    pList->stack_tracker = nodes[pList->stack_tracker].next;
    pList->current_node = &nodes[pList->stack_tracker];
    return pList->current_node->data;
}

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {
    if (pList->initialized == false || (pList->current_node == NULL && pList->current_state == LIST_OOB_START)) {
        return NULL;
    }

    // If the current node is after the list
    if (pList->current_node == NULL && pList->current_state == LIST_OOB_END) {
        return List_last(pList);
    }

    // If the list has 1 item, or the current is head
    if (pList->stack_tracker == pList->head || pList->size == 1) {
        pList->stack_tracker = end1;
        pList->current_node = NULL;
        pList->current_state = LIST_OOB_START;
        return NULL;
    }

    // Move to the previous node and update the current node and stack tracker
    pList->stack_tracker = nodes[pList->stack_tracker].previous;
    pList->current_node = &nodes[pList->stack_tracker];
    return pList->current_node->data;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
    if (pList->initialized == false) {
        return NULL;
    }

    if ((pList->current_node == NULL && pList->current_state == LIST_OOB_START)) {
        return NULL;
    }

    if (pList->current_node == NULL && pList->current_state == LIST_OOB_END) {
        // Handle case where the current item is beyond the end of pList
        // This might happen after advancing beyond the last item in the list
        return NULL;
    }
    // Return the data of the current node
    return pList->current_node->data;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){

    if(pList->initialized == false){

        return -1;
    }
    
    // Get next free node from the stack
    int index_node = popFromStack();

    if(index_node == -1){

        return -1;
    }

    pList->current_node = &nodes[index_node];
    pList->current_node->initialized = true;
    pList->current_node->data = pItem;
    pList->size++;
    
    // Handling special case for an empty list
    if(pList->size == 1){

        pList->current_node->next = end2;
        pList->current_node->previous = end1;
        pList->head = index_node;
        pList->tail = index_node;
        pList->stack_tracker = pList->head;

        return 0;
    }

    // Adding item when the current pointer is before the list
    if(pList->stack_tracker == end1){

        pList->current_node->previous = end1;
        pList->current_node->next = pList->head;
        nodes[pList->head].previous = index_node;
        pList->head = index_node;
        pList->stack_tracker = pList->head;
        
        return 0;
    }

    // Adding item when the current pointer is beyond the list
    if(pList->stack_tracker == end2){

        pList->current_node->previous = pList->tail;
        pList->current_node->next = end2;
        nodes[pList->tail].next = index_node;
        pList->tail = index_node;
        pList->stack_tracker = pList->tail;
        
        return 0;
    }

    // Adding item when the list has 2 items or the current is the tail
    if(pList->size == 2 || pList->stack_tracker == pList->tail){
        
        pList->current_node->previous = pList->tail;
        pList->current_node->next = end2;
        nodes[pList->tail].next = index_node;
        pList->tail = index_node;
        pList->stack_tracker = pList->tail;
        
        return 0;
    }
    
    // Adding item when the current pointer is in the middle of the list
    pList->current_node->next = nodes[pList->stack_tracker].next;
    pList->current_node->previous = pList->stack_tracker;
    nodes[nodes[pList->stack_tracker].next].previous = index_node;
    nodes[pList->stack_tracker].next = index_node;
    pList->stack_tracker = index_node;

    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){

    if(pList->initialized == false){

        return -1;
    }
    
    // Get next free node from the stack
    int index_node = popFromStack();

    if(index_node == -1){

        return -1;
    }

    pList->current_node = &nodes[index_node];
    pList->current_node->initialized = true;
    pList->current_node->data = pItem;
    pList->size++;

    // Handling special case for an empty list
    if(pList->size == 1){

        pList->current_node->next = end2;
        pList->current_node->previous = end1;
        pList->head = index_node;
        pList->tail = index_node;
        pList->stack_tracker = pList->head;
    
        return 0;
    }

    // Adding item when the current pointer is before the list
    if(pList->stack_tracker == end1){ 

        pList->current_node->previous = end1;
        pList->current_node->next = pList->head;
        nodes[pList->head].previous = index_node;
        pList->head = index_node;
        pList->stack_tracker = pList->head;
      
        return 0;
    }

    // Adding item when the current pointer is beyond the list
    if(pList->stack_tracker == end2){

        pList->current_node->previous = pList->tail;
        pList->current_node->next = end2;
        nodes[pList->tail].next = index_node;
        pList->tail = index_node;
        pList->stack_tracker = pList->tail;
       
        return 0;
    }

    // Adding item when the list has 2 items or the current is the head
    if(pList->size == 2 || pList->stack_tracker == pList->head){
        
        pList->current_node->previous = end1;
        pList->current_node->next = pList->head;
        nodes[pList->head].previous = index_node;
        pList->head = index_node;
        pList->stack_tracker = pList->head;
      
        return 0;
    }
        
    // Adding item when the current pointer is in the middle of the list
    pList->current_node->previous = nodes[pList->stack_tracker].previous;
    pList->current_node->next = pList->stack_tracker;
    nodes[nodes[pList->stack_tracker].previous].next = index_node;
    nodes[pList->stack_tracker].previous = index_node;
    pList->stack_tracker = index_node;
    
    return 0;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){

    if(pList->initialized == false){

        return -1;
    }

    int index_node = popFromStack();

    if(index_node == -1){

        return -1;
    }

    pList->current_node = &nodes[index_node];
    pList->current_node->initialized = true;
    pList->current_node->data = pItem;
    pList->current_node->next = end2;
    pList->size++;

    // Handling special case for an empty list
    if(pList->size == 1){

        pList->current_node->previous = end1;
        pList->head = index_node;
        pList->tail = pList->head;
        pList->stack_tracker = pList->head;
        return 0;
    }
    
    pList->current_node->previous = pList->tail;
    nodes[pList->tail].next = index_node;
    pList->tail = index_node;
    pList->stack_tracker = pList->tail;
    return 0;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){

    if(pList->initialized == false){

        return -1;
    }

    int index_node = popFromStack();

    if(index_node == -1){

        return -1;
    }

    pList->current_node = &nodes[index_node];
    pList->current_node->initialized = true;
    pList->current_node->data = pItem;
    pList->current_node->previous = end1;
    pList->size++;

    // Handling special case for an empty list
    if(pList->size == 1){

        pList->current_node->next = end2;
        pList->head = index_node;
        pList->tail = pList->head;
        pList->stack_tracker = pList->head;
        
        return 0;
    }

    pList->current_node->next = pList->head;
    nodes[pList->head].previous = index_node;
    pList->head = index_node;
    pList->stack_tracker = pList->head;

    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){

    if(pList->size == 0 || pList->initialized == false){

        return NULL;
    }

    if(pList->current_node == NULL && pList->current_state == LIST_OOB_START){

        return NULL;
    }

    if(pList->current_node == NULL && pList->current_state == LIST_OOB_END){

        return NULL;
    }

    // Push new free node onto the stack
    pushToStack(pList->stack_tracker);
    pList->size--;
    void* ret = nodes[pList->stack_tracker].data;
    pList->current_node->initialized = false;

    // Handling special case for an empty list
    if(pList->size == 0){
        
        pList->current_node = NULL;
        pList->current_state = LIST_OOB_END;
        pList->head = end1;
        pList->tail = end2;
        pList->stack_tracker = pList->head;

        return ret;
    }

    // Remove item when the current pointer is the head
    if(pList->stack_tracker == pList->head){

        pList->head = nodes[pList->stack_tracker].next;
        nodes[pList->head].previous = end1;
        pList->stack_tracker = pList->head;
        
        return ret;
    }

    // Remove item when the current pointer is the tail
    if(pList->stack_tracker == pList->tail){
        
        pList->tail = nodes[pList->tail].previous;
        nodes[pList->tail].next = end2;
        pList->current_node = NULL;
        pList->current_state = LIST_OOB_END;
        pList->stack_tracker = end2;
        
        return ret;
    }

    // Remove item when the current pointer is in the middle of the list
    int temp_next = nodes[pList->stack_tracker].next;
    int temp_prev = nodes[pList->stack_tracker].previous;
    nodes[temp_prev].next = temp_next;
    nodes[temp_next].previous = temp_prev;
    pList->stack_tracker = temp_next;
    
    return ret;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){

    if(pList->size == 0 || pList->initialized == false){

        return NULL;
    }

    pushToStack(pList->tail);
    pList->size--;
    void* ret = nodes[pList->tail].data;

    // Handling special case for an empty list
    if(pList->size == 0){

        nodes[pList->stack_tracker].initialized = false;
        pList->current_node = NULL;
        pList->current_state = LIST_OOB_START;
        pList->stack_tracker = end1;
        return ret;
    }

    nodes[pList->tail].initialized = false;
    pList->stack_tracker = nodes[pList->tail].previous;
    pList->tail = pList->stack_tracker;
    nodes[pList->stack_tracker].next = end2;
    return ret;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){

    if((pList1->size == 0 && pList2->size == 0) || (pList1->size != 0 && pList2->size == 0)){

        pList2->initialized = false;
        return;
    }

    if(pList1->size == 0 && pList2->size != 0){

        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->size = pList2->size;
        pList2->initialized = false;
        return;
    }

    nodes[pList1->tail].next = pList2->head;
    nodes[pList2->head].previous = pList1->tail;
    pList1->tail = pList2->tail;
    pList1->size += pList2->size;
    pList2->initialized = false;
    return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exist after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){

    if(pList == NULL){

        return;
    }
    
    int curr = pList->head; // Start from the head of the list
    int nextNode;

    while (curr != end2) {

        if(nodes[curr].initialized == true){

            nextNode = nodes[curr].next;
            (*pItemFreeFn)(nodes[curr].data); 

            // Free item data and push the node back to stack
            pushToStack(curr);  

            // Set node back to uninitialized
            nodes[curr].initialized = false;
        } 
        else{

            // Iterate through the list
            nextNode = nodes[curr].next;
            curr = nextNode;
        }
    }    

    pList->size = 0;
    pList->initialized = false;

    return;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){

    if(pList == NULL || pList->initialized == false || pList->size == 0){

        return NULL;
    }
    
    // Start at the head if the current pointer is before the list
    if(pList->current_node == NULL && pList->current_state == LIST_OOB_START){

        List_first(pList);
    }

    int curr = pList->stack_tracker;
    
    // Scan through the whole list
    while(curr != end2){

        if(curr != end2){
            
            // If the items match, 1 will be returned
            int same = (*pComparator)(nodes[curr].data, pComparisonArg);
            pList->stack_tracker = curr;

            if(same == 1){

                return nodes[curr].data;
            }
        }

        curr = nodes[curr].next;
    }   

    pList->stack_tracker = end2;
    pList->current_node = NULL;
    pList->current_state = LIST_OOB_END;
    return NULL;
}

