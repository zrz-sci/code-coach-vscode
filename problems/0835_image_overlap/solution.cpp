// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 835: Image Overlap
 * Difficulty: Medium
 * Tags: Array, Matrix
 *
 * Problem Description:
 * You are given two images, `img1` and `img2`, represented as binary, square
 * matrices of size `n x n`. A binary matrix has only `0`s and `1`s as values.
 * 
 * We **translate** one image however we choose by sliding all the `1` bits
 * left,
 * right, up, and/or down any number of units. We then place it on top of the
 * other
 * image. We can then calculate the **overlap** by counting the number of
 * positions
 * that have a `1` in **both** images.
 * 
 * Note also that a translation does **not** include any kind of rotation. Any
 * `1`
 * bits that are translated outside of the matrix borders are erased.
 * 
 * Return _the largest possible overlap_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** img1 = [[1,1,0],[0,1,0],[0,1,0]], img2 =
 * [[0,0,0],[0,1,1],[0,0,1]]
 *     **Output:** 3
 * **Explanation:** We translate img1 to right by 1 unit and down by 1 unit.
 *     
 * The number of positions that have a 1 in both images is 3 (shown in red).
 *     
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** img1 = [[1]], img2 = [[1]]
 *     **Output:** 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** img1 = [[0]], img2 = [[0]]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == img1.length == img1[i].length`
 *   * `n == img2.length == img2[i].length`
 *   * `1 <= n <= 30`
 *   * `img1[i][j]` is either `0` or `1`.
 *   * `img2[i][j]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/image-overlap/
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
    int largestOverlap(vector<vector<int>>& img1, vector<vector<int>>& img2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1,0],[0,1,0],[0,1,0]]
    // Test case 2: [[0,0,0],[0,1,1],[0,0,1]]
    // Test case 3: [[1]]
    // Test case 4: [[1]]
    // Test case 5: [[0]]
    // Test case 6: [[0]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
