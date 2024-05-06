#include <iostream>
#include <queue>
using namespace std;

class Node {
public:
	Node *left, *right;
	int info;
	bool lthread;  // True if left pointer points to predecessor in Inorder Traversal
	bool rthread;  // True if right pointer points to successor in Inorder Traversal

	Node() {
	}

	explicit Node(int data) {
		info = data;
		left = NULL;
		right = NULL;
		lthread = true;
		rthread = true;
	}

	// Insert a Node in Binary Threaded Tree
	Node *insert(Node *root, int ikey) {
		// Searching for a Node with given value
		Node *ptr = root;
		Node *par = NULL;  // Parent of key to be inserted
		while (ptr != NULL) {
			// If key already exists, return
			if (ikey == ptr->info) {
				cout << "Duplicate Key!" << endl;
				return root;
			}

			par = ptr;	// Update parent pointer

			// Update ptr only if the child is not threaded
			if (ikey < ptr->info) {	 // Moving on left subtree.
				if (ptr->lthread == false)
					ptr = ptr->left;
				else
					break;
			} else {  // Moving on right subtree.
				if (ptr->rthread == false)
					ptr = ptr->right;
				else
					break;
			}
		}  // this loop stops when ptr becoms null making par the actual point of insertion

		// Create a new Node
		Node *tmp = new Node;
		tmp->info = ikey;
		tmp->lthread = true;
		tmp->rthread = true;

		// the new node will always be attached as a leaf node to the tree as its a BST
		// if a node is attached to the left, then:
		// 			the parent is no longer threaded on its left, and instead points to its new left child
		//			the left of new node is threaded to point back to the parent's left (Predecessor)
		// 			the right of the new node is threaded to point back to its parent (Successor)
		// if a node is attached to the right, then
		//			the parent is no longer threaded on its right, and instead points ot its right child
		//			the left of the new node is threaded to point back to its parent (Predecessor)
		// 			the right of the new node is threaded to point back to the parent's right (Successor)

		if (par == NULL) {	// For Empty Tree
			root = tmp;
			tmp->left = NULL;
			tmp->right = NULL;
		} else if (ikey < (par->info)) {  // attach new node to the left
			tmp->left = par->left;		  // Predecessor
			tmp->right = par;			  // Successor
			par->lthread = false;		  // lthread is converted to left link
			par->left = tmp;			  // Tmp will become left child.
		} else {						  // ikey > (par->info), not >=, as already checked for duplicates, attach new node to the right
			tmp->left = par;			  // Parent will become the predecessor for its right child.
			tmp->right = par->right;	  // right pointer will Point to Successor
			par->rthread = false;		  // Convert Thread to link.
			par->right = tmp;			  // New node will become right child of parent.
		}
		return root;
	}

	// Recursive Inorder Traversing(without exploiting the threaded nature of the BST)
	void nonThreadedInorder(Node *root) {
		if (root == NULL)
			return;
		// First recur on left subtree
		if (root->lthread == false)
			nonThreadedInorder(root->left);
		// Then read the data of child
		cout << root->info << " ";
		// Recur on the right subtree
		if (root->rthread == false)
			nonThreadedInorder(root->right);
	}

	// Recursive Preorder Traversing(without exploiting the threaded nature of the BST)
	void nonThreadedPreorder(Node *root) {
		if (root == NULL)
			return;
		// First read the data of child
		cout << root->info << " ";
		// Then recur on left subtree
		if (root->lthread == false)
			nonThreadedPreorder(root->left);
		// Then Recur on the right subtree
		if (root->rthread == false)
			nonThreadedPreorder(root->right);
	}

	// Recursive Podtorder Traversing(without exploiting the threaded nature of the BST)
	void nonThreadedPostorder(Node *root) {
		if (root == NULL)
			return;
		// Then recur on left subtree
		if (root->lthread == false)
			nonThreadedPostorder(root->left);
		// Then Recur on the right subtree
		if (root->rthread == false)
			nonThreadedPostorder(root->right);
		// First read the data of child
		cout << root->info << " ";
	}

	// Returns inorder successor using rthread (Used in inorder and deletion)
	Node *getInorderSuccessor(Node *ptr) {
		// If rthread is set, we can quickly find
		if (ptr->rthread == true)
			return ptr->right;

		// Else return leftmost child of right subtree
		ptr = ptr->right;
		while (ptr->lthread == false)
			ptr = ptr->left;
		return ptr;
	}

	// Non-recursive Printing the threaded tree in Inorder
	void threadedInorder(Node *root) {
		if (root == NULL)
			cout << "Tree is empty" << endl;

		// Reach leftmost Node
		Node *ptr = root;
		while (ptr->lthread == false)
			ptr = ptr->left;

		// One by one print successors
		while (ptr != NULL) {
			cout << ptr->info << " ";
			ptr = getInorderSuccessor(ptr);
		}
	}

	// Non-recursive Preorder Traversal of TBT
	void threadedPreorder(Node *root) {
		Node *ptr;
		if (root == NULL) {
			cout << "Tree is empty";
			return;
		}
		ptr = root;
		while (ptr != NULL) {
			cout << ptr->info << " ";
			if (ptr->lthread == false)
				ptr = ptr->left;
			else if (ptr->rthread == false)
				ptr = ptr->right;
			else {
				while (ptr != NULL && ptr->rthread == true)
					ptr = ptr->right;  // go to successor / parent
				if (ptr != NULL)
					ptr = ptr->right;  // go to right child of successor / parent
			}
		}
	}

	// Returns inorder predessor using left and right children (Used in deletion)
	Node *getInorderPredecessor(Node *ptr) {
		if (ptr->lthread == true)
			return ptr->left;

		ptr = ptr->left;
		while (ptr->rthread == false)
			ptr = ptr->right;

		return ptr;
	}

	// Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
	Node *caseA(Node *root, Node *par, Node *ptr) {	 // No Children of the node to be deleted
		// If Node to be deleted is root
		if (par == NULL)
			root = NULL;

		// If Node to be deleted is left of its parent
		else if (ptr == par->left) {
			par->lthread = true;
			par->left = ptr->left;
		} else {
			par->rthread = true;
			par->right = ptr->right;
		}

		// Free memory and return new root
		delete ptr;

		return root;
	}

	// Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
	Node *caseB(Node *root, Node *par, Node *ptr) {	 // One Child of the node to be deleted
		Node *child;

		// Initialize child Node to be deleted has left child.
		if (ptr->lthread == false)
			child = ptr->left;

		// Node to be deleted has right child.
		else
			child = ptr->right;

		// Node to be deleted is root Node.
		if (par == NULL)
			root = child;

		// Node is left child of its parent.
		else if (ptr == par->left)
			par->left = child;
		else
			par->right = child;

		// Find successor and predecessor
		Node *s = getInorderSuccessor(ptr);
		Node *p = getInorderPredecessor(ptr);

		// If ptr has left subtree.
		if (ptr->lthread == false)
			p->right = s;
		// If ptr has right subtree.
		else if (ptr->rthread == false)
			s->left = p;

		delete ptr;

		return root;
	}

	// Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
	Node *caseC(Node *root, Node *ptr) {  // Two Children of the node to be deleted
		// Find inorder successor and its parent.
		Node *parsucc = ptr;
		Node *succ = ptr->right;

		// Find leftmost child of successor
		while (succ->lthread == false) {
			parsucc = succ;
			succ = succ->left;
		}

		ptr->info = succ->info;

		if (succ->lthread == true && succ->rthread == true)
			root = caseA(root, parsucc, succ);
		else
			root = caseB(root, parsucc, succ);

		return root;
	}

	// Deletes a key from threaded BST with given root and
	// returns new root of BST.
	Node *delThreadedBST(Node *root, int dkey) {
		// Initialize parent as NULL and ptrent Node as root.
		Node *par = NULL, *ptr = root;

		// Set true if key is found
		int found = 0;

		// Search key in BST : find Node and its parent.
		while (ptr != NULL) {
			if (dkey == ptr->info) {
				found = 1;
				break;
			}
			par = ptr;
			if (dkey < ptr->info) {
				if (ptr->lthread == false)
					ptr = ptr->left;
				else
					break;
			} else {
				if (ptr->rthread == false)
					ptr = ptr->right;
				else
					break;
			}
		}

		if (found == 0)
			cout << "key not present in tree" << endl;

		// Two Children
		else if (ptr->lthread == false && ptr->rthread == false)
			root = caseC(root, ptr);

		// Only Left Child
		else if (ptr->lthread == false)
			root = caseB(root, par, ptr);

		// Only Right Child
		else if (ptr->rthread == false)
			root = caseB(root, par, ptr);

		// No child
		else
			root = caseA(root, par, ptr);

		return root;
	}

	// Searching in TBT
	Node *search(Node *root, int key) {
		if (root == NULL || root->info == key)
			return root;

		// Key is greater than root's data
		if (root->info < key && root->rthread == false)
			return search(root->right, key);

		// Key is smaller than root's data
		else if (root->info > key && root->lthread == false)
			return search(root->left, key);

		return nullptr;
	}

	// Level Order Printing
	void printLevelOrder(Node *root) {
		// Base Case
		if (root == NULL)
			return;

		// Create an empty queue for level order tarversal
		queue<Node *> q;

		// Enqueue Root and initialize height
		q.push(root);

		while (q.empty() == false) {
			// Dequeue all nodes of current level and Enqueue all nodes of next level
			cout << " ===> ";
			while (q.size() > 0) {
				Node *node = q.front();
				cout << node->info << " ";
				q.pop();
				if (node->left != NULL)
					q.push(node->left);
				if (node->right != NULL)
					q.push(node->right);
			}
		}
	}
};
