// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 623: Add One Row to Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree and two integers `val` and `depth`, add a
 * row
 * of nodes with value `val` at the given depth `depth`.
 * 
 * Note that the `root` node is at depth `1`.
 * 
 * The adding rule is:
 * 
 * * Given the integer `depth`, for each not null tree node `cur` at the depth
 * `depth - 1`, create two tree nodes with value `val` as `cur`'s left subtree
 * root and right subtree root.
 * * `cur`'s original left subtree should be the left subtree of the new left
 * subtree root.
 * * `cur`'s original right subtree should be the right subtree of the new
 * right subtree root.
 * * If `depth == 1` that means there is no depth `depth - 1` at all, then
 * create a tree node with value `val` as the new root of the whole original
 * tree, and the original tree is the new root's left subtree.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,6,3,1,5], val = 1, depth = 2
 *     **Output:** [4,1,1,2,null,null,6,3,1,5]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,null,3,1], val = 1, depth = 3
 *     **Output:** [4,2,null,1,1,3,null,null,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 104]`.
 *   * The depth of the tree is in the range `[1, 104]`.
 *   * `-100 <= Node.val <= 100`
 *   * `-105 <= val <= 105`
 *   * `1 <= depth <= the depth of tree + 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-one-row-to-tree/
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
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,6,3,1,5]
    // Test case 2: 1
    // Test case 3: 2
    // Test case 4: [4,2,null,3,1]
    // Test case 5: 1
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
