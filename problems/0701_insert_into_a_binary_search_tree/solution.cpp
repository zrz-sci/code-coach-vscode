// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 701: Insert into a Binary Search Tree
 * Difficulty: Medium
 * Tags: Tree, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * You are given the `root` node of a binary search tree (BST) and a `value` to
 * insert into the tree. Return _the root node of the BST after the
 * insertion_. It
 * is **guaranteed** that the new value does not exist in the original BST.
 * 
 * **Notice** that there may exist multiple valid ways for the insertion, as
 * long
 * as the tree remains a BST after insertion. You can return **any of them**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,7,1,3], val = 5
 *     **Output:** [4,2,7,1,3,5]
 *     **Explanation:** Another accepted tree is:
 *     
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [40,20,60,10,30,50,70], val = 25
 *     **Output:** [40,20,60,10,30,50,70,null,null,25]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [4,2,7,1,3,null,null,null,null,null,null], val = 5
 *     **Output:** [4,2,7,1,3,5]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree will be in the range `[0, 104]`.
 *   * `-108 <= Node.val <= 108`
 *   * All the values `Node.val` are **unique**.
 *   * `-108 <= val <= 108`
 *   * It's **guaranteed** that `val` does not exist in the original BST.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/insert-into-a-binary-search-tree/
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
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,7,1,3]
    // Test case 2: 5
    // Test case 3: [40,20,60,10,30,50,70]
    // Test case 4: 25
    // Test case 5: [4,2,7,1,3,null,null,null,null,null,null]
    // Test case 6: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
