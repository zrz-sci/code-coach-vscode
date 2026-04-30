// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 99: Recover Binary Search Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a binary search tree (BST), where the values of
 * **exactly** two nodes of the tree were swapped by mistake. _Recover the tree
 * without changing its structure_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,3,null,null,2]
 *     **Output:** [3,1,null,null,2]
 * **Explanation:** 3 cannot be a left child of 1 because 3 > 1. Swapping 1
 * and 3 makes the BST valid.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [3,1,4,null,null,2]
 *     **Output:** [2,1,4,null,null,3]
 * **Explanation:** 2 cannot be in the right subtree of 3 because 2 < 3.
 * Swapping 2 and 3 makes the BST valid.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[2, 1000]`.
 *   * `-231 <= Node.val <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:** A solution using `O(n)` space is pretty straight-forward.
 * Could
 * you devise a constant `O(1)` space solution?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/recover-binary-search-tree/
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
    void recoverTree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,null,null,2]
    // Test case 2: [3,1,4,null,null,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
