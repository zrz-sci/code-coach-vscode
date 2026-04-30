// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 572: Subtree of Another Tree
 * Difficulty: Easy
 * Tags: Tree, Depth-First Search, Binary Tree, String Matching, Hash Function
 *
 * Problem Description:
 * Given the roots of two binary trees `root` and `subRoot`, return `true` if
 * there
 * is a subtree of `root` with the same structure and node values of` subRoot`
 * and
 * `false` otherwise.
 * 
 * A subtree of a binary tree `tree` is a tree that consists of a node in
 * `tree`
 * and all of this node's descendants. The tree `tree` could also be
 * considered as
 * a subtree of itself.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,4,5,1,2], subRoot = [4,1,2]
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [3,4,5,1,2,null,null,null,null,0], subRoot = [4,1,2]
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the `root` tree is in the range `[1, 2000]`.
 *   * The number of nodes in the `subRoot` tree is in the range `[1, 1000]`.
 *   * `-104 <= root.val <= 104`
 *   * `-104 <= subRoot.val <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subtree-of-another-tree/
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
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,4,5,1,2]
    // Test case 2: [4,1,2]
    // Test case 3: [3,4,5,1,2,null,null,null,null,0]
    // Test case 4: [4,1,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
