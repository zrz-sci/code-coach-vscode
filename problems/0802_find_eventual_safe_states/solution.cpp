// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 802: Find Eventual Safe States
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Graph, Topological Sort
 *
 * Problem Description:
 * There is a directed graph of `n` nodes with each node labeled from `0` to
 * `n -
 * 1`. The graph is represented by a **0-indexed** 2D integer array `graph`
 * where
 * `graph[i]` is an integer array of nodes adjacent to node `i`, meaning there
 * is
 * an edge from node `i` to each node in `graph[i]`.
 * 
 * A node is a **terminal node** if there are no outgoing edges. A node is a
 * **safe
 * node** if every possible path starting from that node leads to a **terminal
 * node** (or another safe node).
 * 
 * Return _an array containing all the**safe nodes** of the graph_. The answer
 * should be sorted in **ascending** order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,2],[2,3],[5],[0],[5],[],[]]
 *     **Output:** [2,4,5,6]
 *     **Explanation:** The given graph is shown above.
 * Nodes 5 and 6 are terminal nodes as there are no outgoing edges from either
 * of them.
 * Every path starting at nodes 2, 4, 5, and 6 all lead to either node 5 or 6.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,2,3,4],[1,2],[3,4],[0,4],[]]
 *     **Output:** [4]
 *     **Explanation:**
 * Only node 4 is a terminal node, and every path starting at node 4 leads to
 * node 4.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == graph.length`
 *   * `1 <= n <= 104`
 *   * `0 <= graph[i].length <= n`
 *   * `0 <= graph[i][j] <= n - 1`
 *   * `graph[i]` is sorted in a strictly increasing order.
 *   * The graph may contain self-loops.
 *   * The number of edges in the graph will be in the range `[1, 4 * 104]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-eventual-safe-states/
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
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[2,3],[5],[0],[5],[],[]]
    // Test case 2: [[1,2,3,4],[1,2],[3,4],[0,4],[]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
