// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1190: Reverse Substrings Between Each Pair of Parentheses
 * Difficulty: Medium
 * Tags: Stack, String
 *
 * Problem Description:
 * You are given a string `s` that consists of lower case English letters and
 * brackets.
 * 
 * Reverse the strings in each pair of matching parentheses, starting from the
 * innermost one.
 * 
 * Your result should **not** contain any brackets.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "(abcd)"
 *     **Output:** "dcba"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "(u(love)i)"
 *     **Output:** "iloveu"
 * **Explanation:** The substring "love" is reversed first, then the whole
 * string is reversed.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "(ed(et(oc))el)"
 *     **Output:** "leetcode"
 * **Explanation:** First, we reverse the substring "oc", then "etco", and
 * finally, the whole string.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 2000`
 *   * `s` only contains lower case English characters and parentheses.
 *   * It is guaranteed that all parentheses are balanced.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-substrings-between-each-pair-of-parentheses/
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
    string reverseParentheses(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "(abcd)"
    // Test case 2: "(u(love)i)"
    // Test case 3: "(ed(et(oc))el)"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
