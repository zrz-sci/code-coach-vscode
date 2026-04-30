// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 772: Basic Calculator III
 * Difficulty: Hard
 * Tags: Stack, Recursion, Math, String
 *
 * Problem Description:
 * Implement a basic calculator to evaluate a simple expression string.
 * 
 * The expression string contains only non-negative integers, `'+'`, `'-'`,
 * `'*'`,
 * `'/'` operators, and open `'('` and closing parentheses `')'`. The integer
 * division should **truncate toward zero**.
 * 
 * You may assume that the given expression is always valid. All intermediate
 * results will be in the range of `[-231, 231 - 1]`.
 * 
 * **Note:** You are not allowed to use any built-in function which evaluates
 * strings as mathematical expressions, such as `eval()`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "1+1"
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "6-4/2"
 *     **Output:** 4
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "2*(5+5*2)/3+(6/2+8)"
 *     **Output:** 21
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s <= 104`
 *   * `s` consists of digits, `'+'`, `'-'`, `'*'`, `'/'`, `'('`, and `')'`.
 *   * `s` is a **valid** expression.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/basic-calculator-iii/
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
    int calculate(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "1+1"
    // Test case 2: "6-4/2"
    // Test case 3: "2*(5+5*2)/3+(6/2+8)"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
