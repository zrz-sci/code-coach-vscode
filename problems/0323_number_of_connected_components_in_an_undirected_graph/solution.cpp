// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 323: Number of Connected Components in an Undirected Graph
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Graph
 *
 * Problem Description:
 * You have a graph of `n` nodes. You are given an integer `n` and an array
 * `edges`
 * where `edges[i] = [ai, bi]` indicates that there is an edge between `ai` and
 * `bi` in the graph.
 * 
 * Return _the number of connected components in the graph_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 5, edges = [[0,1],[1,2],[3,4]]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 5, edges = [[0,1],[1,2],[2,3],[3,4]]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 2000`
 *   * `1 <= edges.length <= 5000`
 *   * `edges[i].length == 2`
 *   * `0 <= ai <= bi < n`
 *   * `ai != bi`
 *   * There are no repeated edges.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-connected-components-in-an-undirected-graph/
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
    int countComponents(int n, vector<vector<int>>& edges) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: [[0,1],[1,2],[3,4]]
    // Test case 3: 5
    // Test case 4: [[0,1],[1,2],[2,3],[3,4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
