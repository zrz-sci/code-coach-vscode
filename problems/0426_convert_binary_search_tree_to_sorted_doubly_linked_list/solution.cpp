// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 426: Convert Binary Search Tree to Sorted Doubly Linked List
 * Difficulty: Medium
 * Tags: Stack, Tree, Depth-First Search, Binary Search Tree, Linked List, Binary Tree, Doubly-Linked List
 *
 * Problem Description:
 * Convert a **Binary Search Tree** to a sorted **Circular Doubly-Linked
 * List** in
 * place.
 * 
 * You can think of the left and right pointers as synonymous to the
 * predecessor
 * and successor pointers in a doubly-linked list. For a circular doubly linked
 * list, the predecessor of the first element is the last element, and the
 * successor of the last element is the first element.
 * 
 * We want to do the transformation **in place**. After the transformation, the
 * left pointer of the tree node should point to its predecessor, and the right
 * pointer should point to its successor. You should return the pointer to the
 * smallest element of the linked list.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,5,1,3]
 *     
 *     
 *     **Output:** [1,2,3,4,5]
 *     
 * **Explanation:** The figure below shows the transformed BST. The solid line
 * indicates the successor relationship, while the dashed line means the
 * predecessor relationship.
 *     
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [2,1,3]
 *     **Output:** [1,2,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 2000]`.
 *   * `-1000 <= Node.val <= 1000`
 *   * All the values of the tree are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/convert-binary-search-tree-to-sorted-doubly-linked-list/
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

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/

class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,5,1,3]
    // Test case 2: [2,1,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
