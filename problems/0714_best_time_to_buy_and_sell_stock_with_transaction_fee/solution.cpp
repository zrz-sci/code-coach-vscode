// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 714: Best Time to Buy and Sell Stock with Transaction Fee
 * Difficulty: Medium
 * Tags: Greedy, Array, Dynamic Programming
 *
 * Problem Description:
 * You are given an array `prices` where `prices[i]` is the price of a given
 * stock
 * on the `ith` day, and an integer `fee` representing a transaction fee.
 * 
 * Find the maximum profit you can achieve. You may complete as many
 * transactions
 * as you like, but you need to pay the transaction fee for each transaction.
 * 
 * **Note:**
 * 
 * * You may not engage in multiple transactions simultaneously (i.e., you
 * must sell the stock before you buy again).
 * * The transaction fee is only charged once for each stock purchase and
 * sale.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** prices = [1,3,2,8,4,9], fee = 2
 *     **Output:** 8
 *     **Explanation:** The maximum profit can be achieved by:
 *     - Buying at prices[0] = 1
 *     - Selling at prices[3] = 8
 *     - Buying at prices[4] = 4
 *     - Selling at prices[5] = 9
 *     The total profit is ((8 - 1) - 2) + ((9 - 4) - 2) = 8.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** prices = [1,3,7,5,10,3], fee = 3
 *     **Output:** 6
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= prices.length <= 5 * 104`
 *   * `1 <= prices[i] < 5 * 104`
 *   * `0 <= fee < 5 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/
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
    int maxProfit(vector<int>& prices, int fee) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2,8,4,9]
    // Test case 2: 2
    // Test case 3: [1,3,7,5,10,3]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
