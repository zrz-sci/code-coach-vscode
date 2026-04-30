// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 538: Convert BST to Greater Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a Binary Search Tree (BST), convert it to a Greater Tree
 * such that every key of the original BST is changed to the original key plus
 * the
 * sum of all keys greater than the original key in BST.
 * 
 * As a reminder, a _binary search tree_ is a tree that satisfies these
 * constraints:
 * 
 * * The left subtree of a node contains only nodes with keys **less than**
 * the node's key.
 * * The right subtree of a node contains only nodes with keys **greater
 * than** the node's key.
 *   * Both the left and right subtrees must also be binary search trees.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
 *     **Output:** [30,36,21,36,35,26,15,null,null,null,33,null,null,null,8]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [0,null,1]
 *     **Output:** [1,null,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 104]`.
 *   * `-104 <= Node.val <= 104`
 *   * All the values in the tree are **unique**.
 *   * `root` is guaranteed to be a valid binary search tree.
 * 
 * 
 * 
 * **Note:** This question is the same as 1038:
 * <https://leetcode.com/problems/binary-search-tree-to-greater-sum-tree/>
 * 
 * 
 *
 * Link: https://leetcode.com/problems/convert-bst-to-greater-tree/
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
    TreeNode* convertBST(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
    // Test case 2: [0,null,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
