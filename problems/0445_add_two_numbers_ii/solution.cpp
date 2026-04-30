// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 445: Add Two Numbers II
 * Difficulty: Medium
 * Tags: Stack, Linked List, Math
 *
 * Problem Description:
 * You are given two **non-empty** linked lists representing two non-negative
 * integers. The most significant digit comes first and each of their nodes
 * contains a single digit. Add the two numbers and return the sum as a linked
 * list.
 * 
 * You may assume the two numbers do not contain any leading zero, except the
 * number 0 itself.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** l1 = [7,2,4,3], l2 = [5,6,4]
 *     **Output:** [7,8,0,7]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** l1 = [2,4,3], l2 = [5,6,4]
 *     **Output:** [8,0,7]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** l1 = [0], l2 = [0]
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in each linked list is in the range `[1, 100]`.
 *   * `0 <= Node.val <= 9`
 * * It is guaranteed that the list represents a number that does not have
 * leading zeros.
 * 
 * 
 * 
 * **Follow up:**  Could you solve it without reversing the input lists?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-two-numbers-ii/
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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [7,2,4,3]
    // Test case 2: [5,6,4]
    // Test case 3: [2,4,3]
    // Test case 4: [5,6,4]
    // Test case 5: [0]
    // Test case 6: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
