// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 817: Linked List Components
 * Difficulty: Medium
 * Tags: Array, Hash Table, Linked List
 *
 * Problem Description:
 * You are given the `head` of a linked list containing unique integer values
 * and
 * an integer array `nums` that is a subset of the linked list values.
 * 
 * Return _the number of connected components in_`nums` _where two values are
 * connected if they appear**consecutively** in the linked list_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [0,1,2,3], nums = [0,1,3]
 *     **Output:** 2
 * **Explanation:** 0 and 1 are connected, so [0, 1] and [3] are the two
 * connected components.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [0,1,2,3,4], nums = [0,3,1,4]
 *     **Output:** 2
 * **Explanation:** 0 and 1 are connected, 3 and 4 are connected, so [0, 1]
 * and [3, 4] are the two connected components.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the linked list is `n`.
 *   * `1 <= n <= 104`
 *   * `0 <= Node.val < n`
 *   * All the values `Node.val` are **unique**.
 *   * `1 <= nums.length <= n`
 *   * `0 <= nums[i] < n`
 *   * All the values of `nums` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/linked-list-components/
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
    int numComponents(ListNode* head, vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,2,3]
    // Test case 2: [0,1,3]
    // Test case 3: [0,1,2,3,4]
    // Test case 4: [0,3,1,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
