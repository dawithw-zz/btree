#B-Tree

This is an implementation of a B-Tree for learning purposes.

Code organization:
The code is divided into a header file, used for most declarations, and a source file, where most of the implementation/function definitions are included. A make file provided with the source files can compile the code with a client file named “bTree_main.cpp.”

Implementation:
This bTree is implemented in an abstract way, where the data stored in the bTree is of type Item (described later in this document), and organized into Nodes(described later in this document). Since the Item class can be modified to contain any abstract/basic data types, this bTree can then be used with any other class by simply modifying the Items class. Note that, some of the functions such as input, delete and search take in strings (which are the expected data types that make up the Item class), therefore these need to be changed if to be used with a different class. Additionally, the Item class to be used by the bTree must have a defined way of comparing (equality and less than operations) two Items.

The Classes:
- Item: This is a very simple structure that has two variables for storing data, both of which are strings. It has a basic constructor, a modified constructor that takes in two inputs that are used to instantiate these variables, and two overloaded operators (== and < ) that can use used by anyone trying to compare Items. Comparison is done solely by comparing keys.
- Node: This structure makes up the basic building block of the bTree. It stores a size variable, which determines how many items the node can hold, a boolean variable that determines if the node is a leaf node in the tree, a pointer to the parent node, a list of Items (stored in vector), and a list of pointers to other nodes (stored in vector) – which are the children of the node. The Node will always store n+1 children, where n = number of items. It has a basic constructor, as well as an overloaded constructor that takes in an integer to initialize the size.
- bTree: This class defines all the operations that can be operated on the Nodes that make the tree. The class only stores on variable, a root Node structure, which point to the rest of the tree by extension. All operation on Nodes is delegated to the private functions of the bTree, and the public functions are only a means of converting the user input from some data type to Items that are moved in and out of Nodes. Therefore, the client has no direct control on the organization of the Nodes in the bTree – automatic management. The bTree supports four main operations – insert a key-pair value, delete a key (also deletes the value associated), find a key and returns the value associated with a key, and print the contents of the tree. The implementation is further explained in the Code section of this document.

The Code :
> Note: The code for the Items class and the Node class are not explained here, because everything other than what was described in the classes section of this document is self explanatory.
- Public functions:
  - Insert: takes a string key-value pair, creates an Item using the pair and throws it at the private insert function that adds it to a Node. The Node is determined by calling the private findNode function. Adjusts the node after.
  - Find: takes a string key, and uses the key to find the node where the key belongs. If an Item with that key exists, it returns true.
  - Delete: takes a string key, and similar to Insert, finds the node where the deletion is to take place. It finds the item with that key and removes the item by simply shifting all items and children in the node down, replacing the item in the process, or by simply just popping the item form the end. It adjusts the node accordingly after deletion.
  - To String: calls a recursive function on the root node – writeTree. 
- Private functions:
  - Insert: gets the item and node where the item belongs, and inserts the item in the node by simply shifting all items above the index where this item belongs, and inserting it in that space.
  - Adjust node: this is one of the most fundamental functions of the bTree that determines the organization of the nodes. It proceeds as follows:
  If called on an empty node, it doesn’t do anything
  If there is no overflow (more items in node that allotted), or undeflow
(items less than the minimum degree of the node), then nothing happens.
  If there is overflow
    - Check if the left sibling is not full, then shift over items to the left sibling
    - If above fails, try the same with the right sibling
    - If above fails, then the node should be split. Call split and adjust
the parent of the node
  If there is undeflow
    - Checks if the left sibling of the node has enough elements it can share, and shifts them over. The shift is explained in slide
    - Above fails, then try the same with the right sibling
    - Above fails, then maybe it’s the root node (no sibling). If so, and if
the root happens to have only one child, then move the child up as the root
    - Above condition fails, then it means the node can’t borrow from its siblings, and if it’s not the root node then it MUST have a sibling. Therefore, concatenate it with either the left or the right sibling, based on which one exists.
  - Slide (L/R): these functions shift over an consecutive items from a node to its left or right sibling. This is done by shifting the item closes to the parent node to the parent node, and shifting the parent node to the left/right sibling at the position closest to the node. This keeps the order, as well as allows a good balance within the tree. Non-leaf nodes will also transfer their child to the sibling.
  - Split: When called on the root node, it gets creates two new nodes where half of the root’s data is stored in one, and the other half in the other. The middle item is extracted and added to a Node with single item, and this will serve as the new node. This new node is then joined to the two new created nodes, and the old large node deleted. When called on an internal node, the same process is applied to create the two new nodes, but the parent is not deleted, but rather the item inserted into the parent. Children nodes as split accordingly.
  - Concatenate: Does the reverse of the split. It joins the right node with with left node, and deletes the right node.
  -  Find: recursively goes through the tree starting at the node, and uses the input argument Item to find where the item belongs. The recursive condition is if the item is less than an item in the current node, then the find goes to the left child of the node item and searches there.
  - sibL and sibR go to the parent node and find the node’s index in the parent. They get the left/right child, from this node, respectively.
  - The get functions simply traverse through the node data and return whatever they suggest they will return. getL and getR return the left and right data respectively, whereas getMax recursively goes through the node’s children to return the right most child in the subtree defined by the node.
  - Write Tree recursively goes through the tree specified by node and prints the Item keys in the order they appear. It goes to the left most child, print out all the items’ keys in the order they appear, goes to the parent and prints the item that corresponds to the parent of the left node, then goes to the right node and recursively prints the items’ keys.
 
