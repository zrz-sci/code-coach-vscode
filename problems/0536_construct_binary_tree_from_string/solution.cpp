// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 536: Construct Binary Tree from String
 * Difficulty: Medium
 * Tags: Stack, Tree, Depth-First Search, String, Binary Tree
 *
 * Problem Description:
 * You need to construct a binary tree from a string consisting of parenthesis
 * and
 * integers.
 * 
 * The whole input represents a binary tree. It contains an integer followed by
 * zero, one or two pairs of parenthesis. The integer represents the root's
 * value
 * and a pair of parenthesis contains a child binary tree with the same
 * structure.
 * 
 * You always start to construct the **left** child node of the parent first
 * if it
 * exists.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "4(2(3)(1))(6(5))"
 *     **Output:** [4,2,6,3,1,5]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "4(2(3)(1))(6(5)(7))"
 *     **Output:** [4,2,6,3,1,5,7]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "-4(2(3)(1))(6(5)(7))"
 *     **Output:** [-4,2,6,3,1,5,7]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 3 * 104`
 *   * `s` consists of digits, `'('`, `')'`, and `'-'` only.
 *   * All numbers in the tree have value **at most** than `230`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/construct-binary-tree-from-string/
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
    TreeNode* str2tree(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "4(2(3)(1))(6(5))"
    // Test case 2: "4(2(3)(1))(6(5)(7))"
    // Test case 3: "-4(2(3)(1))(6(5)(7))"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
