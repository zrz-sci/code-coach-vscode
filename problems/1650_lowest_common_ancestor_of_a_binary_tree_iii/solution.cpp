// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1650: Lowest Common Ancestor of a Binary Tree III
 * Difficulty: Medium
 * Tags: Tree, Hash Table, Two Pointers, Binary Tree
 *
 * Problem Description:
 * Given two nodes of a binary tree `p` and `q`, return _their  lowest common
 * ancestor (LCA)_.
 * 
 * Each node will have a reference to its parent node. The definition for
 * `Node` is
 * below:
 * 
 *     
 *     
 *     class Node {
 *         public int val;
 *         public Node left;
 *         public Node right;
 *         public Node parent;
 *     }
 *     
 * 
 * According to the **[definition of LCA on
 * Wikipedia](https://en.wikipedia.org/wiki/Lowest_common_ancestor)** : "The
 * lowest
 * common ancestor of two nodes p and q in a tree T is the lowest node that has
 * both p and q as descendants (where we allow **a node to be a descendant of
 * itself**)."
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
 *     **Output:** 3
 *     **Explanation:** The LCA of nodes 5 and 1 is 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
 *     **Output:** 5
 * **Explanation:** The LCA of nodes 5 and 4 is 5 since a node can be a
 * descendant of itself according to the LCA definition.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2], p = 1, q = 2
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[2, 105]`.
 *   * `-109 <= Node.val <= 109`
 *   * All `Node.val` are **unique**.
 *   * `p != q`
 *   * `p` and `q` exist in the tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree-iii/
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
    Node* left;
    Node* right;
    Node* parent;
};
*/

class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node * q) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,5,1,6,2,0,8,null,null,7,4]
    // Test case 2: 5
    // Test case 3: 1
    // Test case 4: [3,5,1,6,2,0,8,null,null,7,4]
    // Test case 5: 5
    // Test case 6: 4
    // Test case 7: [1,2]
    // Test case 8: 1
    // Test case 9: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
