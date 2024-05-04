/**
the idea is to be able to traverse a tree in constant space,
without allocating any stacks(implicit/explict) or queues.
Instead the XOR operation will guide the traversal of the
tree. Before proceeding we need to answer what purpose do
stacks/queues actually serve specifically in guiding the
traversal. After which we can think of offsetting the work
by performing XOR operation on some data stored inside
every single node while building the tree itself.
				a                   <- root node
				/\
		b               c
		/\              /\
	d       e       f       g
	|       |       |       |
	X       X       X       X       <- leaf nodes

suppose we start the traversal at the root node a,
and go left, explore it entirely before going right.
so from a we go to b, from b to d. we reach a dead end.
we need to go back, but back to where? its not always
going to be e(right child of the immediate parent).
if we reached a dead end at e we would have to escape
to the subtree rooted at c. Such redirections from
d to e and e to c can be computed in O(1) Time if
they have been cached in a stack or a queue O(N).

So vaguely speaking, we can assume that a node of
the tree will have 1 data field and 2 address
fields for the left and right child respectively.
Instead of simply storing the address of the
immidiate left/right child that will only help us
navigate downwards, we must store the result of an
XOR operation that will help us go both ways,
downwards if the subtree hasnt been explored, but
if it has been explored it must be able to redirect
us upwards, towards a root of an unexplored subtree.

So here are the rules:
node's left stores the xor of its parent and left child's addresses,
node's right stores the xor of its parent and right child's addresses,
root node assumes the parent nodes as nullptr and
leaf nodes assume the child nodes as nullptr.
*/

#include <cstdint>
#include <iostream>
using namespace std;

struct Node {
	int data;
	Node* leftChildXORparent = nullptr;
	Node* rightChildXORparent = nullptr;
	Node(int data) : data(data) {}
};

// Helper function for XOR operation
Node* XOR(Node* a, Node* b) {
	return reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b));
}

// Function to insert a new node into the tree
void insert(Node** root, int data) {
}

// Function to traverse the tree in inorder fashion using XOR traversal
void inorder(Node* root) {
	Node* parent = nullptr;
	Node* current = root;
	bool canGoLeft = true;
	while (current != nullptr) {
		if (canGoLeft and XOR(current->leftChildXORparent, parent) != nullptr) {
			Node* prevParent = parent;
			parent = current;
			current = XOR(current->leftChildXORparent, prevParent);
		} else {
			cout << current->data << endl;
			if (XOR(current->rightChildXORparent, parent) != nullptr) {
				Node* prevParent = parent;
				parent = current;
				current = XOR(current->rightChildXORparent, prevParent);
				canGoLeft = true;
			} else {  // if (XOR(current->leftChildXORparent, parent) == nullptr and XOR(current->rightChildXORparent, parent) == nullptr) {
				canGoLeft = false;
				Node* prevCurrent = current;
				current = parent;
				parent = XOR(current->leftChildXORparent, prevCurrent);
			}
		}
	}
}

int main() {
	Node* root = new Node(0);
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 3);
	insert(&root, 4);
	insert(&root, 5);
	insert(&root, 6);
	insert(&root, 7);
	inorder(root);
	return 0;
}
