// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 156: Binary Tree Upside Down
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, turn the tree upside down and return
 * _the new
 * root_.
 * 
 * You can turn a binary tree upside down with the following steps:
 * 
 *   1. The original left child becomes the new root.
 *   2. The original root becomes the new right child.
 *   3. The original right child becomes the new left child.
 * 
 * The mentioned steps are done level by level. It is **guaranteed** that every
 * right node has a sibling (a left node with the same parent) and has no
 * children.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,5]
 *     **Output:** [4,5,2,null,null,3,1]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = []
 *     **Output:** []
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1]
 *     **Output:** [1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree will be in the range `[0, 10]`.
 *   * `1 <= Node.val <= 10`
 * * Every right node in the tree has a sibling (a left node that shares the
 * same parent).
 *   * Every right node in the tree has no children.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-upside-down/
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
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5]
    // Test case 2: []
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
