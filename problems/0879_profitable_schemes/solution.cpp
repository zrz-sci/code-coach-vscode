// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 879: Profitable Schemes
 * Difficulty: Hard
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * There is a group of `n` members, and a list of various crimes they could
 * commit.
 * The `ith` crime generates a `profit[i]` and requires `group[i]` members to
 * participate in it. If a member participates in one crime, that member can't
 * participate in another crime.
 * 
 * Let's call a **profitable scheme** any subset of these crimes that
 * generates at
 * least `minProfit` profit, and the total number of members participating in
 * that
 * subset of crimes is at most `n`.
 * 
 * Return the number of schemes that can be chosen. Since the answer may be
 * very
 * large, **return it modulo** `109 + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 5, minProfit = 3, group = [2,2], profit = [2,3]
 *     **Output:** 2
 * **Explanation:** To make a profit of at least 3, the group could either
 * commit crimes 0 and 1, or just crime 1.
 *     In total, there are 2 schemes.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 10, minProfit = 5, group = [2,3,5], profit = [6,7,8]
 *     **Output:** 7
 * **Explanation:** To make a profit of at least 5, the group could commit any
 * crimes, as long as they commit one.
 * There are 7 possible schemes: (0), (1), (2), (0,1), (0,2), (1,2), and
 * (0,1,2).
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 100`
 *   * `0 <= minProfit <= 100`
 *   * `1 <= group.length <= 100`
 *   * `1 <= group[i] <= 100`
 *   * `profit.length == group.length`
 *   * `0 <= profit[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/profitable-schemes/
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
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: 3
    // Test case 3: [2,2]
    // Test case 4: [2,3]
    // Test case 5: 10
    // Test case 6: 5
    // Test case 7: [2,3,5]
    // Test case 8: [6,7,8]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
