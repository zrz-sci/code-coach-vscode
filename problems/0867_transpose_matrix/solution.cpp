// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 867: Transpose Matrix
 * Difficulty: Easy
 * Tags: Array, Matrix, Simulation
 *
 * Problem Description:
 * Given a 2D integer array `matrix`, return _the**transpose** of_ `matrix`.
 * 
 * The **transpose** of a matrix is the matrix flipped over its main diagonal,
 * switching the matrix's row and column indices.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** matrix = [[1,2,3],[4,5,6],[7,8,9]]
 *     **Output:** [[1,4,7],[2,5,8],[3,6,9]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** matrix = [[1,2,3],[4,5,6]]
 *     **Output:** [[1,4],[2,5],[3,6]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == matrix.length`
 *   * `n == matrix[i].length`
 *   * `1 <= m, n <= 1000`
 *   * `1 <= m * n <= 105`
 *   * `-109 <= matrix[i][j] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/transpose-matrix/
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
    vector<vector<int>> transpose(vector<vector<int>>& matrix) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[4,5,6],[7,8,9]]
    // Test case 2: [[1,2,3],[4,5,6]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
