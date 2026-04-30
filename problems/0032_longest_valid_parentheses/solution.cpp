// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 32: Longest Valid Parentheses
 * Difficulty: Hard
 * Tags: Stack, String, Dynamic Programming
 *
 * Problem Description:
 * Given a string containing just the characters `'('` and `')'`, return _the
 * length of the longest valid (well-formed) parentheses_ _substring_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "(()"
 *     **Output:** 2
 *     **Explanation:** The longest valid parentheses substring is "()".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = ")()())"
 *     **Output:** 4
 *     **Explanation:** The longest valid parentheses substring is "()()".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = ""
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 3 * 104`
 *   * `s[i]` is `'('`, or `')'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-valid-parentheses/
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
    int longestValidParentheses(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "(()"
    // Test case 2: ")()())"
    // Test case 3: ""

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
