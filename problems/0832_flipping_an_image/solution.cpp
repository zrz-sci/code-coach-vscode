// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 832: Flipping an Image
 * Difficulty: Easy
 * Tags: Bit Manipulation, Array, Two Pointers, Matrix, Simulation
 *
 * Problem Description:
 * Given an `n x n` binary matrix `image`, flip the image **horizontally** ,
 * then
 * invert it, and return _the resulting image_.
 * 
 * To flip an image horizontally means that each row of the image is reversed.
 * 
 *   * For example, flipping `[1,1,0]` horizontally results in `[0,1,1]`.
 * 
 * To invert an image means that each `0` is replaced by `1`, and each `1` is
 * replaced by `0`.
 * 
 *   * For example, inverting `[0,1,1]` results in `[1,0,0]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** image = [[1,1,0],[1,0,1],[0,0,0]]
 *     **Output:** [[1,0,0],[0,1,0],[1,1,1]]
 *     **Explanation:** First reverse each row: [[0,1,1],[1,0,1],[0,0,0]].
 *     Then, invert the image: [[1,0,0],[0,1,0],[1,1,1]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** image = [[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]
 *     **Output:** [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
 * **Explanation:** First reverse each row:
 * [[0,0,1,1],[1,0,0,1],[1,1,1,0],[0,1,0,1]].
 *     Then invert the image: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == image.length`
 *   * `n == image[i].length`
 *   * `1 <= n <= 20`
 *   * `images[i][j]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flipping-an-image/
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
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& image) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1,0],[1,0,1],[0,0,0]]
    // Test case 2: [[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
