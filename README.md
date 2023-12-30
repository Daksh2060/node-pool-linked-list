# Node Pool Linked List

This C project is a unique implementation of the linked list data structure. As the name implies, this linked list implementation makes use of a pre-allocated pool of memory to create both a set of list heads and nodes. For example, the head set allows for 10 heads while the node set allows for 100 nodes by default, this enables a maximum of 10 lists with a maximum of 100 nodes total (these values can be adjusted). This linked list was created with speed and memory efficiency as a focus, with the intent of being used in other programs where these factors may be of high importance. 

## Features

- **Dynamic Type:** The list holds data of any type, making use of void pointers and user-created comparison functions to allow custom data types.

- **Memory Efficient:** The only memory used is static memory for the node pools. The avoidance of system calls in the program allows it to run fast and efficiently, notable for projects that involve multithreaded processes.

- **Current Item:** An additional pointer allows users to keep track of a "current" item within the linked list, allowing for controllable iteration or addition/deletion from the list.

- **Constant Time Operation:** Most operations of the linked list, such as adding a node to the front, end, or next to the current pointer are done in constant time.

## How does it work?

To achieve a constant runtime (`O(N)`), the program relies heavily on pointer manipulation. The struct use to create the nodes for the list hold pointers to both the next and previous nodes in its own list. The list itself holds many meta pointers such as the `current` pointer, as well as the an enum that tracks the state of the current pointer, such as if it out of bounds. It also keeps track of the `front` and `end` of the list this way. Both node and list structs contain an `initialized` bool to ensrue the same node or list cannot be overwritten. Since nodes are added in constant time, and are taken from a finite pool of nodes, a stack is used to keep track of which nodes are free to be used, and are readded to this stack when they are made avaiable.

![](images/.png)

This figure shows

![](images/.png)

## Installation and Use

Follow these steps to set up and run the Node Pool Linked List:

1. Clone the repository to your local machine:

   ```bash
   git clone
   ```

## Contact

Feel free to reach out if you have any questions, suggestions, or feedback:

- **Email:** dpa45@sfu.ca
- **LinkedIn:** [@Daksh Patel](https://www.linkedin.com/in/daksh-patel-956622290/)