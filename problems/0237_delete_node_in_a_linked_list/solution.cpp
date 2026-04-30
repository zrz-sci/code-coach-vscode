// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 237: Delete Node in a Linked List
 * Difficulty: Medium
 * Tags: Linked List
 *
 * Problem Description:
 * There is a singly-linked list `head` and we want to delete a node `node` in
 * it.
 * 
 * You are given the node to be deleted `node`. You will **not be given
 * access** to
 * the first node of `head`.
 * 
 * All the values of the linked list are **unique** , and it is guaranteed
 * that the
 * given node `node` is not the last node in the linked list.
 * 
 * Delete the given node. Note that by deleting the node, we do not mean
 * removing
 * it from memory. We mean:
 * 
 *   * The value of the given node should not exist in the linked list.
 *   * The number of nodes in the linked list should decrease by one.
 *   * All the values before `node` should be in the same order.
 *   * All the values after `node` should be in the same order.
 * 
 * **Custom testing:**
 * 
 * * For the input, you should provide the entire linked list `head` and the
 * node to be given `node`. `node` should not be the last node of the list and
 * should be an actual node in the list.
 *   * We will build the linked list and pass the node to your function.
 *   * The output will be the entire list after calling your function.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [4,5,1,9], node = 5
 *     **Output:** [4,1,9]
 * **Explanation:** You are given the second node with value 5, the linked
 * list should become 4 -> 1 -> 9 after calling your function.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [4,5,1,9], node = 1
 *     **Output:** [4,5,9]
 * **Explanation:** You are given the third node with value 1, the linked list
 * should become 4 -> 5 -> 9 after calling your function.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of the nodes in the given list is in the range `[2, 1000]`.
 *   * `-1000 <= Node.val <= 1000`
 *   * The value of each node in the list is **unique**.
 *   * The `node` to be deleted is **in the list** and is **not a tail** node.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/delete-node-in-a-linked-list/
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
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    void deleteNode(ListNode* node) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,5,1,9]
    // Test case 2: 5
    // Test case 3: [4,5,1,9]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
