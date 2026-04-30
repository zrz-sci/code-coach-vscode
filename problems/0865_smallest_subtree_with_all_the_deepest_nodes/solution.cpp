// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 865: Smallest Subtree with all the Deepest Nodes
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Hash Table, Binary Tree
 *
 * Problem Description:
 * Given the `root` of a binary tree, the depth of each node is **the shortest
 * distance to the root**.
 * 
 * Return _the smallest subtree_ such that it contains **all the deepest
 * nodes** in
 * the original tree.
 * 
 * A node is called **the deepest** if it has the largest depth possible among
 * any
 * node in the entire tree.
 * 
 * The **subtree** of a node is a tree consisting of that node, plus the set
 * of all
 * descendants of that node.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,5,1,6,2,0,8,null,null,7,4]
 *     **Output:** [2,7,4]
 * **Explanation:** We return the node with value 2, colored in yellow in the
 * diagram.
 *     The nodes coloured in blue are the deepest nodes of the tree.
 * Notice that nodes 5, 3 and 2 contain the deepest nodes in the tree but node
 * 2 is the smallest subtree among them, so we return it.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1]
 *     **Output:** [1]
 *     **Explanation:** The root is the deepest node in the tree.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [0,1,3,null,2]
 *     **Output:** [2]
 * **Explanation:** The deepest node in the tree is 2, the valid subtrees are
 * the subtrees of nodes 2, 1 and 0 but the subtree of node 2 is the smallest.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree will be in the range `[1, 500]`.
 *   * `0 <= Node.val <= 500`
 *   * The values of the nodes in the tree are **unique**.
 * 
 * 
 * 
 * **Note:** This question is the same as 1123:
 * <https://leetcode.com/problems/lowest-common-ancestor-of-deepest-leaves/>
 * 
 * 
 *
 * Link: https://leetcode.com/problems/smallest-subtree-with-all-the-deepest-nodes/
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
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,5,1,6,2,0,8,null,null,7,4]
    // Test case 2: [1]
    // Test case 3: [0,1,3,null,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
