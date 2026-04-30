// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 311: Sparse Matrix Multiplication
 * Difficulty: Medium
 * Tags: Array, Hash Table, Matrix
 *
 * Problem Description:
 * Given two [sparse matrices](https://en.wikipedia.org/wiki/Sparse_matrix)
 * `mat1`
 * of size `m x k` and `mat2` of size `k x n`, return the result of `mat1 x
 * mat2`.
 * You may assume that multiplication is always possible.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** mat1 = [[1,0,0],[-1,0,3]], mat2 = [[7,0,0],[0,0,0],[0,0,1]]
 *     **Output:** [[7,0,0],[-7,0,3]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** mat1 = [[0]], mat2 = [[0]]
 *     **Output:** [[0]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == mat1.length`
 *   * `k == mat1[i].length == mat2.length`
 *   * `n == mat2[i].length`
 *   * `1 <= m, n, k <= 100`
 *   * `-100 <= mat1[i][j], mat2[i][j] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sparse-matrix-multiplication/
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
    vector<vector<int>> multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,0,0],[-1,0,3]]
    // Test case 2: [[7,0,0],[0,0,0],[0,0,1]]
    // Test case 3: [[0]]
    // Test case 4: [[0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
