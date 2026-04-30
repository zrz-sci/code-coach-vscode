// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1351: Count Negative Numbers in a Sorted Matrix
 * Difficulty: Easy
 * Tags: Array, Binary Search, Matrix
 *
 * Problem Description:
 * Given a `m x n` matrix `grid` which is sorted in non-increasing order both
 * row-
 * wise and column-wise, return _the number of**negative** numbers in_ `grid`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[4,3,2,-1],[3,2,1,-1],[1,1,-1,-2],[-1,-1,-2,-3]]
 *     **Output:** 8
 *     **Explanation:** There are 8 negatives number in the matrix.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[3,2],[1,0]]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 100`
 *   * `-100 <= grid[i][j] <= 100`
 * 
 * 
 * 
 * **Follow up:** Could you find an `O(n + m)` solution?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-negative-numbers-in-a-sorted-matrix/
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
    int countNegatives(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[4,3,2,-1],[3,2,1,-1],[1,1,-1,-2],[-1,-1,-2,-3]]
    // Test case 2: [[3,2],[1,0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
