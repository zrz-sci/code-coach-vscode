// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 面试题 02.05: Sum Lists LCCI
 * Difficulty: Medium
 * Tags: Recursion, Linked List, Math
 *
 * Problem Description:
 * You have two numbers represented by a linked list, where each node contains
 * a
 * single digit. The digits are stored in reverse order, such that the 1's
 * digit is
 * at the head of the list. Write a function that adds the two numbers and
 * returns
 * the sum as a linked list.
 * 
 * 
 * 
 * **Example:**
 * 
 *     
 *     
 *     **Input:**(7 -> 1 -> 6) + (5 -> 9 -> 2). That is, 617 + 295.
 *     **Output:** 2 -> 1 -> 9. That is, 912.
 *     
 * 
 * **Follow Up: **Suppose the digits are stored in forward order. Repeat the
 * above
 * problem.
 * 
 * **Example:**
 * 
 *     
 *     
 *     **Input:**(6 -> 1 -> 7) + (2 -> 9 -> 5). That is, 617 + 295.
 *     **Output:** 9 -> 1 -> 2. That is, 912.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sum-lists-lcci/
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
    // Test case 1: [2,4,3]
    // Test case 2: [5,6,4]
    // Test case 3: [0]
    // Test case 4: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
