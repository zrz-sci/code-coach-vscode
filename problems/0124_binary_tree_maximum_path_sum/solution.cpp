// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 124: Binary Tree Maximum Path Sum
 * Difficulty: Hard
 * Tags: Tree, Depth-First Search, Dynamic Programming, Binary Tree
 *
 * Problem Description:
 * A **path** in a binary tree is a sequence of nodes where each pair of
 * adjacent
 * nodes in the sequence has an edge connecting them. A node can only appear
 * in the
 * sequence **at most once**. Note that the path does not need to pass through
 * the
 * root.
 * 
 * The **path sum** of a path is the sum of the node's values in the path.
 * 
 * Given the `root` of a binary tree, return _the maximum**path sum** of any
 * **non-
 * empty** path_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3]
 *     **Output:** 6
 * **Explanation:** The optimal path is 2 -> 1 -> 3 with a path sum of 2 + 1 +
 * 3 = 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [-10,9,20,null,null,15,7]
 *     **Output:** 42
 * **Explanation:** The optimal path is 15 -> 20 -> 7 with a path sum of 15 +
 * 20 + 7 = 42.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 3 * 104]`.
 *   * `-1000 <= Node.val <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-maximum-path-sum/
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
    int maxPathSum(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [-10,9,20,null,null,15,7]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
