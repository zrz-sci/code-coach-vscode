// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 790: Domino and Tromino Tiling
 * Difficulty: Medium
 * Tags: Dynamic Programming
 *
 * Problem Description:
 * You have two types of tiles: a `2 x 1` domino shape and a tromino shape.
 * You may
 * rotate these shapes.
 * 
 * Given an integer n, return _the number of ways to tile an_ `2 x n` _board_.
 * Since the answer may be very large, return it **modulo** `109 + 7`.
 * 
 * In a tiling, every square must be covered by a tile. Two tilings are
 * different
 * if and only if there are two 4-directionally adjacent cells on the board
 * such
 * that exactly one of the tilings has both squares occupied by a tile.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 5
 *     **Explanation:** The five different ways are shown above.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/domino-and-tromino-tiling/
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
    int numTilings(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
