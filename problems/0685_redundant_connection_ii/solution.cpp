// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 685: Redundant Connection II
 * Difficulty: Hard
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Graph
 *
 * Problem Description:
 * In this problem, a rooted tree is a **directed** graph such that, there is
 * exactly one node (the root) for which all other nodes are descendants of
 * this
 * node, plus every node has exactly one parent, except for the root node
 * which has
 * no parents.
 * 
 * The given input is a directed graph that started as a rooted tree with `n`
 * nodes
 * (with distinct values from `1` to `n`), with one additional directed edge
 * added.
 * The added edge has two different vertices chosen from `1` to `n`, and was
 * not an
 * edge that already existed.
 * 
 * The resulting graph is given as a 2D-array of `edges`. Each element of
 * `edges`
 * is a pair `[ui, vi]` that represents a **directed** edge connecting nodes
 * `ui`
 * and `vi`, where `ui` is a parent of child `vi`.
 * 
 * Return _an edge that can be removed so that the resulting graph is a rooted
 * tree
 * of_ `n` _nodes_. If there are multiple answers, return the answer that
 * occurs
 * last in the given 2D-array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** edges = [[1,2],[1,3],[2,3]]
 *     **Output:** [2,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** edges = [[1,2],[2,3],[3,4],[4,1],[1,5]]
 *     **Output:** [4,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == edges.length`
 *   * `3 <= n <= 1000`
 *   * `edges[i].length == 2`
 *   * `1 <= ui, vi <= n`
 *   * `ui != vi`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/redundant-connection-ii/
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

class Solution {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[1,3],[2,3]]
    // Test case 2: [[1,2],[2,3],[3,4],[4,1],[1,5]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
