// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 361: Bomb Enemy
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Matrix
 *
 * Problem Description:
 * Given an `m x n` matrix `grid` where each cell is either a wall `'W'`, an
 * enemy
 * `'E'` or empty `'0'`, return _the maximum enemies you can kill using one
 * bomb_.
 * You can only place the bomb in an empty cell.
 * 
 * The bomb kills all the enemies in the same row and column from the planted
 * point
 * until it hits the wall since it is too strong to be destroyed.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** grid = [["0","E","0","0"],["E","0","W","E"],["0","E","0","0"]]
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [["W","W","W"],["0","0","0"],["E","E","E"]]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 500`
 *   * `grid[i][j]` is either `'W'`, `'E'`, or `'0'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/bomb-enemy/
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
    int maxKilledEnemies(vector<vector<char>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["0","E","0","0"],["E","0","W","E"],["0","E","0","0"]]
    // Test case 2: [["W","W","W"],["0","0","0"],["E","E","E"]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
