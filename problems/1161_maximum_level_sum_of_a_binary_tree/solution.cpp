// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1161: Maximum Level Sum of a Binary Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, the level of its root is `1`, the level
 * of
 * its children is `2`, and so on.
 * 
 * Return the **smallest** level `x` such that the sum of all the values of
 * nodes
 * at level `x` is **maximal**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,7,0,7,-8,null,null]
 *     **Output:** 2
 *     **Explanation:**
 *     Level 1 sum = 1.
 *     Level 2 sum = 7 + 0 = 7.
 *     Level 3 sum = 7 + -8 = -1.
 *     So we return the level with the maximum sum which is level 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [989,null,10250,98693,-89388,null,null,null,-32127]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 104]`.
 *   * `-105 <= Node.val <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/
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
    int maxLevelSum(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,7,0,7,-8,null,null]
    // Test case 2: [989,null,10250,98693,-89388,null,null,null,-32127]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
