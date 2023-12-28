#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.c"

//PLEASE NOTE: The printing was done to confirm the outputs, not as a part of the testing

//Helper function used to print out a list of ints
void printList(List* pList){

    if(pList == NULL || pList->initialized == false){

        printf("List is not initialized\n");
        return;
    }

    if(pList->size == 0){

        printf("List is empty\n");
        return;
    }
    
    printf("List Size: %d\n", pList->size);
    printf("List Contents:\n");
    int cur = pList->head;

    while(cur != end2){

        if(nodes[cur].initialized == true){

            printf("| %d ", *(int*)nodes[cur].data);
      
            if(nodes[cur].next != end2){

                printf("-> ");
            }    
        }

        cur = nodes[cur].next;
    }

    printf("|\n\n");
}

//Helper function used to print out a list of strings
void printList_strings(List* pList) {

    if(pList == NULL || pList->initialized == 0){

        printf("List is not initialized\n");
        return;
    }

    if(pList->size == 0){

        printf("List is empty\n");
        return;
    }

    printf("List Size: %d\n", pList->size);
    printf("List Contents:\n");
    int cur = pList->head;

    while(cur != end2){

        if (nodes[cur].initialized){

            printf("| %s ", (char*)nodes[cur].data);

            if(nodes[cur].next != end2){

                printf("-> ");
            }
        }

        cur = nodes[cur].next;
    }

    printf("|\n\n");
}

//User functions used to free either dynamic memory or static
void pItemFreeFn_allocated(void* pItem){

    free(pItem);
}

//User functions used to free either dynamic memory or static
void pItemFreeFn_static(void* pItem){

    if(pItem)
    pItem = NULL;
}


//User functions used to compare ints or strings
bool pComparator_int(void* pItem, void* pComparisonArg){

    int item = *((int*)pItem);
    int comparisonArg = *((int*)pComparisonArg);
    
    if(item == comparisonArg){

        return 1;
    }
    else{
        
        return 0;
    }
}


// User function used to compare strings
bool pComparator_string(void* pItem, void* pComparisonArg){
    char* item = (char*)pItem;
    char* comparisonArg = (char*)pComparisonArg;

    if(strcmp(item, comparisonArg) == 0){

        return true;
    } 
    else{

        return false;
    }
}


int main(){    

    //Will start by testing with dynamically allocated memory for items
    
    //Can only make 10 lists, last 2 should return NULL;
    //Test: List_create()
    List* lists_holder[LIST_MAX_NUM_HEADS+2];

    for(int i = 0; i < LIST_MAX_NUM_HEADS+2; i++){

        lists_holder[i] = List_create();

        if(!lists_holder[i]){

            printf("list%d is NULL.\n", i + 1);
        }
    }



    //Can have a total of 100 nodes, so all 10 lists can each have 10 nodes, or other possible combos.
    //Test: List_append()
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){

        for (int j = i * 10; j < (i + 1) * 10; j++){

            int* value = (int*)malloc(sizeof(int));
            *value = j + 1;
            int check = List_append(lists_holder[i], value);
            
            if(check == -1){

                printf("Failed to add node");
                free(value);
            }
        }
    }

    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){

        printList(lists_holder[i]);
    }



    //Since all 100 nodes are currently taken, these 5 extra values will not be added
    //Test: List_append()
    for(int i = 0; i < 5; i++){
    
        int* value = (int*)malloc(sizeof(int));
        *value = i + 1;
        int check = List_append(lists_holder[i], value);

        if(check == -1){

            printf("Failed to add node\n");
            free(value);
        }
    }



    //Following code will print last and first values of each list, for non empty lists of size > 1;
    //Test: List_last() and List_first()
    for(int i = 0; i < 10; i++){
        
        printf("List%d:\n",i+1);
        printf("Last item: %d\n", *((int *)List_last(lists_holder[i])));
        printf("First item: %d\n", *((int *)List_first(lists_holder[i])));
        printf("\n");
    }
    


    //All list current items should now point to the first item in each, so we can print the second item:
    //Test: List_next()
    for(int i = 0; i < 10; i++){
        
        printf("List%d:\n",i+1);
        printf("Second item: %d\n", *((int*)List_next(lists_holder[i])));
        printf("\n");
    }
    
    
    
    //All list current items should now point to the second item in each, so we can print the first item again:
    //Test: List_prev()
    for(int i = 0; i < 10; i++){
        
        printf("List%d:\n",i+1);
        printf("First item again: %d\n", *((int *)List_prev(lists_holder[i])));
        printf("\n");
    }

    printList(lists_holder[0]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[0])));



    //For further testing, List1 will only contain 1 item, and list 2 will be freed:
    //Test: List_trim() and List_free()
    for(int i = 0; i < 9; i++){

        free(List_trim(lists_holder[0]));
        printList(lists_holder[0]);
    }

    printList(lists_holder[0]);
    List_free(lists_holder[1], pItemFreeFn_allocated);

    lists_holder[1] = List_create();
    printList(lists_holder[1]);

    

    //Will test single item conditions with List1, should all be "1":
    //Tests: Multiple, using single item case
    printf("List1 Count: %d\n",List_count(lists_holder[0]));
    printf("List1 current: %d\n",*((int *)List_curr(lists_holder[0])));
    printf("List1 first: %d\n",*((int *)List_first(lists_holder[0])));
    printf("List1 last: %d\n",*((int *)List_last(lists_holder[0])));



    //Will test the out of bounds behavior with a list of size 1:
    //Tests: List_next() and List_previous() with 1 item
    if(List_prev(lists_holder[0]) == NULL){

        printf("Current Item before front\n");
    }

    if(List_next(lists_holder[0]) != NULL){

        printf("List current: %d\n",*((int *)List_curr(lists_holder[0])));
    }

    if(List_next(lists_holder[0]) == NULL){

        printf("Current Item after end\n");
    }

    printList(lists_holder[0]);



    //Will examine the behavior of reassigning nodes to freed lists:
    //Tests: List_append(), List_prepend(), and List_remove()
    for(int i = 0; i < 13 ; i++){

        int* value = (int*)malloc(sizeof(int));
        *value = i + 1;
        int check = List_prepend(lists_holder[1], value);

        if(check == -1){

            printf("Failed to add node");
            free(value);
        }
    }
    
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    printList(lists_holder[1]);

    //Should only remove 1 item
    List_last(lists_holder[1]);
    printf("List current (last item): %d\n",*((int *)List_curr(lists_holder[1])));
    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    printList(lists_holder[1]);

    //Only have 12 nodes left, so will try to overload the list
    for(int i = 0; i < 15 ; i++){

        int* value = (int*)malloc(sizeof(int));
        *value = i + 1;
        int check = List_append(lists_holder[1], value);

        if(check == -1){

            printf("Failed to add node\n");
            free(value);
        }
    }
    
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    List_prev(lists_holder[1]);
    List_prev(lists_holder[1]);
    List_prev(lists_holder[1]);
    List_prev(lists_holder[1]);
    printf("New List current: %d\n",*((int *)List_curr(lists_holder[1])));

    free(List_remove(lists_holder[1]));
    free(List_remove(lists_holder[1]));
    printList(lists_holder[1]);
    printf("New List current: %d\n",*((int *)List_curr(lists_holder[1])));
    
    //Removing items with List_trim:
    printList(lists_holder[9]);
    printf("List9 Current: %d\n",*((int *)List_curr(lists_holder[9])));
    
    for(int i = 0; i < 5; i++){

        void* item = List_trim(lists_holder[9]);

        if(item != NULL){
            printf("Item trimmed: %d\n",*((int *)item));
            printf("List9 Current: %d\n",*((int *)List_curr(lists_holder[9])));
            printList(lists_holder[9]);
            free(item);
        }
        else{
            printf("No item trimmed\n");
        } 
    }

    //Removing an entire list with overcount through List_trim
    List_first(lists_holder[9]);

    for(int i = 0; i < 4; i++){

        void* item = List_trim(lists_holder[9]);

        if(item != NULL){
            printf("Item trimmed: %d\n",*((int *)item));
            printf("List9 Current: %d\n",*((int *)List_curr(lists_holder[9])));
            printList(lists_holder[9]);
            free(item);
        }
        else{
            printf("No item trimmed\n");
        }
    }

    printf("List9 Count: %d\n",List_count(lists_holder[9]));
    free(List_trim(lists_holder[9]));
    printf("List9 Count: %d\n",List_count(lists_holder[9]));


    
    //Will test the concatination of lists, with filled and empty lists
    //Tests List_concat()
    printf("\nTesting List Concatination: \n\n");

    //Adding an empty list to end of a non-empty list
    List_first(lists_holder[0]);
    free(List_remove(lists_holder[0]));
    printList(lists_holder[0]);
    printList(lists_holder[1]);
    List_concat(lists_holder[1], lists_holder[0]);
    printList(lists_holder[1]);
    printList(lists_holder[0]);
    printf("List2 current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Adding 2 non-empty lists
    List_concat(lists_holder[1], lists_holder[5]);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
    printf("List first: %d\n",*((int *)List_first(lists_holder[1])));
    printf("List last: %d\n",*((int *)List_last(lists_holder[1])));

    //Adding 2 empty lists:
    lists_holder[0] = List_create();
    lists_holder[5] = List_create();
    List_concat(lists_holder[0], lists_holder[5]);
    printList(lists_holder[0]);
    printList(lists_holder[5]);

    //Adding non-empty list to end of an empty list
    List_concat(lists_holder[0], lists_holder[1]);
    printList(lists_holder[0]);
    printList(lists_holder[1]);
    printf("List1 first: %d\n",*((int *)List_first(lists_holder[0])));
    printf("List1 last: %d\n",*((int *)List_last(lists_holder[0])));



    //List seraching will be tested, on non-empty, empty and unintiallized lists
    //Tests: List_search()
    printf("\n\nSearch Testing\n\n");
    List_concat(lists_holder[0], lists_holder[7]);
    printList(lists_holder[0]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[0])));

    //Search item that is in list and after current value
    int search = 75;
    if(List_search(lists_holder[0], pComparator_int, &search) != NULL){

        printf("Found: %d\n",*((int *)List_curr(lists_holder[0])));
    }
    else{

        printf("Did not find %d\n",search);
    }

    printf("List current: %d\n",*((int *)List_curr(lists_holder[0])));

    //Search item that is in list and before current value
    search = 4;
    if(List_search(lists_holder[0], pComparator_int, &search) != NULL){

        printf("Found: %d\n",*((int *)List_curr(lists_holder[0])));
    }
    else{

        printf("Did not find %d\n",search);
    }
    
    //Search item from start that is not in list
    List_first(lists_holder[0]);
    search = 91;

    if(List_search(lists_holder[0], pComparator_int, &search) != NULL){

        printf("Found: %d\n",*((int *)List_curr(lists_holder[0])));
    }
    else{

        printf("Did not find %d\n",search);
    }

    //Search item from start that is at the start
    List_first(lists_holder[0]);
    search = 8;

    if(List_search(lists_holder[0], pComparator_int, &search) != NULL){

        printf("Found: %d\n",*((int *)List_curr(lists_holder[0])));
    }
    else{

        printf("Did not find %d\n",search);
    }

    //Search item from start that is at the end
    List_first(lists_holder[0]);
    search = 80;

    if(List_search(lists_holder[0], pComparator_int, &search) != NULL){

        printf("Found: %d\n",*((int *)List_curr(lists_holder[0])));
    }
    else{

        printf("Did not find %d\n",search);
    }

    printf("List current: %d\n",*((int *)List_curr(lists_holder[0])));

    

    //Will test inserting before on empty and non empty lists, and full list;
    //Tests: List_insert_before()
    List_free(lists_holder[9], pItemFreeFn_allocated);
    lists_holder[1] = List_create();
    printList(lists_holder[1]);

    //Insert before on an empty list
    int* test_before_1 = (int*)malloc(sizeof(int));
    *test_before_1 = 21;
    List_insert_before(lists_holder[1], test_before_1);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Insert before the head of a list
    int* test_before_2 = (int*)malloc(sizeof(int));
    *test_before_2 = 22;
    List_insert_before(lists_holder[1], test_before_2);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
    List_prev(lists_holder[1]);

    //Insert Before when current is before list
    int* test_before_3 = (int*)malloc(sizeof(int));
    *test_before_3 = 23;
    List_insert_before(lists_holder[1], test_before_3);
    printList(lists_holder[1]);
    
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
    List_next(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
    
    //Insert before an item in the middle of the list
    int* test_before_4 = (int*)malloc(sizeof(int));
    *test_before_4 = 24;
    List_insert_before(lists_holder[1], test_before_4);
    printList(lists_holder[1]);

    List_next(lists_holder[1]);
    List_next(lists_holder[1]);
    List_next(lists_holder[1]);

    //Insert Before when current is after end of list:
    int* test_before_5 = (int*)malloc(sizeof(int));
    *test_before_5 = 25;
    List_insert_before(lists_holder[1], test_before_5);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Check Head and Tail
    printf("First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("Last item: %d\n", *((int *)List_last(lists_holder[1])));



    //Will test inserting after on empty and non empty lists, and full list;
    //Tests: List_insert_after()
    List_free(lists_holder[1], pItemFreeFn_allocated);
    lists_holder[1] = List_create();

    //Insert after on an empty list
    int* test_after_1 = (int*)malloc(sizeof(int));
    *test_after_1 = 31;
    List_insert_after(lists_holder[1], test_after_1);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Insert after the head of the list
    int* test_after_2 = (int*)malloc(sizeof(int));
    *test_after_2 = 32;
    List_insert_after(lists_holder[1], test_after_2);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Insert after when the current is before the list
    List_prev(lists_holder[1]);
    List_prev(lists_holder[1]);
    int* test_after_3 = (int*)malloc(sizeof(int));
    *test_after_3 = 33;
    List_insert_after(lists_holder[1], test_after_3);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Insert before an item in the middle of the list
    List_next(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
    int* test_after_4 = (int*)malloc(sizeof(int));
    *test_after_4 = 34;
    List_insert_after(lists_holder[1], test_after_4);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));

    //Insert after when the current item is beyond end of the list
    List_next(lists_holder[1]);
    List_next(lists_holder[1]);
    int* test_after_5 = (int*)malloc(sizeof(int));
    *test_after_5 = 35;
    List_insert_after(lists_holder[1], test_after_5);
    printList(lists_holder[1]);
    printf("List current: %d\n",*((int *)List_curr(lists_holder[1])));
   
    //Check Head and Tail
    printf("First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("Last item: %d\n", *((int *)List_last(lists_holder[1])));



    //Clear all dynamically allocated lists:
    //Tests: List_free() and pItemFreeFn_allocated()
    for(int i = 0; i< 10; i++){

        List_free(lists_holder[i], pItemFreeFn_allocated);
    }

    

    //Will now test using statically generated ints (primarily memory related):

    //Will create 4 lists of 25 items, each statically generated + 5 overload
    int static_values[LIST_MAX_NUM_NODES+5];

    for(int i = 0; i < LIST_MAX_NUM_NODES+5; i++){

        static_values[i] = i + 1; 
    }

    for(int i = 0; i < 4; i++){

        lists_holder[i] = List_create();

        for(int j = i*25; j < (i+1)*25; j++){

            int* value = &static_values[j];
            int result = List_append(lists_holder[i], value);

            if(result == -1){

                printf("Failed to add item to list\n");
            }
        }
    }

    //Ensure overload isnt possible by adding 5 surplus items
    for(int i = 100; i < 105; i++){

        int* value = &static_values[i];
        int result = List_append(lists_holder[1], value);
            
        if(result == -1){

            printf("Failed to add item to list\n");
        }
    }
    
    for(int i = 0; i < 4; i++){

        printList(lists_holder[i]);
    }
    
    printf("Check\n");
    


    //Check all pointers of non-empty and single item list
    //Tests: List_first(), List_last(), List_next, List_prev(), and List_trim()
    
    printf("List1 First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("List1 Last item: %d\n", *((int *)List_last(lists_holder[1])));
    printf("List1 Current item: %d\n", *((int *)List_curr(lists_holder[1])));

    for(int i = 0 ; i< 23; i++){
        List_trim(lists_holder[1]);
    }
    printList(lists_holder[1]);
    printf("List1 First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("List1 Last item: %d\n", *((int *)List_last(lists_holder[1])));
    printf("List1 Current item: %d\n", *((int *)List_curr(lists_holder[1])));

    //Moving to ends of list and printing
    List_next(lists_holder[1]);
    List_next(lists_holder[1]);
    List_prev(lists_holder[1]);
    printf("List1 Current item: %d\n", *((int *)List_curr(lists_holder[1])));
    List_prev(lists_holder[1]);
    List_prev(lists_holder[1]);
    List_next(lists_holder[1]);
    printf("List1 Current item: %d\n", *((int *)List_curr(lists_holder[1])));

    //Testing points with single item:
    List_trim(lists_holder[1]);
    printList(lists_holder[1]);
    printf("List1 First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("List1 Last item: %d\n", *((int *)List_last(lists_holder[1])));
    printf("List1 Current item: %d\n", *((int *)List_curr(lists_holder[1])));

    List_free(lists_holder[1],pItemFreeFn_static);
    lists_holder[1] = List_create();
    printList(lists_holder[1]);



    //Testing adding items to the list:
    //Tests: List_append(), List_prepend, List_insert_before() and List_insert_after()

    //Adding to a list with append/prepend
    int * value1 = &static_values[0];
    List_append(lists_holder[1], value1);
    printList(lists_holder[1]);
    List_trim(lists_holder[1]);

    int * value2 = &static_values[1];
    List_prepend(lists_holder[1], value2);
    printList(lists_holder[1]);

    int * value3 = &static_values[2];
    List_append(lists_holder[1], value3);
    printList(lists_holder[1]);
    List_trim(lists_holder[1]);

    int * value4 = &static_values[3];
    List_prepend(lists_holder[1], value4);
    printList(lists_holder[1]);
    
    int * value5 = &static_values[4];
    List_append(lists_holder[1], value5);
    printList(lists_holder[1]);
    printf("First item: %d\n", *((int *)List_first(lists_holder[1])));
    printf("Last item: %d\n", *((int *)List_last(lists_holder[1])));
    printf("Current item: %d\n", *((int *)List_curr(lists_holder[1])));

    //Adding Items to list using Insert_before
    List_free(lists_holder[1], pItemFreeFn_static);
    lists_holder[1] = List_create();

    for(int i = 0; i < 4; i++){
        printList(lists_holder[i]);
    }
    
    //Insert before into an empty list
    int * insert_val1 = &static_values[90];
    List_insert_before(lists_holder[1], insert_val1);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    printf("\n");

    //Insert before the head
    int * insert_val2 = &static_values[91];
    List_insert_before(lists_holder[1], insert_val2);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_prev(lists_holder[1]);
    printf("\n");

    //Insert before when current is before list
    int * insert_val3 = &static_values[92];
    List_insert_before(lists_holder[1], insert_val3);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_next(lists_holder[1]);
    printf("\n");

    //Insert before item in middle of list
    int * insert_val4 = &static_values[93];
    List_insert_before(lists_holder[1], insert_val4);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));

    //Insert before when current is after end of list
    List_next(lists_holder[1]);
    List_next(lists_holder[1]);
    List_next(lists_holder[1]);
    
    int * insert_val5 = &static_values[94];
    List_insert_before(lists_holder[1], insert_val5);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    printf("\n");

    List_free(lists_holder[1], pItemFreeFn_static);
    lists_holder[1] = List_create();
    printList(lists_holder[1]);

    //Insert after into an empty list
    int * insert_after1 = &static_values[81];
    List_insert_after(lists_holder[1], insert_after1);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_prev(lists_holder[1]);
    printf("\n");
    
    //Insert after when current is before the head
    int * insert_after2 = &static_values[82];
    List_insert_after(lists_holder[1], insert_after2);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    printf("\n");

    //Insert after the head
    int * insert_after3 = &static_values[83];
    List_insert_after(lists_holder[1], insert_after3);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_next(lists_holder[1]);
    printf("\n");

    //Insert after tail
    int * insert_after4 = &static_values[84];
    List_insert_after(lists_holder[1], insert_after4);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_next(lists_holder[1]);

    //Insert after when current is after end of list
    int * insert_after5 = &static_values[85];
    List_insert_after(lists_holder[1], insert_after5);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    printf("\n");



    //Testing list concating on empty and non empty static lists:
    //Tests: List_concat()
    List_free(lists_holder[0], pItemFreeFn_static);
    lists_holder[0] = List_create();
    printList(lists_holder[0]);
    printList(lists_holder[1]);
    printList(lists_holder[2]);

    //Connect a empty list to the end of a non-empty list:
    printf("\nEmpty list to the end of a non-empty list\n");
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    List_concat(lists_holder[1],lists_holder[0]);
    printList(lists_holder[0]);
    printList(lists_holder[1]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[1])));
    lists_holder[0] = List_create();

    //Conenct a non-empty list to the end of a empty list:
    printf("\nNon-empty list to the end of a empty list\n");
    printList(lists_holder[0]);
    printList(lists_holder[1]);
    List_concat(lists_holder[0],lists_holder[1]);
    
    //Connect 2 non-empty strings
    printf("\nNon-empty list to the end of a non-empty list\n");
    printList(lists_holder[0]);
    printList(lists_holder[2]);
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[2])));
    List_concat(lists_holder[2],lists_holder[0]);
    printList(lists_holder[0]);
    printList(lists_holder[2]);
    printf("Current item: %d\n", *((int *)List_curr(lists_holder[2])));
    printf("First item: %d\n", *((int *)List_first(lists_holder[2])));
    printf("Last item: %d\n", *((int *)List_last(lists_holder[2])));
    List_prev(lists_holder[2]);
    List_prev(lists_holder[2]);
    List_prev(lists_holder[2]);
    List_prev(lists_holder[2]);
    List_prev(lists_holder[2]);
    


    //Testing the serach feature on newly created list:
    //Tests: List_search()

    //Search item that is in list and after current value
    printList(lists_holder[2]);
    printf("Current item: %d\n", *((int *)List_curr(lists_holder[2])));
    int search_static = 83;
    if(List_search(lists_holder[2], pComparator_int, &search_static) != NULL){
        printf("Found: %d\n",*((int *)List_curr(lists_holder[2])));
    }
    else{
        printf("Did not find %d\n",search_static);
    }

    //Search an item not in the item after cur:
    search_static = 91;
    if(List_search(lists_holder[2], pComparator_int, &search_static) != NULL){
        printf("Found: %d\n",*((int *)List_curr(lists_holder[2])));
    }
    else{
        printf("Did not find %d\n",search_static);
    }
    List_first(lists_holder[2]);
    List_next(lists_holder[2]);
    List_next(lists_holder[2]);

    //Search for an item that is before current:
    search_static = 52;
    printf("Current item List1: %d\n", *((int *)List_curr(lists_holder[2])));
    if(List_search(lists_holder[2], pComparator_int, &search_static) != NULL){
        printf("Found: %d\n",*((int *)List_curr(lists_holder[2])));
    }
    else{
        printf("Did not find %d\n",search_static);
    }

    //Free all static item lists
    for(int i = 0; i< 10; i++){
        List_free(lists_holder[i], pItemFreeFn_static);
    }



    //Some simple tests to ensure data type works with strings:
    
    char *fruits[10] ={
        "Apple", "Banana", "Orange", "Grapes", "Strawberry",
        "Watermelon", "Pineapple", "Mango", "Cherry", "Blueberry"};

    // Display the array of fruits
    printf("Fruits:\n");
    for (int i = 0; i < 10; ++i){
        printf("%s\n", fruits[i]);
    }

    lists_holder[0] = List_create();
    lists_holder[1] = List_create();

    for(int i = 0; i < 2; i++){

        for(int j = i * 2; j < (i + 1) * 5; j++){

            int check = List_append(lists_holder[i], fruits[j]);
            
            if(check == -1){

                printf("Failed to add node");

            }
        }
    }

    printList_strings(lists_holder[0]);
    printList_strings(lists_holder[1]);



    //Quick tests:
    //Tests: List_prepend();
    List_prepend(lists_holder[0], fruits[7]);
    printList_strings(lists_holder[0]);
    printf("Current Item: %s\n",((char *)List_curr(lists_holder[0])));



    //Tests: List_next(), List_prev();
    List_next(lists_holder[0]);
    List_next(lists_holder[0]);
    List_next(lists_holder[0]);
    printf("Current Item: %s\n",((char *)List_curr(lists_holder[0])));
    List_prev(lists_holder[0]);
    printf("Current Item: %s\n",((char *)List_curr(lists_holder[0])));



    //Tests: List_search()
    char* search_word = "Grapes";
    if(List_search(lists_holder[0], pComparator_string, search_word) != NULL){
        printf("Found: %s\n", (char *)List_curr(lists_holder[0]));
    }
    else{
        printf("Did not find %s\n",search_word);
    }
    printf("Current Item: %s\n",((char *)List_curr(lists_holder[0])));

    char* search_word2 = "Apple";
    if(List_search(lists_holder[0], pComparator_string, search_word2) != NULL){
        printf("Found: %s\n", (char *)List_curr(lists_holder[0]));
    }
    else{
        printf("Did not find %s\n",search_word2);
    }
    


    //Tests: List_concat()
    printList_strings(lists_holder[0]);
    List_last(lists_holder[0]);
    printf("List 1 Current Item: %s\n",((char *)List_curr(lists_holder[0])));
    printList_strings(lists_holder[1]);
    List_concat(lists_holder[0], lists_holder[1]);
    printList_strings(lists_holder[0]);
    printf("List 1 Current Item: %s\n",((char *)List_curr(lists_holder[0])));



    //Free all static item lists
    for(int i = 0; i< 10; i++){
        List_free(lists_holder[i], pItemFreeFn_static);
    }

    //Ensure all lists have been freed before ending
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        printList(lists_holder[i]);
    }
    
    return 0;
}