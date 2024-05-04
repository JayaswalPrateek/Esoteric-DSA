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

void inorder(Node* root) {
	// this is all i could do, not enough but
	// implemetaion left as an exercise for a smarter me
	// keep working hard, its not too far
}

int main() {
	Node* root = new Node(0);
	Node* one = new Node(1);
	Node* two = new Node(2);
	Node* three = new Node(3);
	Node* four = new Node(4);
	Node* five = new Node(5);
	Node* six = new Node(6);

	root->leftChildXORparent = XOR(one, nullptr);
	root->rightChildXORparent = XOR(nullptr, two);

	one->leftChildXORparent = XOR(three, root);
	one->rightChildXORparent = XOR(root, four);
	two->leftChildXORparent = XOR(five, root);
	two->rightChildXORparent = XOR(root, six);

	three->leftChildXORparent = XOR(nullptr, one);
	three->rightChildXORparent = XOR(one, nullptr);
	four->leftChildXORparent = XOR(nullptr, one);
	four->rightChildXORparent = XOR(one, nullptr);

	five->leftChildXORparent = XOR(nullptr, two);
	five->rightChildXORparent = XOR(two, nullptr);
	six->leftChildXORparent = XOR(nullptr, two);
	six->rightChildXORparent = XOR(two, nullptr);

	inorder(root);
	return 0;
}
