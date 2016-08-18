#ifndef B_TREE
#define B_TREE

#include <string>
#include <vector>

// declarations
struct Item;
struct Node;
class bTree;

// aliases
typedef std::vector<Item> ItemList;
typedef std::vector<Node*> NodeList;
typedef std::string string;

// Item structure = key-value pair
struct Item
{
 	string key;
 	string value;

 	Item()
 	{
 		key = "";
 		value = "";
 	}
 	Item(string k, string v)
 	{
 		key = k;
 		value = v;
 	}
 	bool operator < (const Item &other) const
 	{
 		if(this->key < other.key)
 			return true;
 		return false;
 	}
 	bool operator == (const Item &other) const
 	{
 		if (this->key == other.key)
 			return  true;
 		return false;
 	}
};

// Node structure
struct Node
{
	int size;
	bool leaf;
	int height;		// debugging tool: print graph
	Node *parent;
	ItemList items;
	NodeList children;
	Node(){}
	Node(int s){size = s;}
};

// B-Tree
class bTree
{
private:
	Node *root;

	int getNodeIndex(Node *n);
	void adjust(Node *n);
	void insert(Item *i, Node *n);
	void slideL(Node *l, Node *r);
	void slideR(Node *l, Node *r);
	Node* getL(Node *n);
	Node* getR(Node *n);
	Node* split(Node *n);
	Node* concatenate(Node *l, Node *r);
	Node* findNode(Item *it, Node *n);
	Node* sibL(Node *n);
	Node* sibR(Node *n);
	Node* getMax(Node *n);
	string writeTree(Node *n);
public:
	bTree();
	bTree(int size);
	~bTree();
	void insert(string key, string value);
	bool find(string key, string *value);
	bool delete_key(string key);
	string toStr();
};





#endif