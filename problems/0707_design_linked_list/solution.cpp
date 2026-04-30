// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 707: Design Linked List
 * Difficulty: Medium
 * Tags: Design, Linked List
 *
 * Problem Description:
 * Design your implementation of the linked list. You can choose to use a
 * singly or
 * doubly linked list.  
 * A node in a singly linked list should have two attributes: `val` and `next`.
 * `val` is the value of the current node, and `next` is a pointer/reference
 * to the
 * next node.  
 * If you want to use the doubly linked list, you will need one more attribute
 * `prev` to indicate the previous node in the linked list. Assume all nodes
 * in the
 * linked list are **0-indexed**.
 * 
 * Implement the `MyLinkedList` class:
 * 
 *   * `MyLinkedList()` Initializes the `MyLinkedList` object.
 * * `int get(int index)` Get the value of the `indexth` node in the linked
 * list. If the index is invalid, return `-1`.
 * * `void addAtHead(int val)` Add a node of value `val` before the first
 * element of the linked list. After the insertion, the new node will be the
 * first node of the linked list.
 * * `void addAtTail(int val)` Append a node of value `val` as the last
 * element of the linked list.
 * * `void addAtIndex(int index, int val)` Add a node of value `val` before
 * the `indexth` node in the linked list. If `index` equals the length of the
 * linked list, the node will be appended to the end of the linked list. If
 * `index` is greater than the length, the node **will not be inserted**.
 * * `void deleteAtIndex(int index)` Delete the `indexth` node in the linked
 * list, if the index is valid.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["MyLinkedList", "addAtHead", "addAtTail", "addAtIndex", "get",
 * "deleteAtIndex", "get"]
 *     [[], [1], [3], [1, 2], [1], [1], [1]]
 *     **Output**
 *     [null, null, null, null, 2, null, 3]
 *     
 *     **Explanation**
 *     MyLinkedList myLinkedList = new MyLinkedList();
 *     myLinkedList.addAtHead(1);
 *     myLinkedList.addAtTail(3);
 *     myLinkedList.addAtIndex(1, 2);    // linked list becomes 1->2->3
 *     myLinkedList.get(1);              // return 2
 *     myLinkedList.deleteAtIndex(1);    // now the linked list is 1->3
 *     myLinkedList.get(1);              // return 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= index, val <= 1000`
 *   * Please do not use the built-in LinkedList library.
 * * At most `2000` calls will be made to `get`, `addAtHead`, `addAtTail`,
 * `addAtIndex` and `deleteAtIndex`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/design-linked-list/
 */


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

// ==================== Common Data Structures ====================

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Definition for a Node (N-ary tree / graph with neighbors)
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0), neighbors(vector<Node*>()) {}
    Node(int _val) : val(_val), neighbors(vector<Node*>()) {}
    Node(int _val, vector<Node*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

class MyLinkedList {
public:
    MyLinkedList() {
        
    }
    
    int get(int index) {
        
    }
    
    void addAtHead(int val) {
        
    }
    
    void addAtTail(int val) {
        
    }
    
    void addAtIndex(int index, int val) {
        
    }
    
    void deleteAtIndex(int index) {
        
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MyLinkedList
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MyLinkedList","addAtHead","addAtTail","addAtIndex","get","deleteAtIndex","get"]
    // [[],[1],[3],[1,2],[1],[1],[1]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MyLinkedList obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
