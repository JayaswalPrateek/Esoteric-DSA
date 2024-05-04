/*
XOR is a bit operation.
It accepts two operands and compares their values.
XOR evaluates to 0(False) if the operands are the same.
If they differ, it outputs outputs 1(True).
Here's its truth table:
x y (x^y)
1 1   0
1 0   1
0 1   1
0 0   0
i.e. x^y has the memory to remember if x and y were the same bit or not

Properties:
Its commutative so x^y == y^x,
Its associative so x^(y^z) == y^(x^z) == z^(y^x),
Its neutral element is zero so x^0 == x,
Its self annihilating so x^x == 0

Property used for swapping:
let x = (1)2 & y = (1)2
x ^ y = (1 ^ 1)2 = (0)2 <- let this result be stored in z
x ^ z = (1 ^ 0)2 = (1)2 <- value of y derived without using initial value of y(so its unused)
y ^ z = (1 ^ 0)2 = (1)2 <- value of x derived without using initial value of x(so its unused)
so we we were able to derive the initial
values of x and y from x^y stored in z
without needing initial values of y and x
respectively. so instead of storing x^y
in z we can store it in one unused variable
while the other is busy in obtaining its
new value after swapping. Once thats done
we need to replace the variable containing
x^y with the other initial value by XORing
its value with the first swapped variable.

So if you XOR any set of bits(let, x and y) into z,
you can get x by XORing z again with y(ie without x)
you can get y by XORing z again with x(ie without y)
so during the above 2 operations either x or y will
be unused while the other is obtaining the new value.
so instead of storing the initial x^y in z we can
store it in that unused variable while the other one
is obtaining the new value, after which its new value
along with the existing value in that unused variable
can be used to obtain the new value for the unused
variable that stored x^y with initial values of x,y.
So both x and y are swapped with XOR without z.

so initially,
x =  x ^ y

initial value of x is lost and x stores
the xor of initial values of x and y

y =  x ^ y
  = (x ^ y)  ^  y      SINCE x stores x^y of initial x and y instead of initial value of x
  = x  ^  (y ^ y)      ASSOCIATIVE PROPERTY
  = x  ^  (  0  )      SINCE x^x == 0
  =    x               SINCE x^0 == x

y has been swapped with initial value of x while
x still stores x^y with initial values of x and y
which needs to be replaced with initial value of y

x =  x ^ y
  = (x ^ y) ^ x         SINCE x stores x^y of initial x and y instead of initial value of x
							and y was previously swapped with initital value of x
  = (y ^ x)  ^  x       COMMUTATIVE PROPERTY
  = y  ^  (x ^ x)       ASSOCIATIVE PROPERTY
  = y  ^  (  0  )       SINCE x^x == 0
  =    y                SINCE x^0 == x

the xor of initial value of x and y which was initially stored in x
has now been replaced by the initial value of y. SWAPPING COMPLETE!

Example of XOR Swapping
let x=(1010)2 and y=(0011)2
x = x^y = (1010 XOR 0011)2 = (1001)2 <- X STORES THE INITIAL XOR OF INITIAL X, Y
y = x^y = (1001 XOR 0011)2 = (1010)2 <- SWAPPED FIRST SINCE WASNT STORING INITIAL X^Y
x = x^y = (1001 XOR 1010)2 = (0011)2 <- SWAPPED LATER, REPLACES INITIAL X^Y WITH INITIAL Y
(Works on -ve numbers too)

#include <iostream>
using namespace std;

int main() {
	int a = 69;
	int b = 420;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;

	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;

	return 0;
}
*/

// https://adi22maurya.medium.com/xor-linked-list-7c540e08d986

/*
let there be 3 nodes of a XOR list called A, B and C
having addresses 100, 200 and 300 respectively.
data stored in A is '1'
data stored in B is '2'
data stored in C is '3'
let A be the head of the XOR list
headptr -> A{1,(nullptr^200)} ⇄ B{2,(100^300)} ⇄ C{3,(200^nullptr)} -> nullptr
			100					  200				300
so each xor node can act as a doubly linked list node by storing xor of its prev and next nodes
it can use its own address to obtain either one of them by performing additional xor operations.
for forward traversal next = Xor(prev, curr->xnode);
for backwards traversal prev = Xor(curr, next->xnode);
*/

#include <bits/stdc++.h>

#include <cinttypes>
using namespace std;

class Node {
public:
	int data;
	Node* xnode;
};

/*
reinterpret_cast :-
It is used to convert a pointer of some data type into a pointer of another data type,
   int* p = new int(65);
   char* ch = reinterpret_cast<char*>(p);  // here ch = A, as p casted to char pointer and assigned to ch pointer.
*/

Node* Xor(Node* x, Node* y) {
	return reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(x) ^ reinterpret_cast<uintptr_t>(y));
}

void insertFirst(Node** head, int data) {
	Node* new_node = new Node();
	new_node->data = data;

	new_node->xnode = *head;

	if (*head != NULL) {
		(*head)->xnode = Xor(new_node, (*head)->xnode);
	}

	*head = new_node;
}

void printList(Node* head) {
	Node* curr = head;
	Node* prev = NULL;
	Node* next;

	while (curr != NULL) {
		cout << curr->data << " ";
		next = Xor(prev, curr->xnode);
		prev = curr;
		curr = next;
	}

	cout << "\n\n";
	curr = prev;
	next = NULL;
	while (curr != NULL) {
		cout << curr->data << " ";
		prev = Xor(next, curr->xnode);
		next = curr;
		curr = prev;
	}
}
