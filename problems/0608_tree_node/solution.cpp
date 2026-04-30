// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 608: Tree Node
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Tree`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | id          | int  |
 *     | p_id        | int  |
 *     +-------------+------+
 *     id is the column with unique values for this table.
 * Each row of this table contains information about the id of a node and the
 * id of its parent node in a tree.
 *     The given structure is always a valid tree.
 *     
 * 
 * 
 * 
 * Each node in the tree can be one of three types:
 * 
 *   * **" Leaf"**: if the node is a leaf node.
 *   * **" Root"**: if the node is the root of the tree.
 *   * **" Inner"**: If the node is neither a leaf node nor a root node.
 * 
 * Write a solution to report the type of each node in the tree.
 * 
 * Return the result table in **any order**.
 * 
 * The result format is in the following example.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** 
 *     Tree table:
 *     +----+------+
 *     | id | p_id |
 *     +----+------+
 *     | 1  | null |
 *     | 2  | 1    |
 *     | 3  | 1    |
 *     | 4  | 2    |
 *     | 5  | 2    |
 *     +----+------+
 *     **Output:** 
 *     +----+-------+
 *     | id | type  |
 *     +----+-------+
 *     | 1  | Root  |
 *     | 2  | Inner |
 *     | 3  | Leaf  |
 *     | 4  | Leaf  |
 *     | 5  | Leaf  |
 *     +----+-------+
 *     **Explanation:** 
 * Node 1 is the root node because its parent node is null and it has child
 * nodes 2 and 3.
 * Node 2 is an inner node because it has parent node 1 and child node 4 and
 * 5.
 * Nodes 3, 4, and 5 are leaf nodes because they have parent nodes and they do
 * not have child nodes.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** 
 *     Tree table:
 *     +----+------+
 *     | id | p_id |
 *     +----+------+
 *     | 1  | null |
 *     +----+------+
 *     **Output:** 
 *     +----+-------+
 *     | id | type  |
 *     +----+-------+
 *     | 1  | Root  |
 *     +----+-------+
 * **Explanation:** If there is only one node on the tree, you only need to
 * output its root attributes.
 *     
 * 
 * 
 * 
 * **Note:** This question is the same as [ 3054: Binary Tree
 * Nodes.](https://leetcode.com/problems/binary-tree-nodes/description/)
 * 
 * 
 *
 * Link: https://leetcode.com/problems/tree-node/
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



// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Solution
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // {"headers":{"Tree":["id","p_id"]},"rows":{"Tree":[[1,null],[2,1],[3,1],[4,2],[5,2]]}}
    // {"headers":{"Tree":["id","p_id"]},"rows":{"Tree":[[1,null]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
