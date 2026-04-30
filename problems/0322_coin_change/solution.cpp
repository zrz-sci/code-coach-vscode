// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 322: Coin Change
 * Difficulty: Medium
 * Tags: Breadth-First Search, Array, Dynamic Programming
 *
 * Problem Description:
 * You are given an integer array `coins` representing coins of different
 * denominations and an integer `amount` representing a total amount of money.
 * 
 * Return _the fewest number of coins that you need to make up that amount_. If
 * that amount of money cannot be made up by any combination of the coins,
 * return
 * `-1`.
 * 
 * You may assume that you have an infinite number of each kind of coin.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** coins = [1,2,5], amount = 11
 *     **Output:** 3
 *     **Explanation:** 11 = 5 + 5 + 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** coins = [2], amount = 3
 *     **Output:** -1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** coins = [1], amount = 0
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= coins.length <= 12`
 *   * `1 <= coins[i] <= 231 - 1`
 *   * `0 <= amount <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/coin-change/
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
    int coinChange(vector<int>& coins, int amount) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,5]
    // Test case 2: 11
    // Test case 3: [2]
    // Test case 4: 3
    // Test case 5: [1]
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
