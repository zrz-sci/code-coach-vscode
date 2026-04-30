// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 988: Smallest String Starting From Leaf
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, String, Backtracking, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a binary tree where each node has a value in the
 * range `[0, 25]` representing the letters `'a'` to `'z'`.
 * 
 * Return _the**lexicographically smallest** string that starts at a leaf of
 * this
 * tree and ends at the root_.
 * 
 * As a reminder, any shorter prefix of a string is **lexicographically
 * smaller**.
 * 
 *   * For example, `"ab"` is lexicographically smaller than `"aba"`.
 * 
 * A leaf of a node is a node that has no children.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [0,1,2,3,4,3,4]
 *     **Output:** "dba"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [25,1,3,1,3,0,2]
 *     **Output:** "adz"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [2,2,1,null,1,0,null,0]
 *     **Output:** "abc"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 8500]`.
 *   * `0 <= Node.val <= 25`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/smallest-string-starting-from-leaf/
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
    string smallestFromLeaf(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,2,3,4,3,4]
    // Test case 2: [25,1,3,1,3,0,2]
    // Test case 3: [2,2,1,null,1,0,null,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
