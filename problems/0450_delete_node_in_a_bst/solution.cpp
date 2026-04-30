// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 450: Delete Node in a BST
 * Difficulty: Medium
 * Tags: Tree, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * Given a root node reference of a BST and a key, delete the node with the
 * given
 * key in the BST. Return _the**root node reference** (possibly updated) of the
 * BST_.
 * 
 * Basically, the deletion can be divided into two stages:
 * 
 *   1. Search for a node to remove.
 *   2. If the node is found, delete the node.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [5,3,6,2,4,null,7], key = 3
 *     **Output:** [5,4,6,2,null,null,7]
 * **Explanation:** Given key to delete is 3. So we find the node with value 3
 * and delete it.
 *     One valid answer is [5,4,6,2,null,null,7], shown in the above BST.
 * Please notice that another valid answer is [5,2,6,null,4,null,7] and it's
 * also accepted.
 *     
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [5,3,6,2,4,null,7], key = 0
 *     **Output:** [5,3,6,2,4,null,7]
 *     **Explanation:** The tree does not contain a node with value = 0.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [], key = 0
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 104]`.
 *   * `-105 <= Node.val <= 105`
 *   * Each node has a **unique** value.
 *   * `root` is a valid binary search tree.
 *   * `-105 <= key <= 105`
 * 
 * 
 * 
 * **Follow up:** Could you solve it with time complexity `O(height of tree)`?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/delete-node-in-a-bst/
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
    TreeNode* deleteNode(TreeNode* root, int key) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,3,6,2,4,null,7]
    // Test case 2: 3
    // Test case 3: [5,3,6,2,4,null,7]
    // Test case 4: 0
    // Test case 5: []
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
