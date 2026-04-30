// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 662: Maximum Width of Binary Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, return _the**maximum width** of the given
 * tree_.
 * 
 * The **maximum width** of a tree is the maximum **width** among all levels.
 * 
 * The **width** of one level is defined as the length between the end-nodes
 * (the
 * leftmost and rightmost non-null nodes), where the null nodes between the
 * end-
 * nodes that would be present in a complete binary tree extending down to that
 * level are also counted into the length calculation.
 * 
 * It is **guaranteed** that the answer will in the range of a **32-bit**
 * signed
 * integer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,3,2,5,3,null,9]
 *     **Output:** 4
 * **Explanation:** The maximum width exists in the third level with length 4
 * (5,3,null,9).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,3,2,5,null,null,9,6,null,7]
 *     **Output:** 7
 * **Explanation:** The maximum width exists in the fourth level with length 7
 * (6,null,null,null,null,null,7).
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,3,2,5]
 *     **Output:** 2
 * **Explanation:** The maximum width exists in the second level with length 2
 * (3,2).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 3000]`.
 *   * `-100 <= Node.val <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-width-of-binary-tree/
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
    int widthOfBinaryTree(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2,5,3,null,9]
    // Test case 2: [1,3,2,5,null,null,9,6,null,7]
    // Test case 3: [1,3,2,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
