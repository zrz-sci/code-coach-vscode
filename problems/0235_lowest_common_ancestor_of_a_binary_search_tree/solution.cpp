// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 235: Lowest Common Ancestor of a Binary Search Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * Given a binary search tree (BST), find the lowest common ancestor (LCA)
 * node of
 * two given nodes in the BST.
 * 
 * According to the [definition of LCA on
 * Wikipedia](https://en.wikipedia.org/wiki/Lowest_common_ancestor): "The
 * lowest
 * common ancestor is defined between two nodes `p` and `q` as the lowest node
 * in
 * `T` that has both `p` and `q` as descendants (where we allow **a node to be
 * a
 * descendant of itself**)."
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 8
 *     **Output:** 6
 *     **Explanation:** The LCA of nodes 2 and 8 is 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 4
 *     **Output:** 2
 * **Explanation:** The LCA of nodes 2 and 4 is 2, since a node can be a
 * descendant of itself according to the LCA definition.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [2,1], p = 2, q = 1
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[2, 105]`.
 *   * `-109 <= Node.val <= 109`
 *   * All `Node.val` are **unique**.
 *   * `p != q`
 *   * `p` and `q` will exist in the BST.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/
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
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [6,2,8,0,4,7,9,null,null,3,5]
    // Test case 2: 2
    // Test case 3: 8
    // Test case 4: [6,2,8,0,4,7,9,null,null,3,5]
    // Test case 5: 2
    // Test case 6: 4
    // Test case 7: [2,1]
    // Test case 8: 2
    // Test case 9: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
