// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 117: Populating Next Right Pointers in Each Node II
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Linked List, Binary Tree
 *
 * Problem Description:
 * Given a binary tree
 * 
 *     
 *     
 *     struct Node {
 *       int val;
 *       Node *left;
 *       Node *right;
 *       Node *next;
 *     }
 *     
 * 
 * Populate each next pointer to point to its next right node. If there is no
 * next
 * right node, the next pointer should be set to `NULL`.
 * 
 * Initially, all next pointers are set to `NULL`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,5,null,7]
 *     **Output:** [1,#,2,3,#,4,5,7,#]
 * **Explanation:** Given the above binary tree (Figure A), your function
 * should populate each next pointer to point to its next right node, just like
 * in Figure B. The serialized output is in level order as connected by the
 * next pointers, with '#' signifying the end of each level.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = []
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 6000]`.
 *   * `-100 <= Node.val <= 100`
 * 
 * 
 * 
 * **Follow-up:**
 * 
 *   * You may only use constant extra space.
 * * The recursive approach is fine. You may assume implicit stack space does
 * not count as extra space for this problem.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/populating-next-right-pointers-in-each-node-ii/
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
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,null,7]
    // Test case 2: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
