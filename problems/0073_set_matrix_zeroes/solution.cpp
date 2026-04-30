// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 73: Set Matrix Zeroes
 * Difficulty: Medium
 * Tags: Array, Hash Table, Matrix
 *
 * Problem Description:
 * Given an `m x n` integer matrix `matrix`, if an element is `0`, set its
 * entire
 * row and column to `0`'s.
 * 
 * You must do it [in place](https://en.wikipedia.org/wiki/In-place_algorithm).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** matrix = [[1,1,1],[1,0,1],[1,1,1]]
 *     **Output:** [[1,0,1],[0,0,0],[1,0,1]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** matrix = [[0,1,2,0],[3,4,5,2],[1,3,1,5]]
 *     **Output:** [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == matrix.length`
 *   * `n == matrix[0].length`
 *   * `1 <= m, n <= 200`
 *   * `-231 <= matrix[i][j] <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:**
 * 
 *   * A straightforward solution using `O(mn)` space is probably a bad idea.
 * * A simple improvement uses `O(m + n)` space, but still not the best
 * solution.
 *   * Could you devise a constant space solution?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/set-matrix-zeroes/
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
    void setZeroes(vector<vector<int>>& matrix) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1,1],[1,0,1],[1,1,1]]
    // Test case 2: [[0,1,2,0],[3,4,5,2],[1,3,1,5]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
