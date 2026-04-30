// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 21: Merge Two Sorted Lists
 * Difficulty: Easy
 * Tags: Recursion, Linked List
 *
 * Problem Description:
 * You are given the heads of two sorted linked lists `list1` and `list2`.
 * 
 * Merge the two lists into one **sorted** list. The list should be made by
 * splicing together the nodes of the first two lists.
 * 
 * Return _the head of the merged linked list_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** list1 = [1,2,4], list2 = [1,3,4]
 *     **Output:** [1,1,2,3,4,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** list1 = [], list2 = []
 *     **Output:** []
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** list1 = [], list2 = [0]
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in both lists is in the range `[0, 50]`.
 *   * `-100 <= Node.val <= 100`
 *   * Both `list1` and `list2` are sorted in **non-decreasing** order.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/merge-two-sorted-lists/
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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,4]
    // Test case 2: [1,3,4]
    // Test case 3: []
    // Test case 4: []
    // Test case 5: []
    // Test case 6: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
