// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 105: Construct Binary Tree from Preorder and Inorder Traversal
 * Difficulty: Medium
 * Tags: Tree, Array, Hash Table, Divide and Conquer, Binary Tree
 *
 * Problem Description:
 * Given two integer arrays `preorder` and `inorder` where `preorder` is the
 * preorder traversal of a binary tree and `inorder` is the inorder traversal
 * of
 * the same tree, construct and return _the binary tree_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
 *     **Output:** [3,9,20,null,null,15,7]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** preorder = [-1], inorder = [-1]
 *     **Output:** [-1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= preorder.length <= 3000`
 *   * `inorder.length == preorder.length`
 *   * `-3000 <= preorder[i], inorder[i] <= 3000`
 *   * `preorder` and `inorder` consist of **unique** values.
 *   * Each value of `inorder` also appears in `preorder`.
 *   * `preorder` is **guaranteed** to be the preorder traversal of the tree.
 *   * `inorder` is **guaranteed** to be the inorder traversal of the tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,9,20,15,7]
    // Test case 2: [9,3,15,20,7]
    // Test case 3: [-1]
    // Test case 4: [-1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
