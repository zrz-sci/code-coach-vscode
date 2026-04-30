// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 482: License Key Formatting
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * You are given a license key represented as a string `s` that consists of
 * only
 * alphanumeric characters and dashes. The string is separated into `n + 1`
 * groups
 * by `n` dashes. You are also given an integer `k`.
 * 
 * We want to reformat the string `s` such that each group contains exactly `k`
 * characters, except for the first group, which could be shorter than `k` but
 * still must contain at least one character. Furthermore, there must be a dash
 * inserted between two groups, and you should convert all lowercase letters to
 * uppercase.
 * 
 * Return _the reformatted license key_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "5F3Z-2e-9-w", k = 4
 *     **Output:** "5F3Z-2E9W"
 * **Explanation:** The string s has been split into two parts, each part has
 * 4 characters.
 *     Note that the two extra dashes are not needed and can be removed.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "2-5g-3-J", k = 2
 *     **Output:** "2-5G-3J"
 * **Explanation:** The string s has been split into three parts, each part
 * has 2 characters except the first part as it could be shorter as mentioned
 * above.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of English letters, digits, and dashes `'-'`.
 *   * `1 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/license-key-formatting/
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
    string licenseKeyFormatting(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "5F3Z-2e-9-w"
    // Test case 2: 4
    // Test case 3: "2-5g-3-J"
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
