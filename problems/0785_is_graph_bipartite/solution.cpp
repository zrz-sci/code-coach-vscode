// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 785: Is Graph Bipartite?
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Graph
 *
 * Problem Description:
 * There is an **undirected** graph with `n` nodes, where each node is numbered
 * between `0` and `n - 1`. You are given a 2D array `graph`, where `graph[u]`
 * is
 * an array of nodes that node `u` is adjacent to. More formally, for each `v`
 * in
 * `graph[u]`, there is an undirected edge between node `u` and node `v`. The
 * graph
 * has the following properties:
 * 
 *   * There are no self-edges (`graph[u]` does not contain `u`).
 * * There are no parallel edges (`graph[u]` does not contain duplicate
 * values).
 * * If `v` is in `graph[u]`, then `u` is in `graph[v]` (the graph is
 * undirected).
 * * The graph may not be connected, meaning there may be two nodes `u` and
 * `v` such that there is no path between them.
 * 
 * A graph is **bipartite** if the nodes can be partitioned into two
 * independent
 * sets `A` and `B` such that **every** edge in the graph connects a node in
 * set
 * `A` and a node in set `B`.
 * 
 * Return `true` _if and only if it is**bipartite**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,2,3],[0,2],[0,1,3],[0,2]]
 *     **Output:** false
 * **Explanation:** There is no way to partition the nodes into two
 * independent sets such that every edge connects a node in one and a node in
 * the other.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,3],[0,2],[1,3],[0,2]]
 *     **Output:** true
 * **Explanation:** We can partition the nodes into two sets: {0, 2} and {1,
 * 3}.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `graph.length == n`
 *   * `1 <= n <= 100`
 *   * `0 <= graph[u].length < n`
 *   * `0 <= graph[u][i] <= n - 1`
 *   * `graph[u]` does not contain `u`.
 *   * All the values of `graph[u]` are **unique**.
 *   * If `graph[u]` contains `v`, then `graph[v]` contains `u`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/is-graph-bipartite/
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
    bool isBipartite(vector<vector<int>>& graph) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[0,2],[0,1,3],[0,2]]
    // Test case 2: [[1,3],[0,2],[1,3],[0,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
