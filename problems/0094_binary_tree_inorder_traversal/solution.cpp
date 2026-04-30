// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 94: Binary Tree Inorder Traversal
 * Difficulty: Easy
 * Tags: Stack, Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, return _the inorder traversal of its
 * nodes '
 * values_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** root = [1,null,2,3]
 * 
 * **Output:** [1,3,2]
 * 
 * **Explanation:**
 * 
 * **Example 2:**
 * 
 * **Input:** root = [1,2,3,4,5,null,8,null,null,6,7,9]
 * 
 * **Output:** [4,2,6,5,7,1,3,9,8]
 * 
 * **Explanation:**
 * 
 * **Example 3:**
 * 
 * **Input:** root = []
 * 
 * **Output:** []
 * 
 * **Example 4:**
 * 
 * **Input:** root = [1]
 * 
 * **Output:** [1]
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
 * **Follow up:** Recursive solution is trivial, could you do it iteratively?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-inorder-traversal/
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
    vector<int> inorderTraversal(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,null,2,3]
    // Test case 2: [1,2,3,4,5,null,8,null,null,6,7,9]
    // Test case 3: []
    // Test case 4: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
