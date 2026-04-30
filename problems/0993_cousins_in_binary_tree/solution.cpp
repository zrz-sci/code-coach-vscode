// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 993: Cousins in Binary Tree
 * Difficulty: Easy
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree with unique values and the values of two
 * different nodes of the tree `x` and `y`, return `true` _if the nodes
 * corresponding to the values_`x` _and_`y` _in the tree are**cousins** , or
 * _`false` _otherwise._
 * 
 * Two nodes of a binary tree are **cousins** if they have the same depth with
 * different parents.
 * 
 * Note that in a binary tree, the root node is at the depth `0`, and children
 * of
 * each depth `k` node are at the depth `k + 1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4], x = 4, y = 3
 *     **Output:** false
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,null,4,null,5], x = 5, y = 4
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,null,4], x = 2, y = 3
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[2, 100]`.
 *   * `1 <= Node.val <= 100`
 *   * Each node has a **unique** value.
 *   * `x != y`
 *   * `x` and `y` are exist in the tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/cousins-in-binary-tree/
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
    bool isCousins(TreeNode* root, int x, int y) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4]
    // Test case 2: 4
    // Test case 3: 3
    // Test case 4: [1,2,3,null,4,null,5]
    // Test case 5: 5
    // Test case 6: 4
    // Test case 7: [1,2,3,null,4]
    // Test case 8: 2
    // Test case 9: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
