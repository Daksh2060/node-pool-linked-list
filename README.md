# Node Pool Linked List

This C project is a unique implementation of the linked list data structure. As the name implies, this linked list implementation makes use of a pre-allocated pool of memory to create both a set of list heads and nodes. For example, the head set allows for 10 heads while the node set allows for 100 nodes by default, this enables a maximum of 10 lists with a maximum of 100 nodes total (these values can be adjusted). This linked list was created with speed and memory efficiency as a focus, with the intent of being used in other programs where these factors may be of high importance, such as programs involving kernel level processes.

## Features

- **Dynamic Type:** The list holds data of any type, making use of void pointers and user-created comparison functions to allow custom data types.

- **Memory Efficient:** The only memory used is static memory for the node pools. The avoidance of system calls in the program allows it to run fast and efficiently, notable for projects that involve multithreaded processes.

- **Current Item:** An additional pointer allows users to keep track of a "current" item within the linked list, allowing for controllable iteration or addition/deletion from the list.

- **Constant Time Operation:** Most operations of the linked list, such as adding a node to the front, end, or next to the current pointer are done in constant time.

## How does it work?

To achieve a constant runtime `O(N)`, the program relies heavily on pointer manipulation. The struct used to create the nodes for the list holds pointers to both the next and previous nodes in its list. The list itself holds many meta pointers such as the `current` pointer, as well as the enum that tracks the state of the current pointer, such as if it is out of bounds. It also keeps track of the `front` and `end` of the list this way. Both node and list structs contain an `initialized` bool to ensure the same node or list cannot be overwritten. Since nodes are added in constant time and are taken from a finite pool of nodes, a stack is used to keep track of which nodes are free to be used, and are readded to this stack when they are made available.

![Two Set System](images/Sets.png)

This diagram shows the two sets of nodes and lists. The node set is held in a stack, ensuring only unoccupied nodes are used when adding to a specific list. Lists can be concatenated to free a list head and allow the creation of a new list. The maximum size of both the head and node pools may be adjusted as needed. When a node is in use, it is designated as such by a bool flag and is taken off the stack of the free node pool. When a node is no longer in use, either because the item it held was freed by the user or the list it was held in was cleared, the node's flag is set to free and it is added to the top of the stack, ensuring no unavailable nodes can be overwritten. This also allows us to add nodes to the list in `O(N)` time, as we do not need to search for free nodes, and can just pop one off the top of the stack.

## Installation and Use

Follow these steps to set up and run the Node Pool Linked List:

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/Daksh2060/node-pool-linked-list-c
   ```

2. To run the included test file, use the makefile:

   ```bash
   make test
   ./test
   ```

3. To include in your program, include the header in your C file:

   ```bash
   #include "list.h"
   ```

4. To adjust the maximum number of lists and nodes, adjust the corresponding values in the header:

   ```bash
   #define LIST_MAX_NUM_HEADS 10
   #define LIST_MAX_NUM_NODES 100
   ```

5. Initiate a list under main:

   ```bash
   List* list1 = List_create();
   ```

6. Use list functions to manipulate the list (see header for all operations):

   ```bash
   List_append(list1, "Hello");
   ```

7. To search a list, you must include a comparison helper function, for example (using ints):

   ```bash
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
   ```
   To use the search the list for an item, in this case, the string `Hello`:

   ```bash
   List_search(list1, pComparator_int, "Hello");
   ```

8. To delete a list, must provide a helper function:

   If using static memory to create items:

   ```bash
   void pItemFreeFn_static(void* pItem){
     if(pItem)
         pItem = NULL;
   }
   ```

   If using allocated memory to create items:

   ```bash
   void pItemFreeFn_allocated(void* pItem){
      free(pItem);
   }
   ```

   To delete a list, in this case, one with static items:

   ```bash
   List_free(list1, pItemFreeFn_static);
   ```

## Contact

Feel free to reach out if you have any questions, suggestions, or feedback:

- **Email:** dpa45@sfu.ca
- **LinkedIn:** [@Daksh Patel](https://www.linkedin.com/in/daksh-patel-956622290/)
