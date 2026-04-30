// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 63: Unique Paths II
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Matrix
 *
 * Problem Description:
 * You are given an `m x n` integer array `grid`. There is a robot initially
 * located at the **top-left corner** (i.e., `grid[0][0]`). The robot tries to
 * move
 * to the **bottom-right corner** (i.e., `grid[m - 1][n - 1]`). The robot can
 * only
 * move either down or right at any point in time.
 * 
 * An obstacle and space are marked as `1` or `0` respectively in `grid`. A
 * path
 * that the robot takes cannot include **any** square that is an obstacle.
 * 
 * Return _the number of possible unique paths that the robot can take to
 * reach the
 * bottom-right corner_.
 * 
 * The testcases are generated so that the answer will be less than or equal
 * to `2
 * * 109`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** obstacleGrid = [[0,0,0],[0,1,0],[0,0,0]]
 *     **Output:** 2
 * **Explanation:** There is one obstacle in the middle of the 3x3 grid above.
 *     There are two ways to reach the bottom-right corner:
 *     1. Right -> Right -> Down -> Down
 *     2. Down -> Down -> Right -> Right
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** obstacleGrid = [[0,1],[0,0]]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == obstacleGrid.length`
 *   * `n == obstacleGrid[i].length`
 *   * `1 <= m, n <= 100`
 *   * `obstacleGrid[i][j]` is `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/unique-paths-ii/
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
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,0,0],[0,1,0],[0,0,0]]
    // Test case 2: [[0,1],[0,0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
