// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 337: House Robber III
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Dynamic Programming, Binary Tree
 *
 * Problem Description:
 * The thief has found himself a new place for his thievery again. There is
 * only
 * one entrance to this area, called `root`.
 * 
 * Besides the `root`, each house has one and only one parent house. After a
 * tour,
 * the smart thief realized that all houses in this place form a binary tree.
 * It
 * will automatically contact the police if **two directly-linked houses were
 * broken into on the same night**.
 * 
 * Given the `root` of the binary tree, return _the maximum amount of money the
 * thief can rob**without alerting the police**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,2,3,null,3,null,1]
 *     **Output:** 7
 * **Explanation:** Maximum amount of money the thief can rob = 3 + 3 + 1 = 7.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [3,4,5,1,3,null,1]
 *     **Output:** 9
 *     **Explanation:** Maximum amount of money the thief can rob = 4 + 5 = 9.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 104]`.
 *   * `0 <= Node.val <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/house-robber-iii/
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
    int rob(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,3,null,3,null,1]
    // Test case 2: [3,4,5,1,3,null,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
