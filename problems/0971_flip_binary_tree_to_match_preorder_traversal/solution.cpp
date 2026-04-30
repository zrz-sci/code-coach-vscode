// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 971: Flip Binary Tree To Match Preorder Traversal
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a binary tree with `n` nodes, where each node is
 * uniquely assigned a value from `1` to `n`. You are also given a sequence of
 * `n`
 * values `voyage`, which is the **desired** [**pre-order
 * traversal**](https://en.wikipedia.org/wiki/Tree_traversal#Pre-order) of the
 * binary tree.
 * 
 * Any node in the binary tree can be **flipped** by swapping its left and
 * right
 * subtrees. For example, flipping node 1 will have the following effect:
 * 
 * Flip the **smallest** number of nodes so that the **pre-order traversal**
 * of the
 * tree **matches** `voyage`.
 * 
 * Return _a list of the values of all**flipped** nodes. You may return the
 * answer
 * in **any order**. If it is **impossible** to flip the nodes in the tree to
 * make
 * the pre-order traversal match _`voyage` _, return the list_`[-1]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2], voyage = [2,1]
 *     **Output:** [-1]
 * **Explanation:** It is impossible to flip the nodes such that the pre-order
 * traversal matches voyage.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3], voyage = [1,3,2]
 *     **Output:** [1]
 * **Explanation:** Flipping node 1 swaps nodes 2 and 3, so the pre-order
 * traversal matches voyage.
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3], voyage = [1,2,3]
 *     **Output:** []
 * **Explanation:** The tree's pre-order traversal already matches voyage, so
 * no nodes need to be flipped.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is `n`.
 *   * `n == voyage.length`
 *   * `1 <= n <= 100`
 *   * `1 <= Node.val, voyage[i] <= n`
 *   * All the values in the tree are **unique**.
 *   * All the values in `voyage` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flip-binary-tree-to-match-preorder-traversal/
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
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2]
    // Test case 2: [2,1]
    // Test case 3: [1,2,3]
    // Test case 4: [1,3,2]
    // Test case 5: [1,2,3]
    // Test case 6: [1,2,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
