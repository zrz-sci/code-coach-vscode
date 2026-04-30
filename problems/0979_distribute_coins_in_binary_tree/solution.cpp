// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 979: Distribute Coins in Binary Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a binary tree with `n` nodes where each `node`
 * in
 * the tree has `node.val` coins. There are `n` coins in total throughout the
 * whole
 * tree.
 * 
 * In one move, we may choose two adjacent nodes and move one coin from one
 * node to
 * another. A move may be from parent to child, or from child to parent.
 * 
 * Return _the**minimum** number of moves required to make every node have
 * **exactly** one coin_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,0,0]
 *     **Output:** 2
 * **Explanation:** From the root of the tree, we move one coin to its left
 * child, and one coin to its right child.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [0,3,0]
 *     **Output:** 3
 * **Explanation:** From the left child of the root, we move two coins to the
 * root [taking two moves]. Then, we move one coin from the root of the tree to
 * the right child.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is `n`.
 *   * `1 <= n <= 100`
 *   * `0 <= Node.val <= n`
 *   * The sum of all `Node.val` is `n`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/distribute-coins-in-binary-tree/
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
    int distributeCoins(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,0,0]
    // Test case 2: [0,3,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
