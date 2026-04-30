// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 827: Making A Large Island
 * Difficulty: Hard
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Array, Matrix
 *
 * Problem Description:
 * You are given an `n x n` binary matrix `grid`. You are allowed to change
 * **at
 * most one** `0` to be `1`.
 * 
 * Return _the size of the largest**island** in_ `grid` _after applying this
 * operation_.
 * 
 * An **island** is a 4-directionally connected group of `1`s.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,0],[0,1]]
 *     **Output:** 3
 * **Explanation:** Change one 0 to 1 and connect two 1s, then we get an
 * island with area = 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,1],[1,0]]
 *     **Output:** 4
 * **Explanation:** Change the 0 to 1 and make the island bigger, only one
 * island with area = 4.
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,1],[1,1]]
 *     **Output:** 4
 *     **Explanation:** Can't change any 0 to 1, only one island with area = 4.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= n <= 500`
 *   * `grid[i][j]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/making-a-large-island/
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
    int largestIsland(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,0],[0,1]]
    // Test case 2: [[1,1],[1,0]]
    // Test case 3: [[1,1],[1,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
