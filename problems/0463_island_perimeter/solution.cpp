// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 463: Island Perimeter
 * Difficulty: Easy
 * Tags: Depth-First Search, Breadth-First Search, Array, Matrix
 *
 * Problem Description:
 * You are given `row x col` `grid` representing a map where `grid[i][j] = 1`
 * represents land and `grid[i][j] = 0` represents water.
 * 
 * Grid cells are connected **horizontally/vertically** (not diagonally). The
 * `grid` is completely surrounded by water, and there is exactly one island
 * (i.e.,
 * one or more connected land cells).
 * 
 * The island doesn't have "lakes", meaning the water inside isn't connected
 * to the
 * water around the island. One cell is a square with side length 1. The grid
 * is
 * rectangular, width and height don't exceed 100. Determine the perimeter of
 * the
 * island.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,1,0,0],[1,1,1,0],[0,1,0,0],[1,1,0,0]]
 *     **Output:** 16
 * **Explanation:** The perimeter is the 16 yellow stripes in the image above.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[1]]
 *     **Output:** 4
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,0]]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `row == grid.length`
 *   * `col == grid[i].length`
 *   * `1 <= row, col <= 100`
 *   * `grid[i][j]` is `0` or `1`.
 *   * There is exactly one island in `grid`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/island-perimeter/
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
    int islandPerimeter(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,1,0,0],[1,1,1,0],[0,1,0,0],[1,1,0,0]]
    // Test case 2: [[1]]
    // Test case 3: [[1,0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
