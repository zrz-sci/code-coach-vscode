// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 856: Score of Parentheses
 * Difficulty: Medium
 * Tags: Stack, String
 *
 * Problem Description:
 * Given a balanced parentheses string `s`, return _the**score** of the
 * string_.
 * 
 * The **score** of a balanced parentheses string is based on the following
 * rule:
 * 
 *   * `"()"` has score `1`.
 * * `AB` has score `A + B`, where `A` and `B` are balanced parentheses
 * strings.
 *   * `(A)` has score `2 * A`, where `A` is a balanced parentheses string.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "()"
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "(())"
 *     **Output:** 2
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "()()"
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= s.length <= 50`
 *   * `s` consists of only `'('` and `')'`.
 *   * `s` is a balanced parentheses string.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/score-of-parentheses/
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
    int scoreOfParentheses(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "()"
    // Test case 2: "(())"
    // Test case 3: "()()"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
