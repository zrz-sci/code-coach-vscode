// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1905: Count Sub Islands
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Array, Matrix
 *
 * Problem Description:
 * You are given two `m x n` binary matrices `grid1` and `grid2` containing
 * only
 * `0`'s (representing water) and `1`'s (representing land). An **island** is a
 * group of `1`'s connected **4-directionally** (horizontal or vertical). Any
 * cells
 * outside of the grid are considered water cells.
 * 
 * An island in `grid2` is considered a **sub-island** if there is an island in
 * `grid1` that contains **all** the cells that make up **this** island in
 * `grid2`.
 * 
 * Return the _**number** of islands in _`grid2` _that are considered**sub-
 * islands**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** grid1 =
 * [[1,1,1,0,0],[0,1,1,1,1],[0,0,0,0,0],[1,0,0,0,0],[1,1,0,1,1]], grid2 =
 * [[1,1,1,0,0],[0,0,1,1,1],[0,1,0,0,0],[1,0,1,1,0],[0,1,0,1,0]]
 *     **Output:** 3
 * **Explanation:** In the picture above, the grid on the left is grid1 and
 * the grid on the right is grid2.
 * The 1s colored red in grid2 are those considered to be part of a
 * sub-island. There are three sub-islands.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** grid1 =
 * [[1,0,1,0,1],[1,1,1,1,1],[0,0,0,0,0],[1,1,1,1,1],[1,0,1,0,1]], grid2 =
 * [[0,0,0,0,0],[1,1,1,1,1],[0,1,0,1,0],[0,1,0,1,0],[1,0,0,0,1]]
 *     **Output:** 2 
 * **Explanation:** In the picture above, the grid on the left is grid1 and
 * the grid on the right is grid2.
 * The 1s colored red in grid2 are those considered to be part of a
 * sub-island. There are two sub-islands.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid1.length == grid2.length`
 *   * `n == grid1[i].length == grid2[i].length`
 *   * `1 <= m, n <= 500`
 *   * `grid1[i][j]` and `grid2[i][j]` are either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-sub-islands/
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
    int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1,1,0,0],[0,1,1,1,1],[0,0,0,0,0],[1,0,0,0,0],[1,1,0,1,1]]
    // Test case 2: [[1,1,1,0,0],[0,0,1,1,1],[0,1,0,0,0],[1,0,1,1,0],[0,1,0,1,0]]
    // Test case 3: [[1,0,1,0,1],[1,1,1,1,1],[0,0,0,0,0],[1,1,1,1,1],[1,0,1,0,1]]
    // Test case 4: [[0,0,0,0,0],[1,1,1,1,1],[0,1,0,1,0],[0,1,0,1,0],[1,0,0,0,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
