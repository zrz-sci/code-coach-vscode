// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 919: Complete Binary Tree Inserter
 * Difficulty: Medium
 * Tags: Tree, Breadth-First Search, Design, Binary Tree
 *
 * Problem Description:
 * A **complete binary tree** is a binary tree in which every level, except
 * possibly the last, is completely filled, and all nodes are as far left as
 * possible.
 * 
 * Design an algorithm to insert a new node to a complete binary tree keeping
 * it
 * complete after the insertion.
 * 
 * Implement the `CBTInserter` class:
 * 
 * * `CBTInserter(TreeNode root)` Initializes the data structure with the
 * `root` of the complete binary tree.
 * * `int insert(int v)` Inserts a `TreeNode` into the tree with value
 * `Node.val == val` so that the tree remains complete, and returns the value
 * of the parent of the inserted `TreeNode`.
 *   * `TreeNode get_root()` Returns the root node of the tree.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["CBTInserter", "insert", "insert", "get_root"]
 *     [[[1, 2]], [3], [4], []]
 *     **Output**
 *     [null, 1, 2, [1, 2, 3, 4]]
 *     
 *     **Explanation**
 *     CBTInserter cBTInserter = new CBTInserter([1, 2]);
 *     cBTInserter.insert(3);  // return 1
 *     cBTInserter.insert(4);  // return 2
 *     cBTInserter.get_root(); // return [1, 2, 3, 4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree will be in the range `[1, 1000]`.
 *   * `0 <= Node.val <= 5000`
 *   * `root` is a complete binary tree.
 *   * `0 <= val <= 5000`
 *   * At most `104` calls will be made to `insert` and `get_root`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/complete-binary-tree-inserter/
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
class CBTInserter {
public:
    CBTInserter(TreeNode* root) {
        
    }
    
    int insert(int val) {
        
    }
    
    TreeNode* get_root() {
        
    }
};

/**
 * Your CBTInserter object will be instantiated and called as such:
 * CBTInserter* obj = new CBTInserter(root);
 * int param_1 = obj->insert(val);
 * TreeNode* param_2 = obj->get_root();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: CBTInserter
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["CBTInserter","insert","insert","get_root"]
    // [[[1,2]],[3],[4],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // CBTInserter obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
