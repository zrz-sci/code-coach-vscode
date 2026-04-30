// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 143: Reorder List
 * Difficulty: Medium
 * Tags: Stack, Recursion, Linked List, Two Pointers
 *
 * Problem Description:
 * You are given the head of a singly linked-list. The list can be represented
 * as:
 * 
 *     
 *     
 *     L0 -> L1 -> … -> Ln - 1 -> Ln
 *     
 * 
 * _Reorder the list to be on the following form:_
 * 
 *     
 *     
 *     L0 -> Ln -> L1 -> Ln - 1 -> L2 -> Ln - 2 -> …
 *     
 * 
 * You may not modify the values in the list's nodes. Only nodes themselves
 * may be
 * changed.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4]
 *     **Output:** [1,4,2,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4,5]
 *     **Output:** [1,5,2,4,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the list is in the range `[1, 5 * 104]`.
 *   * `1 <= Node.val <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reorder-list/
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
    void reorderList(ListNode* head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4]
    // Test case 2: [1,2,3,4,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
