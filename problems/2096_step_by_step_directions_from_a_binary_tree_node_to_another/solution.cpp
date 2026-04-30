// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 2096: Step-By-Step Directions From a Binary Tree Node to Another
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, String, Binary Tree
 *
 * Problem Description:
 * You are given the `root` of a **binary tree** with `n` nodes. Each node is
 * uniquely assigned a value from `1` to `n`. You are also given an integer
 * `startValue` representing the value of the start node `s`, and a different
 * integer `destValue` representing the value of the destination node `t`.
 * 
 * Find the **shortest path** starting from node `s` and ending at node `t`.
 * Generate step-by-step directions of such path as a string consisting of
 * only the
 * **uppercase** letters `'L'`, `'R'`, and `'U'`. Each letter indicates a
 * specific
 * direction:
 * 
 *   * `'L'` means to go from a node to its **left child** node.
 *   * `'R'` means to go from a node to its **right child** node.
 *   * `'U'` means to go from a node to its **parent** node.
 * 
 * Return _the step-by-step directions of the**shortest path** from node _`s`
 * _to
 * node_ `t`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [5,1,2,3,null,6,4], startValue = 3, destValue = 6
 *     **Output:** "UURL"
 *     **Explanation:** The shortest path is: 3 -> 1 -> 5 -> 2 -> 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [2,1], startValue = 2, destValue = 1
 *     **Output:** "L"
 *     **Explanation:** The shortest path is: 2 -> 1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is `n`.
 *   * `2 <= n <= 105`
 *   * `1 <= Node.val <= n`
 *   * All the values in the tree are **unique**.
 *   * `1 <= startValue, destValue <= n`
 *   * `startValue != destValue`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/step-by-step-directions-from-a-binary-tree-node-to-another/
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
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    string getDirections(TreeNode* root, int startValue, int destValue) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,1,2,3,null,6,4]
    // Test case 2: 3
    // Test case 3: 6
    // Test case 4: [2,1]
    // Test case 5: 2
    // Test case 6: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
