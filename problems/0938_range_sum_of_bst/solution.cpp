// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 938: Range Sum of BST
 * Difficulty: Easy
 * Tags: Tree, Depth-First Search, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * Given the `root` node of a binary search tree and two integers `low` and
 * `high`,
 * return _the sum of values of all nodes with a value in the**inclusive**
 * range
 * _`[low, high]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [10,5,15,3,7,null,18], low = 7, high = 15
 *     **Output:** 32
 * **Explanation:** Nodes 7, 10, and 15 are in the range [7, 15]. 7 + 10 + 15
 * = 32.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10
 *     **Output:** 23
 * **Explanation:** Nodes 6, 7, and 10 are in the range [6, 10]. 6 + 7 + 10 =
 * 23.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 2 * 104]`.
 *   * `1 <= Node.val <= 105`
 *   * `1 <= low <= high <= 105`
 *   * All `Node.val` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/range-sum-of-bst/
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
    int rangeSumBST(TreeNode* root, int low, int high) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,5,15,3,7,null,18]
    // Test case 2: 7
    // Test case 3: 15
    // Test case 4: [10,5,15,3,7,13,18,1,null,6]
    // Test case 5: 6
    // Test case 6: 10

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
