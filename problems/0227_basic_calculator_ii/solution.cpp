// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 227: Basic Calculator II
 * Difficulty: Medium
 * Tags: Stack, Math, String
 *
 * Problem Description:
 * Given a string `s` which represents an expression, _evaluate this
 * expression and
 * return its value_.
 * 
 * The integer division should truncate toward zero.
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
 *     **Input:** s = "3+2*2"
 *     **Output:** 7
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = " 3/2 "
 *     **Output:** 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = " 3+5 / 2 "
 *     **Output:** 5
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 3 * 105`
 * * `s` consists of integers and operators `('+', '-', '*', '/')` separated
 * by some number of spaces.
 *   * `s` represents **a valid expression**.
 * * All the integers in the expression are non-negative integers in the range
 * `[0, 231 - 1]`.
 *   * The answer is **guaranteed** to fit in a **32-bit integer**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/basic-calculator-ii/
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
    // Test case 1: "3+2*2"
    // Test case 2: " 3/2 "
    // Test case 3: " 3+5 / 2 "

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
