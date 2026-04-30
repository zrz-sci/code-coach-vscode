// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 510: Inorder Successor in BST II
 * Difficulty: Medium
 * Tags: Tree, Binary Search Tree, Binary Tree
 *
 * Problem Description:
 * Given a `node` in a binary search tree, return _the in-order successor of
 * that
 * node in the BST_. If that node has no in-order successor, return `null`.
 * 
 * The successor of a `node` is the node with the smallest key greater than
 * `node.val`.
 * 
 * You will have direct access to the node but not to the root of the tree.
 * Each
 * node will have a reference to its parent node. Below is the definition for
 * `Node`:
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
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** tree = [2,1,3], node = 1
 *     **Output:** 2
 * **Explanation:** 1's in-order successor node is 2. Note that both the node
 * and the return value is of Node type.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** tree = [5,3,6,2,4,null,null,1], node = 6
 *     **Output:** null
 * **Explanation:** There is no in-order successor of the current node, so the
 * answer is null.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 104]`.
 *   * `-105 <= Node.val <= 105`
 *   * All Nodes will have unique values.
 * 
 * 
 * 
 * **Follow up:** Could you solve it without looking up any of the node's
 * values?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/inorder-successor-in-bst-ii/
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
    Node* inorderSuccessor(Node* node) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1,3]
    // Test case 2: 1
    // Test case 3: [5,3,6,2,4,null,null,1]
    // Test case 4: 6

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
