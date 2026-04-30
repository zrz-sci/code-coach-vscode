// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 113: Path Sum II
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Backtracking, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree and an integer `targetSum`, return
 * _all**root-
 * to-leaf** paths where the sum of the node values in the path equals
 * _`targetSum`
 * _. Each path should be returned as a list of the node**values** , not node
 * references_.
 * 
 * A **root-to-leaf** path is a path starting from the root and ending at any
 * leaf
 * node. A **leaf** is a node with no children.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
 *     **Output:** [[5,4,11,2],[5,8,4,5]]
 *     **Explanation:** There are two paths whose sum equals targetSum:
 *     5 + 4 + 11 + 2 = 22
 *     5 + 8 + 4 + 5 = 22
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3], targetSum = 5
 *     **Output:** []
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2], targetSum = 0
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 5000]`.
 *   * `-1000 <= Node.val <= 1000`
 *   * `-1000 <= targetSum <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/path-sum-ii/
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
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,4,8,11,null,13,4,7,2,null,null,5,1]
    // Test case 2: 22
    // Test case 3: [1,2,3]
    // Test case 4: 5
    // Test case 5: [1,2]
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
