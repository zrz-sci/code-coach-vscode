// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 449: Serialize and Deserialize BST
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Design, Binary Search Tree, String, Binary Tree
 *
 * Problem Description:
 * Serialization is converting a data structure or object into a sequence of
 * bits
 * so that it can be stored in a file or memory buffer, or transmitted across a
 * network connection link to be reconstructed later in the same or another
 * computer environment.
 * 
 * Design an algorithm to serialize and deserialize a **binary search tree**.
 * There
 * is no restriction on how your serialization/deserialization algorithm should
 * work. You need to ensure that a binary search tree can be serialized to a
 * string, and this string can be deserialized to the original tree structure.
 * 
 * **The encoded string should be as compact as possible.**
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [2,1,3]
 *     **Output:** [2,1,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = []
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[0, 104]`.
 *   * `0 <= Node.val <= 104`
 *   * The input tree is **guaranteed** to be a binary search tree.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/serialize-and-deserialize-bst/
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
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        
    }
};

// Your Codec object will be instantiated and called as such:
// Codec* ser = new Codec();
// Codec* deser = new Codec();
// string tree = ser->serialize(root);
// TreeNode* ans = deser->deserialize(tree);
// return ans;

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Codec
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // [2,1,3]
    // []

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Codec obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
