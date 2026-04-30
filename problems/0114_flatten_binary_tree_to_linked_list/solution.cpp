// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 114: Flatten Binary Tree to Linked List
 * Difficulty: Medium
 * Tags: Stack, Tree, Depth-First Search, Linked List, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, flatten the tree into a "linked list":
 * 
 * * The "linked list" should use the same `TreeNode` class where the `right`
 * child pointer points to the next node in the list and the `left` child
 * pointer is always `null`.
 * * The "linked list" should be in the same order as a
 * [**pre-order****traversal**](https://en.wikipedia.org/wiki/Tree_traversal#Pre-order,_NLR)
 * of the binary tree.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,5,3,4,null,6]
 *     **Output:** [1,null,2,null,3,null,4,null,5,null,6]
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
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [0]
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 2000]`.
 *   * `-100 <= Node.val <= 100`
 * 
 * 
 * 
 * **Follow up:** Can you flatten the tree in-place (with `O(1)` extra space)?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flatten-binary-tree-to-linked-list/
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

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    void flatten(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,5,3,4,null,6]
    // Test case 2: []
    // Test case 3: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
