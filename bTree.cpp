#include "bTree.h"

// --------  DEBUGGER  ---------- //
//#include <iostream>

// template <typename T>
// void print(T message)
// {
// 	std::cout << message << std::endl;
// }

// template <typename T, typename U>
// void print(T message, U message2)
// {
// 	std::cout << message << message2 << std::endl;
// }

// // determine spacing based on tree height
// std::string addSpace(int height)
// {
// 	std::string s;
// 	while(height > 0)
// 	{
// 		s = s + "  ";
// 		--height;
// 	}
// 	return s;
// }

// -------- END DEBUGGER -------- //

// Constructor
bTree::bTree()
{
	bTree(2);
}

// Constructor with custom node size
bTree::bTree(int s)
{
	root = new Node();
	root->size = s;
	root->leaf = true;
	root->height = 0;
	root->parent = NULL;
}

// Destructor
bTree::~bTree()
{
	delete root;
}

//Inserts the value pair into the tree
void bTree::insert(string key, string value)
{
	Item *i = new Item(key,value);	// create item
	Node *n = findNode(i,root);		// get node where item belongs
	insert(i,n);					// insert item in node"
	adjust(n);						// adjust node"
}


// Searches for the key in the tree, if found, it returns
// true else returns false.
// If the key is found, it stores the value in the *value variable
bool bTree::find(string key, string *value)
{
	Item *iKey = new Item(key,"");
	Node *n = findNode(iKey,root);
	delete iKey;
	// search for key in node
	for(int i = 0; i < n->items.size(); ++i)
	{
		if(key == n->items[i].key)
		{
			*value = n->items[i].value; 
			return true;
		}
	}
	return false;
}

//Searches for the key and deletes it. Returns true if deletion was
//successful, returns false if the key wasn't found
bool bTree::delete_key(string key)
{
	Item *iKey = new Item (key,"");
	Node *n = findNode(iKey,root);
	delete iKey;
	int i;
	// get index of item in node
	for(i = 0; i < n->items.size(); ++i)
	{
		if(key == n->items[i].key)
			break;
	}
	// delete item: if exists
	if(i != n->items.size())
	{
		// leaf node
		if(n->leaf)
		{
			// shift items down
			for(; i < n->items.size()-1; ++i)
			{
				n->items[i] = n->items[i+1];
			}
			n->items.pop_back();		// delete last item
			adjust(n);
		}
		else
		{
			// get largest leaf node in the left child
			Node *largest = getMax(n->children[i]);
			// get largest item less than item to be deleted
			Item large = largest->items[largest->items.size()-1];
			n->items[i] = large;	// replace item with largest item
			largest->items.pop_back();	// remove largest item from leaf node
			adjust(largest);
		}
		return true;
	}
	return false;
}

// concatenates the contents of the tree from an inorder traversal
// into an output stream with each string item followed by a 
// newline character in the stream
string bTree::toStr()
{
	return writeTree(root);
}

// ----------------- HELPER FUCNTIONS ---------------------------

// Inserts item into specified node
void bTree::insert(Item *i, Node *n)
{
	n->items.push_back(*i);
	Item *temp = new Item();

	// bubble down item to correct position
	for(int j = n->items.size()-1; j > 0; --j)
	{
		if(n->items[j] < n->items[j-1])
		{
			*temp = n->items[j];
			n->items[j] = n->items[j-1];
			n->items[j-1] = *temp;
		}
	}
}

// Adjust the node to ensure proper distribution
void bTree::adjust(Node *n)
{
	// print("ADJUST");
	// writeTree(n);
	// print("--------");

	// called on empty node
	if(n == NULL)
		return;

	Node *l = sibL(n);		// left sibling of node
	Node *r = sibR(n);		// right sibling of node

	// Overflow
	if(n->items.size() == n->size)
	{
		// left sibling exist and not full
		if(l != NULL && l->items.size() != l->size-1)
		{
			slideL(l,n);			// push items to parent -> left sibling
		}
		// right sibling exist and not full
		else if (r != NULL && r->items.size() != r->size-1)
		{
			slideR(n,r);			// push items to parent -> right sibling
		}
		// no siblings/full siblings
		else
		{
			n = split(n);			// split node and return left node
			adjust(n->parent);		// adjust parent
		}

	}
	// Underflow
	else if (n->items.size() < n->size/2)
	{
		// left sibling exist and more than half full
		if(l != NULL && l->items.size() > l->size/2)
		{
			slideR(l,n);			// draw items from parent <- left sibling
		}
		// right sibling exist and more than half full
		else if (r != NULL && r->items.size() > r->size/2)
		{
			slideL(n,r);			// draw items from parent <- right sibling
		}
		// root node: no siblings
		else if (n == root)
		{
			// only one child
			if(n->children.size() == 1)
			{
				Node *child = n->children[0];
				delete root;
				root = child;
				root->parent = NULL;
			}
		}
		// siblings are don't have enough items
		else
		{
			// concatenate with left sibling
			if(l != NULL)
			{
				n = concatenate(l,n);
				adjust(n->parent);
			}
			// concatenate with right sibling
			else
			{
				n = concatenate(n,r);
				adjust(n->parent);
			}
		}

	}
	return;
}

// Moves first item from right sibling to parent, move parent item to left sibling
// Moves first child from right to left, if not leaf node
void bTree::slideL(Node *l, Node *r)
{
	int id = getNodeIndex(l);			// get index of left node in parent
	Item *p = new Item();
	*p = l->parent->items[id];			// item in parent at index
	insert(p,l);						// add item in left node
	l->parent->items[id] = r->items[0];	// replace parent item with first item in right child
	
	// shift items down in right child
	for(int i = 1; i < r->items.size(); ++i)
	{
		r->items[i-1] = r->items[i];
	}
	r->items.pop_back();				// delete last item in right node (duplicate)
	
	// internal node
	if(!r->leaf)
	{
		// add first child of right node to left node
		Node *child = r->children[0];
		child->parent = l;
		l->children.push_back(child);
		// shift children down in right child
		for(int i = 1; i < r->children.size(); ++i)
		{
			r->children[i-1] = r->children[i];
		}
		r->children.pop_back();			// remove last child
	}

}

// Moves last item from left sibling to parent, move parent item to right sibling
// Moves last child from left to right if not leaf node
void bTree::slideR(Node *l, Node *r)
{	
	int id = getNodeIndex(l);			// get index of left node in parent
	Item *p = new Item(); 
	*p = l->parent->items[id];			// item in parent at index
	insert(p,r);						// add item to right node
	// add left node's last item to parent node
	l->parent->items[id] = l->items[l->items.size()-1]; 
	l->items.pop_back();				// remove item from left node

	// internal node
	if(!l->leaf)
	{
		// shift children up in right node
		for(int i = r->children.size()-1; i > 0; --i)
		{
			r->children[i] = r->children[i-1];
		}
		// add last child of left node as first child of right node
		Node *child = l->children[l->children.size()-1];
		child->parent = r;				// update child's parent
		r->children[0] = r;				// add to right node
		l->children.pop_back(); 		// remove child node from left node
	}

}

// Splits the node into left and right node, and either adds to the parent node
// the middle item in the split node, or creates a new node as the parent(root)
// Returns the parent node
Node* bTree::split(Node *n)
{
	// root node
	if(n == root)
	{
		Node *l = getL(root);				// create left node
		Node *r = getR(root);				// create right node						
		int size = root->size;				// save size
		int mid = root->items.size()/2;		// index of middle item
		Item m = root->items[mid];							
		root = new Node(size);				// start fresh
		root->leaf = false;					// root no longer a leaf node
		root->height = l->height + 1;		// update root height
		insert(&m,root);						// add saved item to new node
		root->children.push_back(l);
		root->children.push_back(r);
		l->parent = root;					// update parent of new nodes
		r->parent = root;
		delete n;
		n = root->children[0];
	}
	else
	{
		Node *l = getL(n);					// create a new left node
		Node *r = getR(n);					// create a new right node
		int mid = n->items.size()/2;		// index of middle item in node
		int id = getNodeIndex(n);
		// add empty space at the end
		Item *emptyItem = new Item();
		Node *emptyNode = new Node();
		n->parent->items.push_back(*emptyItem);
		n->parent->children.push_back(emptyNode);
		// shift items up in parent
		for(int i = n->parent->items.size()-1; i > id; --i)
		{
			n->parent->items[i] = n->parent->items[i-1];
			n->parent->children[i+1] = n->parent->children[i];
		}
		n->parent->items[id] = n->items[mid];	// add node middle item to parent
		l->parent = n->parent;					// update parents of new nodes to current node's parent
		r->parent = n->parent;
		n->parent->children[id] = l;			// add left node to parent
		n->parent->children[id+1] = r;			// add right node to parent
		delete n;
		delete emptyItem;
		delete emptyNode;
		n = l;
	}
	return n;
}

// Concatenate the left and right siblings, and returns the joined node
Node* bTree::concatenate(Node *l, Node *r)
{
	int id = getNodeIndex(l);					// index of left node in its parent
	l->items.push_back(l->parent->items[id]);	// add parent item to left node

	int i;
	// add right node's data to left node
	for(i = 0; i < r->items.size(); ++i)
	{
		l->items.push_back(r->items[i]);		// add right node's items
		if(!r->leaf)
		{
			// add right node's children to left node
			Node *child = r->children[i];
			child->parent = l;
			l->children.push_back(child);
		}
	}
	if(!r->leaf)
	{
		// add last child
		Node *child = r->children[i];
		child->parent = l;
		l->children.push_back(child);
	}
	// shift items and children down in parent (replace data)
	for(int i = id; i < l->parent->items.size()-1; ++i)
	{
		l->parent->items[i] = l->parent->items[i+1];
		l->parent->children[i+1] = l->parent->children[i+2];
	}
	l->parent->items.pop_back();		// remove duplicate last item
	l->parent->children.pop_back();		// remove duplicate last child
	delete r;							// free memory of right node
	return l;
}

// Returns the node where Item n belongs
Node* bTree::findNode(Item *it, Node *n)
{
	// leaf node
	if(n->leaf)
		return n;

	int i;
	// get leaf where item belongs
	for(i = 0; i < n->items.size(); ++i)
	{
		if(*it < n->items[i])
		{
			return findNode(it,n->children[i]);
		}
		if(*it == n->items[i])
		{
			return n;
		}
	}
	return findNode(it,n->children[i]);
}

// Retunrs the left sibling of node
Node* bTree::sibL(Node *n)
{
	// root node has no siblings
	if (n->parent == NULL)
		return NULL;
	// get this node's index in parent
	int i = getNodeIndex(n);
	// this node is first child
	if (i == 0)
		return NULL;
	// get left sibling
	return n->parent->children[i-1];
}

// Retunrs the right sibling of node
Node* bTree::sibR(Node *n)
{
	// root node has no siblings
	if (n->parent == NULL)
		return NULL;
	// get this node's index in parent
	int i = getNodeIndex(n);
	// this node is last child
	if (i == n->parent->children.size()-1)
		return NULL;
	// get right sibling
	return n->parent->children[i+1];
}

// Returns the index representing the location of node in parent. -1 = N/a
int bTree::getNodeIndex(Node *n)
{
	// get this node
	for(int i = 0; i < n->parent->children.size(); ++i)
	{
		if(n == n->parent->children[i])
			return i;	// found this node
	}
	return -1;
}

// Get lower half of node's data
Node* bTree::getL(Node *n)
{
	Node *l = new Node(n->size);
	// update properties
	l->leaf = n->leaf;
	l->height = n->height;
	// get data
	int m = n->items.size()/2;
	int i;
	for(i = 0; i < m; ++i)
	{
		l->items.push_back(n->items[i]);
		// internal node, get children
		if(!n->leaf)
		{
			Node *child = n->children[i];
			child->parent = l;
			l->children.push_back(child);
		}
	}
	if(!n->leaf)
	{
		Node *child = n->children[i];
		child->parent = l;
		l->children.push_back(child);
	}
	return l;
}

// Get upper half of node's data
Node* bTree::getR(Node *n)
{
	Node *r = new Node(n->size);
	// update properties
	r->leaf = n->leaf;
	r->height = n->height;
	// get data
	int m = n->items.size()/2;
	int i;
	for(i = m+1; i < n->items.size(); ++i)
	{
		r->items.push_back(n->items[i]);
		// internal node, get children
		if(!n->leaf)
		{
			Node *child = n->children[i];
			child->parent = r;
			r->children.push_back(child);
		}
	}
	if(!n->leaf)
	{
		Node *child = n->children[i];
		child->parent = r;
		r->children.push_back(child);
	}
	return r;
}

// Returns the largest node in given subtree
Node* bTree::getMax(Node *n)
{
	// leaf node: return this node
	if(n->leaf)
		return n;
	// search again in last child of this node
	return getMax(n->children[n->children.size()-1]);
}

// Recursively print the bTree
string bTree::writeTree(Node *n)
{
	string list;	
	// leaf node
	if(n->leaf)
	{
		for(int i = 0; i < n->items.size(); ++i)
		{
			list = list + n->items[i].key + "\n";
		}
		return list;
	}
	int i; 
	// internal node
	for(i = 0; i < n->items.size(); ++i)
	{
		list = list + writeTree(n->children[i]);
		list = list + n->items[i].key + "\n";
	}
	// last child
	return list + writeTree(n->children[i]);

}
