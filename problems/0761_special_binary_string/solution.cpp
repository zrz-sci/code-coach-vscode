// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 761: Special Binary String
 * Difficulty: Hard
 * Tags: String, Divide and Conquer, Sorting
 *
 * Problem Description:
 * **Special binary strings** are binary strings with the following two
 * properties:
 * 
 *   * The number of `0`'s is equal to the number of `1`'s.
 *   * Every prefix of the binary string has at least as many `1`'s as `0`'s.
 * 
 * You are given a **special binary** string `s`.
 * 
 * A move consists of choosing two consecutive, non-empty, special substrings
 * of
 * `s`, and swapping them. Two strings are consecutive if the last character
 * of the
 * first string is exactly one index before the first character of the second
 * string.
 * 
 * Return _the lexicographically largest resulting string possible after
 * applying
 * the mentioned operations on the string_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "11011000"
 *     **Output:** "11100100"
 * **Explanation:** The strings "10" [occuring at s[1]] and "1100" [at s[3]]
 * are swapped.
 * This is the lexicographically largest string possible after some number of
 * swaps.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "10"
 *     **Output:** "10"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 50`
 *   * `s[i]` is either `'0'` or `'1'`.
 *   * `s` is a special binary string.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/special-binary-string/
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
    string makeLargestSpecial(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "11011000"
    // Test case 2: "10"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
