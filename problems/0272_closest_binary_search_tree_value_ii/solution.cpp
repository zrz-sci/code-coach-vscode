// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 272: Closest Binary Search Tree Value II
 * Difficulty: Hard
 * Tags: Stack, Tree, Depth-First Search, Binary Search Tree, Two Pointers, Binary Tree, Heap (Priority Queue)
 *
 * Problem Description:
 * Given the `root` of a binary search tree, a `target` value, and an integer
 * `k`,
 * return _the_`k` _values in the BST that are closest to the_ `target`. You
 * may
 * return the answer in **any order**.
 * 
 * You are **guaranteed** to have only one unique set of `k` values in the BST
 * that
 * are closest to the `target`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,5,1,3], target = 3.714286, k = 2
 *     **Output:** [4,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1], target = 0.000000, k = 1
 *     **Output:** [1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is `n`.
 *   * `1 <= k <= n <= 104`.
 *   * `0 <= Node.val <= 109`
 *   * `-109 <= target <= 109`
 * 
 * 
 * 
 * **Follow up:** Assume that the BST is balanced. Could you solve it in less
 * than
 * `O(n)` runtime (where `n = total nodes`)?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/closest-binary-search-tree-value-ii/
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
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,5,1,3]
    // Test case 2: 3.714286
    // Test case 3: 2
    // Test case 4: [1]
    // Test case 5: 0.000000
    // Test case 6: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
