// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 725: Split Linked List in Parts
 * Difficulty: Medium
 * Tags: Linked List
 *
 * Problem Description:
 * Given the `head` of a singly linked list and an integer `k`, split the
 * linked
 * list into `k` consecutive linked list parts.
 * 
 * The length of each part should be as equal as possible: no two parts should
 * have
 * a size differing by more than one. This may lead to some parts being null.
 * 
 * The parts should be in the order of occurrence in the input list, and parts
 * occurring earlier should always have a size greater than or equal to parts
 * occurring later.
 * 
 * Return _an array of the_`k` _parts_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3], k = 5
 *     **Output:** [[1],[2],[3],[],[]]
 *     **Explanation:**
 * The first element output[0] has output[0].val = 1, output[0].next = null.
 * The last element output[4] is null, but its string representation as a
 * ListNode is [].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4,5,6,7,8,9,10], k = 3
 *     **Output:** [[1,2,3,4],[5,6,7],[8,9,10]]
 *     **Explanation:**
 * The input has been split into consecutive parts with size difference at
 * most 1, and earlier parts are a larger size than the later parts.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the list is in the range `[0, 1000]`.
 *   * `0 <= Node.val <= 1000`
 *   * `1 <= k <= 50`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/split-linked-list-in-parts/
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
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: 5
    // Test case 3: [1,2,3,4,5,6,7,8,9,10]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
