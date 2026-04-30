// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 141: Linked List Cycle
 * Difficulty: Easy
 * Tags: Hash Table, Linked List, Two Pointers
 *
 * Problem Description:
 * Given `head`, the head of a linked list, determine if the linked list has a
 * cycle in it.
 * 
 * There is a cycle in a linked list if there is some node in the list that
 * can be
 * reached again by continuously following the `next` pointer. Internally,
 * `pos` is
 * used to denote the index of the node that tail's `next` pointer is
 * connected to.
 * **Note that  `pos` is not passed as a parameter**.
 * 
 * Return `true` _if there is a cycle in the linked list_. Otherwise, return
 * `false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [3,2,0,-4], pos = 1
 *     **Output:** true
 * **Explanation:** There is a cycle in the linked list, where the tail
 * connects to the 1st node (0-indexed).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [1,2], pos = 0
 *     **Output:** true
 * **Explanation:** There is a cycle in the linked list, where the tail
 * connects to the 0th node.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** head = [1], pos = -1
 *     **Output:** false
 *     **Explanation:** There is no cycle in the linked list.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of the nodes in the list is in the range `[0, 104]`.
 *   * `-105 <= Node.val <= 105`
 *   * `pos` is `-1` or a **valid index** in the linked-list.
 * 
 * 
 * 
 * **Follow up:** Can you solve it using `O(1)` (i.e. constant) memory?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/linked-list-cycle/
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
    bool hasCycle(ListNode *head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,0,-4]
    // Test case 2: 1
    // Test case 3: [1,2]
    // Test case 4: 0
    // Test case 5: [1]
    // Test case 6: -1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
