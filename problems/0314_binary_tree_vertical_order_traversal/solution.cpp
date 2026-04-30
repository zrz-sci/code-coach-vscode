// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 314: Binary Tree Vertical Order Traversal
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Hash Table, Binary Tree, Sorting
 *
 * Problem Description:
 * Given the `root` of a binary tree, return _**the vertical order traversal**
 * of
 * its nodes' values_. (i.e., from top to bottom, column by column).
 * 
 * If two nodes are in the same row and column, the order should be from
 * **left to
 * right**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,9,20,null,null,15,7]
 *     **Output:** [[9],[3,15],[20],[7]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [3,9,8,4,0,1,7]
 *     **Output:** [[4],[9],[3,0,1],[8],[7]]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 * **Input:** root =
 * [1,2,3,4,10,9,11,null,5,null,null,null,null,null,null,null,6]
 *     **Output:** [[4],[2,5],[1,10,9,6],[3],[11]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 100]`.
 *   * `-100 <= Node.val <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-vertical-order-traversal/
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
    vector<vector<int>> verticalOrder(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,9,20,null,null,15,7]
    // Test case 2: [3,9,8,4,0,1,7]
    // Test case 3: [1,2,3,4,10,9,11,null,5,null,null,null,null,null,null,null,6]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
