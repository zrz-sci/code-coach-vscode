// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1091: Shortest Path in Binary Matrix
 * Difficulty: Medium
 * Tags: Breadth-First Search, Array, Matrix
 *
 * Problem Description:
 * Given an `n x n` binary matrix `grid`, return _the length of the
 * shortest**clear
 * path** in the matrix_. If there is no clear path, return `-1`.
 * 
 * A **clear path** in a binary matrix is a path from the **top-left** cell
 * (i.e.,
 * `(0, 0)`) to the **bottom-right** cell (i.e., `(n - 1, n - 1)`) such that:
 * 
 *   * All the visited cells of the path are `0`.
 * * All the adjacent cells of the path are **8-directionally** connected
 * (i.e., they are different and they share an edge or a corner).
 * 
 * The **length of a clear path** is the number of visited cells of this path.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,1],[1,0]]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,0,0],[1,1,0],[1,1,0]]
 *     **Output:** 4
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,0,0],[1,1,0],[1,1,0]]
 *     **Output:** -1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= n <= 100`
 *   * `grid[i][j] is 0 or 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-path-in-binary-matrix/
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
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,1],[1,0]]
    // Test case 2: [[0,0,0],[1,1,0],[1,1,0]]
    // Test case 3: [[1,0,0],[1,1,0],[1,1,0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
