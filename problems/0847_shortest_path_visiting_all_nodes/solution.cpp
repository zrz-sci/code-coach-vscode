// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 847: Shortest Path Visiting All Nodes
 * Difficulty: Hard
 * Tags: Bit Manipulation, Breadth-First Search, Graph, Dynamic Programming, Bitmask
 *
 * Problem Description:
 * You have an undirected, connected graph of `n` nodes labeled from `0` to `n
 * -
 * 1`. You are given an array `graph` where `graph[i]` is a list of all the
 * nodes
 * connected with node `i` by an edge.
 * 
 * Return _the length of the shortest path that visits every node_. You may
 * start
 * and stop at any node, you may revisit nodes multiple times, and you may
 * reuse
 * edges.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,2,3],[0],[0],[0]]
 *     **Output:** 4
 *     **Explanation:** One possible path is [1,0,2,0,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** graph = [[1],[0,2,4],[1,3,4],[2],[1,2]]
 *     **Output:** 4
 *     **Explanation:** One possible path is [0,1,4,2,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == graph.length`
 *   * `1 <= n <= 12`
 *   * `0 <= graph[i].length < n`
 *   * `graph[i]` does not contain `i`.
 *   * If `graph[a]` contains `b`, then `graph[b]` contains `a`.
 *   * The input graph is always connected.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-path-visiting-all-nodes/
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
    int shortestPathLength(vector<vector<int>>& graph) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[0],[0],[0]]
    // Test case 2: [[1],[0,2,4],[1,3,4],[2],[1,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
