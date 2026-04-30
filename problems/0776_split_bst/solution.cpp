// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 776: Split BST
 * Difficulty: Medium
 * Tags: Tree, Binary Search Tree, Recursion, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary search tree (BST) and an integer `target`,
 * split
 * the tree into two subtrees where the first subtree has nodes that are all
 * smaller or equal to the target value, while the second subtree has all nodes
 * that are greater than the target value. It is not necessarily the case that
 * the
 * tree contains a node with the value `target`.
 * 
 * Additionally, most of the structure of the original tree should remain.
 * Formally, for any child `c` with parent `p` in the original tree, if they
 * are
 * both in the same subtree after the split, then node `c` should still have
 * the
 * parent `p`.
 * 
 * Return _an array of the two roots of the two subtrees in order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,6,1,3,5,7], target = 2
 *     **Output:** [[2,1],[4,3,6,null,null,5,7]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1], target = 1
 *     **Output:** [[1],[]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 50]`.
 *   * `0 <= Node.val, target <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/split-bst/
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
    vector<TreeNode*> splitBST(TreeNode* root, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,6,1,3,5,7]
    // Test case 2: 2
    // Test case 3: [1]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
