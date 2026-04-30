// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 678: Valid Parenthesis String
 * Difficulty: Medium
 * Tags: Stack, Greedy, String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s` containing only three types of characters: `'('`, `')'`
 * and
 * `'*'`, return `true` _if_ `s` _is**valid**_.
 * 
 * The following rules define a **valid** string:
 * 
 * * Any left parenthesis `'('` must have a corresponding right parenthesis
 * `')'`.
 * * Any right parenthesis `')'` must have a corresponding left parenthesis
 * `'('`.
 * * Left parenthesis `'('` must go before the corresponding right parenthesis
 * `')'`.
 * * `'*'` could be treated as a single right parenthesis `')'` or a single
 * left parenthesis `'('` or an empty string `""`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "()"
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "(*)"
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "(*))"
 *     **Output:** true
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 100`
 *   * `s[i]` is `'('`, `')'` or `'*'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-parenthesis-string/
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
    bool checkValidString(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "()"
    // Test case 2: "(*)"
    // Test case 3: "(*))"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
