# Node Pool Linked List

This C project is a unique implenmenation of the linked list data strcutre. As the name implies, this linked list implemntation makes use of a pre-allocated pool of memeory to create both a set of list-heads, and nodes. For example, by default the head set allows for 10 heads while the node set allows for 100 nodes, this enables a maximum of 10 lists with a maximum of 100 nodes total (these value can be adjusted). This linked list was created with speed and memory effieicny as a focus, with the intent of being used in other programs where these factors may be of high importance. 

## Features

- **Dynamic Type:** The list holds data of any type, making use of void pointers and user created comparison functions to allow custom data types.

- **Memory Efficent:** The only memeory used is static memeory for the node pools. The avoidance of system calls in the program allows it to run fast and effiectnly, notable for projects which involve multithreaded proccesses.

- **Current Item:** An additioonal pointer allows users to keep track of a "current" item within the linked list, allowing for controllable iteration or addition/deletion from the list.

- **Constant Time Operation:** Most operations of the linked list, such as adding a node to the front, end, or next to the current pointer is done in constant time

## How does it work?

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
