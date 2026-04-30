// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 671: Second Minimum Node In a Binary Tree
 * Difficulty: Easy
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * Given a non-empty special binary tree consisting of nodes with the
 * non-negative
 * value, where each node in this tree has exactly `two` or `zero` sub-node.
 * If the
 * node has two sub-nodes, then this node's value is the smaller value among
 * its
 * two sub-nodes. More formally, the property `root.val = min(root.left.val,
 * root.right.val)` always holds.
 * 
 * Given such a binary tree, you need to output the **second minimum** value
 * in the
 * set made of all the nodes' value in the whole tree.
 * 
 * If no such second minimum value exists, output -1 instead.
 * 
 * 
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [2,2,5,null,null,5,7]
 *     **Output:** 5
 * **Explanation:** The smallest value is 2, the second smallest value is 5.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [2,2,2]
 *     **Output:** -1
 * **Explanation:** The smallest value is 2, but there isn't any second
 * smallest value.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 25]`.
 *   * `1 <= Node.val <= 231 - 1`
 * * `root.val == min(root.left.val, root.right.val)` for each internal node
 * of the tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/second-minimum-node-in-a-binary-tree/
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
    int findSecondMinimumValue(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,2,5,null,null,5,7]
    // Test case 2: [2,2,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
