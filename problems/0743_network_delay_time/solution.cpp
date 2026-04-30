// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 743: Network Delay Time
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Graph, Shortest Path, Heap (Priority Queue)
 *
 * Problem Description:
 * You are given a network of `n` nodes, labeled from `1` to `n`. You are also
 * given `times`, a list of travel times as directed edges `times[i] = (ui, vi,
 * wi)`, where `ui` is the source node, `vi` is the target node, and `wi` is
 * the
 * time it takes for a signal to travel from source to target.
 * 
 * We will send a signal from a given node `k`. Return _the**minimum** time it
 * takes for all the_ `n` _nodes to receive the signal_. If it is impossible
 * for
 * all the `n` nodes to receive the signal, return `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** times = [[1,2,1]], n = 2, k = 1
 *     **Output:** 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** times = [[1,2,1]], n = 2, k = 2
 *     **Output:** -1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= n <= 100`
 *   * `1 <= times.length <= 6000`
 *   * `times[i].length == 3`
 *   * `1 <= ui, vi <= n`
 *   * `ui != vi`
 *   * `0 <= wi <= 100`
 *   * All the pairs `(ui, vi)` are **unique**. (i.e., no multiple edges.)
 * 
 * 
 *
 * Link: https://leetcode.com/problems/network-delay-time/
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
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[2,1,1],[2,3,1],[3,4,1]]
    // Test case 2: 4
    // Test case 3: 2
    // Test case 4: [[1,2,1]]
    // Test case 5: 2
    // Test case 6: 1
    // Test case 7: [[1,2,1]]
    // Test case 8: 2
    // Test case 9: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
