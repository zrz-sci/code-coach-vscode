// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 241: Different Ways to Add Parentheses
 * Difficulty: Medium
 * Tags: Recursion, Memoization, Math, String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `expression` of numbers and operators, return _all possible
 * results from computing all the different possible ways to group numbers and
 * operators_. You may return the answer in **any order**.
 * 
 * The test cases are generated such that the output values fit in a 32-bit
 * integer
 * and the number of different results does not exceed `104`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** expression = "2-1-1"
 *     **Output:** [0,2]
 *     **Explanation:**
 *     ((2-1)-1) = 0 
 *     (2-(1-1)) = 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** expression = "2*3-4*5"
 *     **Output:** [-34,-14,-10,-10,10]
 *     **Explanation:**
 *     (2*(3-(4*5))) = -34 
 *     ((2*3)-(4*5)) = -14 
 *     ((2*(3-4))*5) = -10 
 *     (2*((3-4)*5)) = -10 
 *     (((2*3)-4)*5) = 10
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= expression.length <= 20`
 * * `expression` consists of digits and the operator `'+'`, `'-'`, and `'*'`.
 * * All the integer values in the input expression are in the range `[0,
 * 99]`.
 * * The integer values in the input expression do not have a leading `'-'` or
 * `'+'` denoting the sign.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/different-ways-to-add-parentheses/
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
    vector<int> diffWaysToCompute(string expression) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "2-1-1"
    // Test case 2: "2*3-4*5"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
