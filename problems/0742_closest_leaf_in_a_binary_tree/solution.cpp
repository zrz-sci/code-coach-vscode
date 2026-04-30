// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 742: Closest Leaf in a Binary Tree
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree where every node has **a unique value**
 * and a
 * target integer `k`, return _the value of the**nearest leaf node** to the
 * target
 * _`k` _in the tree_.
 * 
 * **Nearest to a leaf** means the least number of edges traveled on the binary
 * tree to reach any leaf of the tree. Also, a node is called a leaf if it has
 * no
 * children.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [1,3,2], k = 1
 *     **Output:** 2
 * **Explanation:** Either 2 or 3 is the nearest leaf node to the target of 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1], k = 1
 *     **Output:** 1
 *     **Explanation:** The nearest leaf node is the root node itself.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,null,null,null,5,null,6], k = 2
 *     **Output:** 3
 * **Explanation:** The leaf node with value 3 (and not the leaf node with
 * value 6) is nearest to the node with value 2.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 1000]`.
 *   * `1 <= Node.val <= 1000`
 *   * All the values of the tree are **unique**.
 *   * There exist some node in the tree where `Node.val == k`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/closest-leaf-in-a-binary-tree/
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
    int findClosestLeaf(TreeNode* root, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2]
    // Test case 2: 1
    // Test case 3: [1]
    // Test case 4: 1
    // Test case 5: [1,2,3,4,null,null,null,5,null,6]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
