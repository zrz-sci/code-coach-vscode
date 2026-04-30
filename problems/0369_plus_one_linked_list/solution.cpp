// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 369: Plus One Linked List
 * Difficulty: Medium
 * Tags: Linked List, Math
 *
 * Problem Description:
 * Given a non-negative integer represented as a linked list of digits, _plus
 * one
 * to the integer_.
 * 
 * The digits are stored such that the most significant digit is at the `head`
 * of
 * the list.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3]
 *     **Output:** [1,2,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [0]
 *     **Output:** [1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the linked list is in the range `[1, 100]`.
 *   * `0 <= Node.val <= 9`
 * * The number represented by the linked list does not contain leading zeros
 * except for the zero itself.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/plus-one-linked-list/
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
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* plusOne(ListNode* head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
