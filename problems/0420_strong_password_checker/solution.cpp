// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 420: Strong Password Checker
 * Difficulty: Hard
 * Tags: Greedy, String, Heap (Priority Queue)
 *
 * Problem Description:
 * A password is considered strong if the below conditions are all met:
 * 
 *   * It has at least `6` characters and at most `20` characters.
 * * It contains at least **one lowercase** letter, at least **one uppercase**
 * letter, and at least **one digit**.
 * * It does not contain three repeating characters in a row (i.e., `"B
 * _**aaa**_ bb0"` is weak, but `"B** _aa_** b _**a**_ 0"` is strong).
 * 
 * Given a string `password`, return _the minimum number of steps required to
 * make`password` strong. if `password` is already strong, return `0`._
 * 
 * In one step, you can:
 * 
 *   * Insert one character to `password`,
 *   * Delete one character from `password`, or
 *   * Replace one character of `password` with another character.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** password = "a"
 *     **Output:** 5
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** password = "aA1"
 *     **Output:** 3
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** password = "1337C0d3"
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= password.length <= 50`
 * * `password` consists of letters, digits, dot `'.'` or exclamation mark
 * `'!'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/strong-password-checker/
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
    int strongPasswordChecker(string password) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "a"
    // Test case 2: "aA1"
    // Test case 3: "1337C0d3"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
