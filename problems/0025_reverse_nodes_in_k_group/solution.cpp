// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 25: Reverse Nodes in k-Group
 * Difficulty: Hard
 * Tags: Recursion, Linked List
 *
 * Problem Description:
 * Given the `head` of a linked list, reverse the nodes of the list `k` at a
 * time,
 * and return _the modified list_.
 * 
 * `k` is a positive integer and is less than or equal to the length of the
 * linked
 * list. If the number of nodes is not a multiple of `k` then left-out nodes,
 * in
 * the end, should remain as it is.
 * 
 * You may not alter the values in the list's nodes, only nodes themselves may
 * be
 * changed.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4,5], k = 2
 *     **Output:** [2,1,4,3,5]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4,5], k = 3
 *     **Output:** [3,2,1,4,5]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the list is `n`.
 *   * `1 <= k <= n <= 5000`
 *   * `0 <= Node.val <= 1000`
 * 
 * 
 * 
 * **Follow-up:** Can you solve the problem in `O(1)` extra memory space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-nodes-in-k-group/
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
    ListNode* reverseKGroup(ListNode* head, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5]
    // Test case 2: 2
    // Test case 3: [1,2,3,4,5]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
