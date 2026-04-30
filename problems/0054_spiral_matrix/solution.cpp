// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 54: Spiral Matrix
 * Difficulty: Medium
 * Tags: Array, Matrix, Simulation
 *
 * Problem Description:
 * Given an `m x n` `matrix`, return _all elements of the_ `matrix` _in spiral
 * order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** matrix = [[1,2,3],[4,5,6],[7,8,9]]
 *     **Output:** [1,2,3,6,9,8,7,4,5]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
 *     **Output:** [1,2,3,4,8,12,11,10,9,5,6,7]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == matrix.length`
 *   * `n == matrix[i].length`
 *   * `1 <= m, n <= 10`
 *   * `-100 <= matrix[i][j] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/spiral-matrix/
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
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[4,5,6],[7,8,9]]
    // Test case 2: [[1,2,3,4],[5,6,7,8],[9,10,11,12]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
