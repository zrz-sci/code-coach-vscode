// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 814: Binary Tree Pruning
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, return _the same tree where every
 * subtree (of
 * the given tree) not containing a_`1` _has been removed_.
 * 
 * A subtree of a node `node` is `node` plus every node that is a descendant of
 * `node`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,null,0,0,1]
 *     **Output:** [1,null,0,null,1]
 *     **Explanation:** 
 * Only the red nodes satisfy the property "every subtree not containing a 1".
 *     The diagram on the right represents the answer.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,0,1,0,0,0,1]
 *     **Output:** [1,null,1,null,1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,1,0,1,1,0,1,0]
 *     **Output:** [1,1,0,1,1,null,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 200]`.
 *   * `Node.val` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-pruning/
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
    TreeNode* pruneTree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,null,0,0,1]
    // Test case 2: [1,0,1,0,0,0,1]
    // Test case 3: [1,1,0,1,1,0,1,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
