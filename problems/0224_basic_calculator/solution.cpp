// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 224: Basic Calculator
 * Difficulty: Hard
 * Tags: Stack, Recursion, Math, String
 *
 * Problem Description:
 * Given a string `s` representing a valid expression, implement a basic
 * calculator
 * to evaluate it, and return _the result of the evaluation_.
 * 
 * **Note:** You are **not** allowed to use any built-in function which
 * evaluates
 * strings as mathematical expressions, such as `eval()`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "1 + 1"
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = " 2-1 + 2 "
 *     **Output:** 3
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "(1+(4+5+2)-3)+(6+8)"
 *     **Output:** 23
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 3 * 105`
 *   * `s` consists of digits, `'+'`, `'-'`, `'('`, `')'`, and `' '`.
 *   * `s` represents a valid expression.
 * * `'+'` is **not** used as a unary operation (i.e., `"+1"` and `"+(2 + 3)"`
 * is invalid).
 * * `'-'` could be used as a unary operation (i.e., `"-1"` and `"-(2 + 3)"`
 * is valid).
 *   * There will be no two consecutive operators in the input.
 * * Every number and running calculation will fit in a signed 32-bit integer.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/basic-calculator/
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
    // Test case 1: "1 + 1"
    // Test case 2: " 2-1 + 2 "
    // Test case 3: "(1+(4+5+2)-3)+(6+8)"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
