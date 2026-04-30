// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 2236: Root Equals Sum of Children
 * Difficulty: Easy
 * Tags: Tree, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a **binary tree** that consists of exactly `3`
 * nodes: the root, its left child, and its right child.
 * 
 * Return `true` _if the value of the root is equal to the**sum** of the
 * values of
 * its two children, or _`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [10,4,6]
 *     **Output:** true
 * **Explanation:** The values of the root, its left child, and its right
 * child are 10, 4, and 6, respectively.
 *     10 is equal to 4 + 6, so we return true.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [5,3,1]
 *     **Output:** false
 * **Explanation:** The values of the root, its left child, and its right
 * child are 5, 3, and 1, respectively.
 *     5 is not equal to 3 + 1, so we return false.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The tree consists only of the root, its left child, and its right child.
 *   * `-100 <= Node.val <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/root-equals-sum-of-children/
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
    bool checkTree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,4,6]
    // Test case 2: [5,3,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
