// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 319: Bulb Switcher
 * Difficulty: Medium
 * Tags: Brainteaser, Math
 *
 * Problem Description:
 * There are `n` bulbs that are initially off. You first turn on all the bulbs,
 * then you turn off every second bulb.
 * 
 * On the third round, you toggle every third bulb (turning on if it's off or
 * turning off if it's on). For the `ith` round, you toggle every `i` bulb.
 * For the
 * `nth` round, you only toggle the last bulb.
 * 
 * Return _the number of bulbs that are on after`n` rounds_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 1
 *     **Explanation:** At first, the three bulbs are [off, off, off].
 *     After the first round, the three bulbs are [on, on, on].
 *     After the second round, the three bulbs are [on, off, on].
 *     After the third round, the three bulbs are [on, off, off]. 
 *     So you should return 1 because there is only one bulb is on.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 0
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
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
 *   * `0 <= n <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/bulb-switcher/
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
    int bulbSwitch(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 0
    // Test case 3: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
