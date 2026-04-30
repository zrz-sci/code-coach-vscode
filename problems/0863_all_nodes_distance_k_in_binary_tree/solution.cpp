// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 863: All Nodes Distance K in Binary Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Hash Table, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, the value of a target node `target`, and
 * an
 * integer `k`, return _an array of the values of all nodes that have a
 * distance_`k` _from the target node._
 * 
 * You can return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,5,1,6,2,0,8,null,null,7,4], target = 5, k = 2
 *     **Output:** [7,4,1]
 * Explanation: The nodes that are a distance 2 from the target node (with
 * value 5) have values 7, 4, and 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1], target = 1, k = 3
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 500]`.
 *   * `0 <= Node.val <= 500`
 *   * All the values `Node.val` are **unique**.
 *   * `target` is the value of one of the nodes in the tree.
 *   * `0 <= k <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/
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
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,5,1,6,2,0,8,null,null,7,4]
    // Test case 2: 5
    // Test case 3: 2
    // Test case 4: [1]
    // Test case 5: 1
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
