// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1110: Delete Nodes And Return Forest
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Array, Hash Table, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, each node in the tree has a distinct
 * value.
 * 
 * After deleting all nodes with a value in `to_delete`, we are left with a
 * forest
 * (a disjoint union of trees).
 * 
 * Return the roots of the trees in the remaining forest. You may return the
 * result
 * in any order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,5,6,7], to_delete = [3,5]
 *     **Output:** [[1,2,null,4],[6],[7]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,4,null,3], to_delete = [3]
 *     **Output:** [[1,2,4]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the given tree is at most `1000`.
 *   * Each node has a distinct value between `1` and `1000`.
 *   * `to_delete.length <= 1000`
 *   * `to_delete` contains distinct values between `1` and `1000`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/delete-nodes-and-return-forest/
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
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,6,7]
    // Test case 2: [3,5]
    // Test case 3: [1,2,4,null,3]
    // Test case 4: [3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
