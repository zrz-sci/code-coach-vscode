// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 549: Binary Tree Longest Consecutive Sequence II
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, return _the length of the longest
 * consecutive
 * path in the tree_.
 * 
 * A consecutive path is a path where the values of the consecutive nodes in
 * the
 * path differ by one. This path can be either increasing or decreasing.
 * 
 * * For example, `[1,2,3,4]` and `[4,3,2,1]` are both considered valid, but
 * the path `[1,2,4,3]` is not valid.
 * 
 * On the other hand, the path can be in the child-Parent-child order, where
 * not
 * necessarily be parent-child order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3]
 *     **Output:** 2
 *     **Explanation:** The longest consecutive path is [1, 2] or [2, 1].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [2,1,3]
 *     **Output:** 3
 *     **Explanation:** The longest consecutive path is [1, 2, 3] or [3, 2, 1].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 3 * 104]`.
 *   * `-3 * 104 <= Node.val <= 3 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-tree-longest-consecutive-sequence-ii/
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
    int longestConsecutive(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [2,1,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
