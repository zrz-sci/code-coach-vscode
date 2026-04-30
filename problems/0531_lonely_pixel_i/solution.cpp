// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 531: Lonely Pixel I
 * Difficulty: Medium
 * Tags: Array, Hash Table, Matrix
 *
 * Problem Description:
 * Given an `m x n` `picture` consisting of black `'B'` and white `'W'` pixels,
 * return _the number of**black** lonely pixels_.
 * 
 * A black lonely pixel is a character `'B'` that located at a specific
 * position
 * where the same row and same column don't have **any other** black pixels.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** picture = [["W","W","B"],["W","B","W"],["B","W","W"]]
 *     **Output:** 3
 *     **Explanation:** All the three 'B's are black lonely pixels.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** picture = [["B","B","B"],["B","B","W"],["B","B","B"]]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == picture.length`
 *   * `n == picture[i].length`
 *   * `1 <= m, n <= 500`
 *   * `picture[i][j]` is `'W'` or `'B'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/lonely-pixel-i/
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
    int findLonelyPixel(vector<vector<char>>& picture) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["W","W","B"],["W","B","W"],["B","W","W"]]
    // Test case 2: [["B","B","B"],["B","B","W"],["B","B","B"]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
