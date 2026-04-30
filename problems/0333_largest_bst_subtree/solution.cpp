// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 333: Largest BST Subtree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Search Tree, Dynamic Programming, Binary Tree
 *
 * Problem Description:
 * Given the root of a binary tree, find the largest subtree, which is also a
 * Binary Search Tree (BST), where the largest means subtree has the largest
 * number
 * of nodes.
 * 
 * A **Binary Search Tree (BST)** is a tree in which all the nodes follow the
 * below-mentioned properties:
 * 
 * * The left subtree values are less than the value of their parent (root)
 * node's value.
 * * The right subtree values are greater than the value of their parent
 * (root) node's value.
 * 
 * **Note:** A subtree must include all of its descendants.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * ****
 * 
 *     
 *     
 *     **Input:** root = [10,5,15,1,8,null,7]
 *     **Output:** 3
 * **Explanation:** The Largest BST Subtree in this case is the highlighted
 * one. The return value is the subtree's size, which is 3.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,7,2,3,5,null,2,null,null,null,null,null,1]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 104]`.
 *   * `-104 <= Node.val <= 104`
 * 
 * 
 * 
 * **Follow up:** Can you figure out ways to solve it with `O(n)` time
 * complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-bst-subtree/
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
    int largestBSTSubtree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,5,15,1,8,null,7]
    // Test case 2: [4,2,7,2,3,5,null,2,null,null,null,null,null,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
