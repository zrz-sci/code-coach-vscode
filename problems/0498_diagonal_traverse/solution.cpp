// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 498: Diagonal Traverse
 * Difficulty: Medium
 * Tags: Array, Matrix, Simulation
 *
 * Problem Description:
 * Given an `m x n` matrix `mat`, return _an array of all the elements of the
 * array
 * in a diagonal order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** mat = [[1,2,3],[4,5,6],[7,8,9]]
 *     **Output:** [1,2,4,7,5,3,6,8,9]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** mat = [[1,2],[3,4]]
 *     **Output:** [1,2,3,4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == mat.length`
 *   * `n == mat[i].length`
 *   * `1 <= m, n <= 104`
 *   * `1 <= m * n <= 104`
 *   * `-105 <= mat[i][j] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/diagonal-traverse/
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
    vector<int> findDiagonalOrder(vector<vector<int>>& mat) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[4,5,6],[7,8,9]]
    // Test case 2: [[1,2],[3,4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
