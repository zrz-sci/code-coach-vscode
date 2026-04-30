// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 987: Vertical Order Traversal of a Binary Tree
 * Difficulty: Hard
 * Tags: Tree, Depth-First Search, Breadth-First Search, Hash Table, Binary Tree, Sorting
 *
 * Problem Description:
 * Given the `root` of a binary tree, calculate the **vertical order
 * traversal** of
 * the binary tree.
 * 
 * For each node at position `(row, col)`, its left and right children will be
 * at
 * positions `(row + 1, col - 1)` and `(row + 1, col + 1)` respectively. The
 * root
 * of the tree is at `(0, 0)`.
 * 
 * The **vertical order traversal** of a binary tree is a list of top-to-bottom
 * orderings for each column index starting from the leftmost column and
 * ending on
 * the rightmost column. There may be multiple nodes in the same row and same
 * column. In such a case, sort these nodes by their values.
 * 
 * Return _the**vertical order traversal** of the binary tree_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** root = [3,9,20,null,null,15,7]
 *     **Output:** [[9],[3,15],[20],[7]]
 *     **Explanation:**
 *     Column -1: Only node 9 is in this column.
 * Column 0: Nodes 3 and 15 are in this column in that order from top to
 * bottom.
 *     Column 1: Only node 20 is in this column.
 *     Column 2: Only node 7 is in this column.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,5,6,7]
 *     **Output:** [[4],[2],[1,5,6],[3],[7]]
 *     **Explanation:**
 *     Column -2: Only node 4 is in this column.
 *     Column -1: Only node 2 is in this column.
 *     Column 0: Nodes 1, 5, and 6 are in this column.
 *               1 is at the top, so it comes first.
 * 5 and 6 are at the same position (2, 0), so we order them by their value, 5
 * before 6.
 *     Column 1: Only node 3 is in this column.
 *     Column 2: Only node 7 is in this column.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** root = [1,2,3,4,6,5,7]
 *     **Output:** [[4],[2],[1,5,6],[3],[7]]
 *     **Explanation:**
 * This case is the exact same as example 2, but with nodes 5 and 6 swapped.
 * Note that the solution remains the same since 5 and 6 are in the same
 * location and should be ordered by their values.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * The number of nodes in the tree is in the range `[1, 1000]`.
 *   * `0 <= Node.val <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree/
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
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,9,20,null,null,15,7]
    // Test case 2: [1,2,3,4,5,6,7]
    // Test case 3: [1,2,3,4,6,5,7]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
