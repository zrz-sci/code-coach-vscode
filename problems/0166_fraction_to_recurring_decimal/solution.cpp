// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 166: Fraction to Recurring Decimal
 * Difficulty: Medium
 * Tags: Hash Table, Math, String
 *
 * Problem Description:
 * Given two integers representing the `numerator` and `denominator` of a
 * fraction,
 * return _the fraction in string format_.
 * 
 * If the fractional part is repeating, enclose the repeating part in
 * parentheses
 * 
 * If multiple answers are possible, return **any of them**.
 * 
 * It is **guaranteed** that the length of the answer string is less than
 * `104` for
 * all the given inputs.
 * 
 * **Note** that if the fraction can be represented as a _finite length
 * string_ ,
 * you **must** return it.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** numerator = 1, denominator = 2
 *     **Output:** "0.5"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** numerator = 2, denominator = 1
 *     **Output:** "2"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** numerator = 4, denominator = 333
 *     **Output:** "0.(012)"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-231 <= numerator, denominator <= 231 - 1`
 *   * `denominator != 0`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/fraction-to-recurring-decimal/
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
    string fractionToDecimal(int numerator, int denominator) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 2
    // Test case 3: 2
    // Test case 4: 1
    // Test case 5: 4
    // Test case 6: 333

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
