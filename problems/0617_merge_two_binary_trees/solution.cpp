// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 617: Merge Two Binary Trees
 * Difficulty: Easy
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * You are given two binary trees `root1` and `root2`.
 * 
 * Imagine that when you put one of them to cover the other, some nodes of the
 * two
 * trees are overlapped while the others are not. You need to merge the two
 * trees
 * into a new binary tree. The merge rule is that if two nodes overlap, then
 * sum
 * node values up as the new value of the merged node. Otherwise, the NOT null
 * node
 * will be used as the node of the new tree.
 * 
 * Return _the merged tree_.
 * 
 * **Note:** The merging process must start from the root nodes of both trees.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root1 = [1,3,2,5], root2 = [2,1,3,null,4,null,7]
 *     **Output:** [3,4,5,5,4,null,7]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root1 = [1], root2 = [1,2]
 *     **Output:** [2,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in both trees is in the range `[0, 2000]`.
 *   * `-104 <= Node.val <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/merge-two-binary-trees/
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
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2,5]
    // Test case 2: [2,1,3,null,4,null,7]
    // Test case 3: [1]
    // Test case 4: [1,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
