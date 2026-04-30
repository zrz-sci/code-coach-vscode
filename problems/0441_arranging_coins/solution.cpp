// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 441: Arranging Coins
 * Difficulty: Easy
 * Tags: Math, Binary Search
 *
 * Problem Description:
 * You have `n` coins and you want to build a staircase with these coins. The
 * staircase consists of `k` rows where the `ith` row has exactly `i` coins.
 * The
 * last row of the staircase **may be** incomplete.
 * 
 * Given the integer `n`, return _the number of**complete rows** of the
 * staircase
 * you will build_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 5
 *     **Output:** 2
 *     **Explanation:** Because the 3rd row is incomplete, we return 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 8
 *     **Output:** 3
 *     **Explanation:** Because the 4th row is incomplete, we return 3.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/arranging-coins/
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
    int arrangeCoins(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: 8

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
