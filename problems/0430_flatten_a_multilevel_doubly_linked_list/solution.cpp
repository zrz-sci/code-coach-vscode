// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 430: Flatten a Multilevel Doubly Linked List
 * Difficulty: Medium
 * Tags: Depth-First Search, Linked List, Doubly-Linked List
 *
 * Problem Description:
 * You are given a doubly linked list, which contains nodes that have a next
 * pointer, a previous pointer, and an additional **child pointer**. This child
 * pointer may or may not point to a separate doubly linked list, also
 * containing
 * these special nodes. These child lists may have one or more children of
 * their
 * own, and so on, to produce a **multilevel data structure** as shown in the
 * example below.
 * 
 * Given the `head` of the first level of the list, **flatten** the list so
 * that
 * all the nodes appear in a single-level, doubly linked list. Let `curr` be a
 * node
 * with a child list. The nodes in the child list should appear **after**
 * `curr`
 * and **before** `curr.next` in the flattened list.
 * 
 * Return _the_`head` _of the flattened list. The nodes in the list must
 * have**all** of their child pointers set to _`null`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,3,4,5,6,null,null,null,7,8,9,10,null,null,11,12]
 *     **Output:** [1,2,3,7,8,11,12,9,10,4,5,6]
 *     **Explanation:** The multilevel linked list in the input is shown.
 *     After flattening the multilevel linked list it becomes:
 *     
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** head = [1,2,null,3]
 *     **Output:** [1,3,2]
 *     **Explanation:** The multilevel linked list in the input is shown.
 *     After flattening the multilevel linked list it becomes:
 *     
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** head = []
 *     **Output:** []
 *     **Explanation:** There could be empty list in the input.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of Nodes will not exceed `1000`.
 *   * `1 <= Node.val <= 105`
 * 
 * 
 * 
 * **How the multilevel linked list is represented in test cases:**
 * 
 * We use the multilevel linked list from **Example 1** above:
 * 
 *     
 *     
 *      1---2---3---4---5---6--NULL
 *              |
 *              7---8---9---10--NULL
 *                  |
 *                  11--12--NULL
 * 
 * The serialization of each level is as follows:
 * 
 *     
 *     
 *     [1,2,3,4,5,6,null]
 *     [7,8,9,10,null]
 *     [11,12,null]
 *     
 * 
 * To serialize all levels together, we will add nulls in each level to
 * signify no
 * node connects to the upper node of the previous level. The serialization
 * becomes:
 * 
 *     
 *     
 *     [1,    2,    3, 4, 5, 6, null]
 *                  |
 *     [null, null, 7,    8, 9, 10, null]
 *                        |
 *     [            null, 11, 12, null]
 *     
 * 
 * Merging the serialization of each level and removing trailing nulls we
 * obtain:
 * 
 *     
 *     
 *     [1,2,3,4,5,6,null,null,null,7,8,9,10,null,null,11,12]
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flatten-a-multilevel-doubly-linked-list/
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

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
};
*/

class Solution {
public:
    Node* flatten(Node* head) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,6,null,null,null,7,8,9,10,null,null,11,12]
    // Test case 2: [1,2,null,3]
    // Test case 3: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
