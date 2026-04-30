// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 304: Range Sum Query 2D - Immutable
 * Difficulty: Medium
 * Tags: Design, Array, Matrix, Prefix Sum
 *
 * Problem Description:
 * Given a 2D matrix `matrix`, handle multiple queries of the following type:
 * 
 * * Calculate the **sum** of the elements of `matrix` inside the rectangle
 * defined by its **upper left corner** `(row1, col1)` and **lower right
 * corner** `(row2, col2)`.
 * 
 * Implement the `NumMatrix` class:
 * 
 * * `NumMatrix(int[][] matrix)` Initializes the object with the integer
 * matrix `matrix`.
 * * `int sumRegion(int row1, int col1, int row2, int col2)` Returns the
 * **sum** of the elements of `matrix` inside the rectangle defined by its
 * **upper left corner** `(row1, col1)` and **lower right corner** `(row2,
 * col2)`.
 * 
 * You must design an algorithm where `sumRegion` works on `O(1)` time
 * complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["NumMatrix", "sumRegion", "sumRegion", "sumRegion"]
 * [[[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7], [1,
 * 0, 3, 0, 5]]], [2, 1, 4, 3], [1, 1, 2, 2], [1, 2, 2, 4]]
 *     **Output**
 *     [null, 8, 11, 12]
 *     
 *     **Explanation**
 * NumMatrix numMatrix = new NumMatrix([[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1,
 * 2, 0, 1, 5], [4, 1, 0, 1, 7], [1, 0, 3, 0, 5]]);
 * numMatrix.sumRegion(2, 1, 4, 3); // return 8 (i.e sum of the red rectangle)
 * numMatrix.sumRegion(1, 1, 2, 2); // return 11 (i.e sum of the green
 * rectangle)
 * numMatrix.sumRegion(1, 2, 2, 4); // return 12 (i.e sum of the blue
 * rectangle)
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == matrix.length`
 *   * `n == matrix[i].length`
 *   * `1 <= m, n <= 200`
 *   * `-104 <= matrix[i][j] <= 104`
 *   * `0 <= row1 <= row2 < m`
 *   * `0 <= col1 <= col2 < n`
 *   * At most `104` calls will be made to `sumRegion`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/range-sum-query-2d-immutable/
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

class NumMatrix {
public:
    NumMatrix(vector<vector<int>>& matrix) {
        
    }
    
    int sumRegion(int row1, int col1, int row2, int col2) {
        
    }
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix* obj = new NumMatrix(matrix);
 * int param_1 = obj->sumRegion(row1,col1,row2,col2);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: NumMatrix
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["NumMatrix","sumRegion","sumRegion","sumRegion"]
    // [[[[3,0,1,4,2],[5,6,3,2,1],[1,2,0,1,5],[4,1,0,1,7],[1,0,3,0,5]]],[2,1,4,3],[1,1,2,2],[1,2,2,4]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // NumMatrix obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
