// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 921: Minimum Add to Make Parentheses Valid
 * Difficulty: Medium
 * Tags: Stack, Greedy, String
 *
 * Problem Description:
 * A parentheses string is valid if and only if:
 * 
 *   * It is the empty string,
 * * It can be written as `AB` (`A` concatenated with `B`), where `A` and `B`
 * are valid strings, or
 *   * It can be written as `(A)`, where `A` is a valid string.
 * 
 * You are given a parentheses string `s`. In one move, you can insert a
 * parenthesis at any position of the string.
 * 
 * * For example, if `s = "()))"`, you can insert an opening parenthesis to be
 * `"(**(**)))"` or a closing parenthesis to be `"())**)**)"`.
 * 
 * Return _the minimum number of moves required to make_`s` _valid_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "())"
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "((("
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `s[i]` is either `'('` or `')'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-add-to-make-parentheses-valid/
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
    int minAddToMakeValid(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "())"
    // Test case 2: "((("

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
