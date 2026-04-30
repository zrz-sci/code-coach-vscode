// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 797: All Paths From Source to Target
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Graph, Backtracking
 *
 * Problem Description:
 * Given a directed acyclic graph (**DAG**) of `n` nodes labeled from `0` to
 * `n -
 * 1`, find all possible paths from node `0` to node `n - 1` and return them in
 * **any order**.
 * 
 * The graph is given as follows: `graph[i]` is a list of all nodes you can
 * visit
 * from node `i` (i.e., there is a directed edge from node `i` to node
 * `graph[i][j]`).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** graph = [[1,2],[3],[3],[]]
 *     **Output:** [[0,1,3],[0,2,3]]
 *     **Explanation:** There are two paths: 0 -> 1 -> 3 and 0 -> 2 -> 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** graph = [[4,3,1],[3,2,4],[3],[4],[]]
 *     **Output:** [[0,4],[0,3,4],[0,1,3,4],[0,1,2,3,4],[0,1,4]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == graph.length`
 *   * `2 <= n <= 15`
 *   * `0 <= graph[i][j] < n`
 *   * `graph[i][j] != i` (i.e., there will be no self-loops).
 *   * All the elements of `graph[i]` are **unique**.
 *   * The input graph is **guaranteed** to be a **DAG**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/all-paths-from-source-to-target/
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
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[3],[3],[]]
    // Test case 2: [[4,3,1],[3,2,4],[3],[4],[]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
