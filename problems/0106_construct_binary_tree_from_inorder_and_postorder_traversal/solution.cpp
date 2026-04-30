// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 106: Construct Binary Tree from Inorder and Postorder Traversal
 * Difficulty: Medium
 * Tags: Tree, Array, Hash Table, Divide and Conquer, Binary Tree
 *
 * Problem Description:
 * Given two integer arrays `inorder` and `postorder` where `inorder` is the
 * inorder traversal of a binary tree and `postorder` is the postorder
 * traversal of
 * the same tree, construct and return _the binary tree_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** inorder = [9,3,15,20,7], postorder = [9,15,7,20,3]
 *     **Output:** [3,9,20,null,null,15,7]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** inorder = [-1], postorder = [-1]
 *     **Output:** [-1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= inorder.length <= 3000`
 *   * `postorder.length == inorder.length`
 *   * `-3000 <= inorder[i], postorder[i] <= 3000`
 *   * `inorder` and `postorder` consist of **unique** values.
 *   * Each value of `postorder` also appears in `inorder`.
 *   * `inorder` is **guaranteed** to be the inorder traversal of the tree.
 *   * `postorder` is **guaranteed** to be the postorder traversal of the tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/construct-binary-tree-from-inorder-and-postorder-traversal/
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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [9,3,15,20,7]
    // Test case 2: [9,15,7,20,3]
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
