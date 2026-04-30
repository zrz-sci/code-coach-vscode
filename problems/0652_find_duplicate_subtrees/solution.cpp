// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 652: Find Duplicate Subtrees
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Hash Table, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, return all **duplicate subtrees**.
 * 
 * For each kind of duplicate subtrees, you only need to return the root node
 * of
 * any **one** of them.
 * 
 * Two trees are **duplicate** if they have the **same structure** with the
 * **same
 * node values**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,null,2,4,null,null,4]
 *     **Output:** [[2,4],[4]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [2,1,1]
 *     **Output:** [[1]]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [2,2,2,3,null,3,null]
 *     **Output:** [[2,3],[3]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of the nodes in the tree will be in the range `[1, 5000]`
 *   * `-200 <= Node.val <= 200`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-duplicate-subtrees/
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
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,null,2,4,null,null,4]
    // Test case 2: [2,1,1]
    // Test case 3: [2,2,2,3,null,3,null]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
