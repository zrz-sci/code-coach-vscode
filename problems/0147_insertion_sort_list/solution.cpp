// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 147: Insertion Sort List
 * Difficulty: Medium
 * Tags: Linked List, Sorting
 *
 * Problem Description:
 * Given the `head` of a singly linked list, sort the list using **insertion
 * sort**
 * , and return _the sorted list 's head_.
 * 
 * The steps of the **insertion sort** algorithm:
 * 
 * 1. Insertion sort iterates, consuming one input element each repetition and
 * growing a sorted output list.
 * 2. At each iteration, insertion sort removes one element from the input
 * data, finds the location it belongs within the sorted list and inserts it
 * there.
 *   3. It repeats until no input elements remain.
 * 
 * The following is a graphical example of the insertion sort algorithm. The
 * partially sorted list (black) initially contains only the first element in
 * the
 * list. One element (red) is removed from the input data and inserted in-place
 * into the sorted list with each iteration.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [4,2,1,3]
 *     **Output:** [1,2,3,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [-1,5,3,4,0]
 *     **Output:** [-1,0,3,4,5]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the list is in the range `[1, 5000]`.
 *   * `-5000 <= Node.val <= 5000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/insertion-sort-list/
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
    ListNode* insertionSortList(ListNode* head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,1,3]
    // Test case 2: [-1,5,3,4,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
