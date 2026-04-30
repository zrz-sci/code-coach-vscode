// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 301: Remove Invalid Parentheses
 * Difficulty: Hard
 * Tags: Breadth-First Search, String, Backtracking
 *
 * Problem Description:
 * Given a string `s` that contains parentheses and letters, remove the minimum
 * number of invalid parentheses to make the input string valid.
 * 
 * Return _a list of**unique strings** that are valid with the minimum number
 * of
 * removals_. You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "()())()"
 *     **Output:** ["(())()","()()()"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "(a)())()"
 *     **Output:** ["(a())()","(a)()()"]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = ")("
 *     **Output:** [""]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 25`
 * * `s` consists of lowercase English letters and parentheses `'('` and
 * `')'`.
 *   * There will be at most `20` parentheses in `s`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/remove-invalid-parentheses/
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
    vector<string> removeInvalidParentheses(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "()())()"
    // Test case 2: "(a)())()"
    // Test case 3: ")("

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
