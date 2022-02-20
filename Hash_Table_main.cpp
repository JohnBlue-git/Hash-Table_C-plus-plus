/*
Auther: John Blue
Time: 2022/2
Platform: ATOM with MinGw and g++compiler
Object: HASH TABLE

Implementation:
 mod 10 as hash function
 collision then use linked list

Key Range(int): 0 ~ 100

mod(%)
0 % 10 = 0
...
9 % 10 = 9
10 % 10 = 0
...
*/

#include <iostream> // i/o

////// ListNode.h

class HASH;

class ListNode {
private:// content
  int key;
  int value;
  ListNode* next;

private:// prevent cilent from using these
  // operator
  ListNode& operator=(const ListNode& copy) { return *this; }

public:
	// constructor
	ListNode(): key(- 1), value(- 1), next(0) {}
	ListNode(int k, int v): key(k), value(v), next(0) {}
  ListNode(const ListNode& copy) {
    key = copy.key;
    value = copy.value;
    next = 0;
    if (copy.next != 0) {
      next = new ListNode(*copy.next);
    }
  }
  ~ListNode() {
    key = 0;
    value = 0;
    delete next;
  }

  friend HASH;// set HASH as friend of ListNode
};

////// HASH.h

class HASH {
private:
  // var
  // method 1
  //ListNode** table;
  // method 2
  ListNode* table[10];
  // method 3 not work
  //ListNode (*table)[10];

public:
  // constructor
  HASH() {
    // method 1
    //table = new ListNode*[10];
    // method 1 & 2
    for (int i = 0; i < 10; i++) {// !!! must initialize
      table[i] = 0;
    }
  }
  HASH(const HASH& copy) {
    // method 1
    //table = new ListNode*[10];
    // method 1 & 2
    for (int i = 0; i < 10; i++) {
      if (copy.table[i] != 0) {
        table[i] = new ListNode(*copy.table[i]);
      }
      else {
        table[i] = 0;
      }
    }
  }
  ~HASH() {
    // method 1
    //delete[] table;
  }

  // function
  void print();
  bool exist(int ky);
  int find(int ky);
  void put(int ky, int vu);
  void del(int ky);
};

////// HASH.cpp
void HASH::print() {
  for (int i = 0; i < 10; i++) {
    if (table[i] != 0) {
      ListNode* current = table[i];
      while (current != 0) {
        std::cout << "(" << current->key << ")" << current->value << " ";
        current = current->next;
      }
      std::cout << "\n";
    }
  }
}

bool HASH::exist(int ky) {
  // check
  if (ky < 0 || ky > 100) {
    std::cout << "key out of range\n";
    return false;
  }
  // exist?
  ListNode* current = table[ky % 10];
  while (current != 0) {
    if (current->key == ky) {
      return true;
    }
    current = current->next;
  }
  return false;
}

int HASH::find(int ky) {
  // check
  if (ky < 0 || ky > 100) {
    std::cout << "key out of range\n";
    return - 1;
  }
  // find
  ListNode* current = table[ky % 10];
  while (current != 0) {
    if (current->key == ky) {
      return current->value;
    }
    current = current->next;
  }
  std::cout << "no such key\n";
  return - 1;
}

void HASH::put(int ky, int vu) {
  // check
  if (ky < 0 || ky > 100) {
    std::cout << "key out of range\n";
    return;
  }
  // put
  // !!! **root so that we remember the address of pointer
  ListNode** root = &table[ky % 10];
  if (*root == 0) {// case 1: first item in the table[ky % 10]?
    ListNode* nn = new ListNode(ky, vu);// new
    *root = nn;                         // then point
    return;
  }
  // root turn to current
  ListNode* current = *root;
  while (current->next != 0) {// case 2: collision, then ...
    current = current->next;
  }
  current->next = new ListNode(ky, vu);
  return;
}

void HASH::del(int ky) {
  // check
  if (ky < 0 || ky > 100) {
    std::cout << "key out of range\n";
    return;
  }
  // delete
  ListNode* current = table[ky % 10];
  ListNode** ad_p_current = 0;// remember the address of pointer
  while (current != 0) {
    // initial the address of pointer
    if (ad_p_current == 0) {
      ad_p_current = &current;
    }
    // deleting
    if (current->key == ky) {
      *ad_p_current = current->next;
      current->next = 0;
      delete current;
      return;
    }
    // iterate
    ad_p_current = &current->next;
    current = current->next;
  }
  std::cout << "no such key\n";
  return;
}

int main()
{
  //
  int rd[15] = {22, 25, 99, 44, 77, 66, 33, 11, 55, 1, 4, 9, 21, 13, 45};
  HASH hs;
  //
  // put
  //
  for (int i = 0; i < 15; i++) {
    hs.put(rd[i], i);
  }
  //
  // print
  // (key)index
  // (11)7 (1)9 (21)12
  // (22)0
  // (33)6 (13)13
  // (44)3 (4)10
  // (25)1 (55)8 (45)14
  // (66)5
  // (77)4
  // (99)2 (9)11
  std::cout << "print:\n";
  hs.print();
  std::cout << "\n";
  //
  // find
  //
  std::cout << "find:\n";
  std::cout << "key(33) = " << hs.find(33) << "\n";
  std::cout << "\n";
  //
  // delete
  //
  std::cout << "del(1):\n";
  hs.del(1);
  hs.print();
  std::cout << "\n";
  //
  // copy
  //
  std::cout << "Copy:\n";
  HASH cp(hs);
  cp.print();
  std::cout << "\n";
  //
  // two sum
  //
  std::cout << "Two Sum (target = 88):\n";
  int target = 88;// key + key = 88
  int key1, key2; key1 = - 1; key2 = - 1;
  for (int i = 0; i < 15; i++) {
    if (hs.exist(target - rd[i])) {
      key1 = i;
      key2 = hs.find(target - rd[i]);
    }
  }
  std::cout << "rd[" << key1 << "] + rd[" << key2 << "] = 88\n";
}
