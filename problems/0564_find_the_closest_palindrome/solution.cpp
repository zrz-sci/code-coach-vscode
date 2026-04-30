// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 564: Find the Closest Palindrome
 * Difficulty: Hard
 * Tags: Math, String
 *
 * Problem Description:
 * Given a string `n` representing an integer, return _the closest integer (not
 * including itself), which is a palindrome_. If there is a tie, return _**the
 * smaller one**_.
 * 
 * The closest is defined as the absolute difference minimized between two
 * integers.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = "123"
 *     **Output:** "121"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = "1"
 *     **Output:** "0"
 * **Explanation:** 0 and 2 are the closest palindromes but we return the
 * smallest which is 0.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n.length <= 18`
 *   * `n` consists of only digits.
 *   * `n` does not have leading zeros.
 *   * `n` is representing an integer in the range `[1, 1018 - 1]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-closest-palindrome/
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
    string nearestPalindromic(string n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "123"
    // Test case 2: "1"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
