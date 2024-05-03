// https://blog.reachsumit.com/posts/2020/07/skip-list/

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

const int MAX_LEVEL = 16;  // Maximum number of levels in the skip list
/*
Increasing the value for MAX_LEVEL involves a trade-off between space complexity and search efficiency.

Costs:
1. Increased Space Complexity: With each additional level, the skip list requires more memory to maintain pointers, increasing space consumption.
2. Higher Construction Overhead: Constructing a skip list with more levels requires additional time and computational resources.
3. Potential for Increased Memory Fragmentation: Larger skip lists may suffer from memory fragmentation, especially in environments with limited memory allocation.

Benefits:
1. Improved Search Efficiency: Higher max levels can reduce the average number of nodes visited during a search operation, leading to faster search times.
2. Balanced Search Performance: Skip lists maintain logarithmic search time even with increased max levels, ensuring consistent performance.
3. Reduced Probability of Degenerate Lists: Increasing max levels lowers the likelihood of generating degenerate skip lists, which could otherwise degrade search performance to linear time complexity.
4. Flexibility for Dynamic Environments: Greater max levels provide flexibility for accommodating future growth and changes in data distribution without significant degradation in search performance.

Determining the optimal value for the MAX_LEVEL in a skip list when the total number of elements is known apriori involves considering the balance between search efficiency and space complexity.
A popular formula for determining the optimal max levels in a skip list is based on the equation:

	MAX_LEVEL=⌈log⁡2(n)⌉−1max levels=⌈log2​(n)⌉−1

Where:
	nn is the total number of elements in the skip list.
	⌈x⌉⌈x⌉ represents the ceiling function, which rounds xx up to the nearest integer.

Explanation:
	The logarithmic function (log⁡2(n)log2​(n)) provides a good balance between search efficiency and space complexity. It ensures that search operations remain efficient while keeping the space overhead manageable.
	Subtracting 1 from the result ensures that the maximum number of levels is proportional to the logarithm of the number of elements but still provides a sufficient number of levels for effective searching.
*/

// Node class for the Skip List
class Node {
public:
	int key;		 // query the key and
	int value;		 // search for value from the skip list
	Node** forward;	 // Array of forward pointers

	// Constructor for Node
	Node(int key, int value, int level) {
		this->key = key;
		this->value = value;
		this->forward = new Node*[level + 1];  // to store head pointers for each level of the Skip List
		for (int i = 0; i <= level; i++) {
			forward[i] = nullptr;
		}
	}

	// Destructor for Node
	~Node() {
		delete[] forward;
	}
};

// Skip List class
class SkipList {
private:
	Node* header;  // Head node for the Skip List
	int level;	   // Current level of the Skip List(<=MAX_LEVEL)
	int size;	   // Number of nodes in the Skip List

public:
	// Constructor for Skip List
	SkipList() {
		header = new Node(0, 0, MAX_LEVEL);
		level = 0;
		size = 0;
	}

	// Destructor for Skip List
	// removes nodes bottom up
	// a node stores an array of heads of all nodes below it
	// so 0th index of such a node in each level will be different!
	~SkipList() {
		Node* p = header->forward[0];
		while (p) {
			Node* q = p->forward[0];
			delete p;
			p = q;
		}
		delete header;
	}

	// Returns the number of nodes in the Skip List
	int getSize() {
		return size;
	}

	// Returns the current level of the Skip List
	int getLevel() {
		return level;
	}

	// Searches for a node with the given key in the Skip List
	Node* find(int key) {
		Node* p = header;					// the level with least nodes(on the top)
		for (int i = level; i >= 0; i--) {	// search from highest to lowest level
			// if the next key on the same level exists and is smaller than the queried key
			while (p->forward[i] && p->forward[i]->key < key) {
				p = p->forward[i];	// fearlessly move to it
			}						// else need to go one level down
		}							// so either go right or go down
		return p->forward[0];
	}

	int randomLevel() {
		int level = 0;
		while (rand() % 2 == 0 && level < MAX_LEVEL) {
			level++;
		}
		return level;
	}

	// Inserts a new node with the given key and value into the Skip List
	void insert(int key, int value) {
		// in a sorted linked list, we would insert the key just after the last key thats smaller than the key to be inserted
		// here we have multiple layers of sorted linked lists with varying number of nodes in each
		// so update array stores the address of all such nodes
		Node* update[MAX_LEVEL + 1];
		Node* p = header;
		// going from the layer with least nodes(highest level at the top)
		// to the lowest layer with most nodes(lowest level at the bottom)
		for (int i = level; i >= 0; i--) {
			while (p->forward[i] && p->forward[i]->key < key) {
				p = p->forward[i];
			}
			update[i] = p;	// stores the node that has the largest key thats just smaller than the key to be inserted
		}
		// so after the loop, p should be at the lowest node(which has the most nodes and is at the bottom of the skip list)
		// and p will be poiting the the node with largest key just smaller to the key to be inserted
		// so the key must be inserted after it
		p = p->forward[0];
		if (p && p->key == key) {  // if the key already exists simply update value
			p->value = value;
		} else {  // otherwise insert it randomly in a random level
			int newLevel = randomLevel();
			if (newLevel > level) {	 // level < newLevel <= MAX_LEVEL
				// create the random level if not exists already
				for (int i = level + 1; i <= newLevel; i++) {
					update[i] = header;
				}
				level = newLevel;
			}
			p = new Node(key, value, newLevel);
			for (int i = 0; i <= newLevel; i++) {
				p->forward[i] = update[i]->forward[i];	// p points to the node after the node stored in update[]
				update[i]->forward[i] = p;				// the node stored in update[] points to p
			}
			size++;
		}
	}

	// Removes the node with the given key from the Skip List
	void remove(int key) {
		Node* update[MAX_LEVEL + 1];
		Node* p = header;
		for (int i = level; i >= 0; i--) {
			while (p->forward[i] && p->forward[i]->key < key) {
				p = p->forward[i];
			}
			update[i] = p;
		}
		p = p->forward[0];
		if (p && p->key == key) {
			for (int i = 0; i <= level; i++) {
				if (update[i]->forward[i] != p) {
					break;	// if the node to be deleted is the last in the list or exceeds key
				}
				update[i]->forward[i] = p->forward[i];
			}
			delete p;
			while (level > 0 && header->forward[level] == nullptr) {
				level--;  // recount levels as a level might just have a single node that got deleted
			}
			size--;
		}
	}

	// print the Skip List
	void print() {
		for (int i = 0; i <= level; i++) {
			Node* p = header->forward[i];
			cout << "Level " << i << ": ";
			while (p) {
				cout << "(" << p->key << ", " << p->value << ") ";
				p = p->forward[i];
			}
			cout << endl;
		}
	}
};