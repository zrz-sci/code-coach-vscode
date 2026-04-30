// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 926: Flip String to Monotone Increasing
 * Difficulty: Medium
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * A binary string is monotone increasing if it consists of some number of
 * `0`'s
 * (possibly none), followed by some number of `1`'s (also possibly none).
 * 
 * You are given a binary string `s`. You can flip `s[i]` changing it from `0`
 * to
 * `1` or from `1` to `0`.
 * 
 * Return _the minimum number of flips to make_`s` _monotone increasing_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "00110"
 *     **Output:** 1
 *     **Explanation:** We flip the last digit to get 00111.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "010110"
 *     **Output:** 2
 *     **Explanation:** We flip to get 011111, or alternatively 000111.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "00011000"
 *     **Output:** 2
 *     **Explanation:** We flip to get 00000000.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s[i]` is either `'0'` or `'1'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flip-string-to-monotone-increasing/
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
    int minFlipsMonoIncr(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "00110"
    // Test case 2: "010110"
    // Test case 3: "00011000"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
