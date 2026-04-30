// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 566: Reshape the Matrix
 * Difficulty: Easy
 * Tags: Array, Matrix, Simulation
 *
 * Problem Description:
 * In MATLAB, there is a handy function called `reshape` which can reshape an
 * `m x
 * n` matrix into a new one with a different size `r x c` keeping its original
 * data.
 * 
 * You are given an `m x n` matrix `mat` and two integers `r` and `c`
 * representing
 * the number of rows and the number of columns of the wanted reshaped matrix.
 * 
 * The reshaped matrix should be filled with all the elements of the original
 * matrix in the same row-traversing order as they were.
 * 
 * If the `reshape` operation with given parameters is possible and legal,
 * output
 * the new reshaped matrix; Otherwise, output the original matrix.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** mat = [[1,2],[3,4]], r = 1, c = 4
 *     **Output:** [[1,2,3,4]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** mat = [[1,2],[3,4]], r = 2, c = 4
 *     **Output:** [[1,2],[3,4]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == mat.length`
 *   * `n == mat[i].length`
 *   * `1 <= m, n <= 100`
 *   * `-1000 <= mat[i][j] <= 1000`
 *   * `1 <= r, c <= 300`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reshape-the-matrix/
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
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[3,4]]
    // Test case 2: 1
    // Test case 3: 4
    // Test case 4: [[1,2],[3,4]]
    // Test case 5: 2
    // Test case 6: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
