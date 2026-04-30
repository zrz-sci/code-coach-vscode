// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 24: Swap Nodes in Pairs
 * Difficulty: Medium
 * Tags: Recursion, Linked List
 *
 * Problem Description:
 * Given a linked list, swap every two adjacent nodes and return its head. You
 * must
 * solve the problem without modifying the values in the list's nodes (i.e.,
 * only
 * nodes themselves may be changed.)
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** head = [1,2,3,4]
 * 
 * **Output:** [2,1,4,3]
 * 
 * **Explanation:**
 * 
 * **Example 2:**
 * 
 * **Input:** head = []
 * 
 * **Output:** []
 * 
 * **Example 3:**
 * 
 * **Input:** head = [1]
 * 
 * **Output:** [1]
 * 
 * **Example 4:**
 * 
 * **Input:** head = [1,2,3]
 * 
 * **Output:** [2,1,3]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the list is in the range `[0, 100]`.
 *   * `0 <= Node.val <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/swap-nodes-in-pairs/
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
    ListNode* swapPairs(ListNode* head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4]
    // Test case 2: []
    // Test case 3: [1]
    // Test case 4: [1,2,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
