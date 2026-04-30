// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 222: Count Complete Tree Nodes
 * Difficulty: Easy
 * Tags: Bit Manipulation, Tree, Binary Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a **complete** binary tree, return the number of the
 * nodes
 * in the tree.
 * 
 * According to
 * **[Wikipedia](http://en.wikipedia.org/wiki/Binary_tree#Types_of_binary_trees)**
 * , every level, except possibly the last, is completely filled in a complete
 * binary tree, and all nodes in the last level are as far left as possible.
 * It can
 * have between `1` and `2h` nodes inclusive at the last level `h`.
 * 
 * Design an algorithm that runs in less than `O(n)` time complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,5,6]
 *     **Output:** 6
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = []
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 5 * 104]`.
 *   * `0 <= Node.val <= 5 * 104`
 *   * The tree is guaranteed to be **complete**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-complete-tree-nodes/
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
    int countNodes(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,6]
    // Test case 2: []
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
