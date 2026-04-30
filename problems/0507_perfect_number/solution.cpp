// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 507: Perfect Number
 * Difficulty: Easy
 * Tags: Math
 *
 * Problem Description:
 * A [**perfect number**](https://en.wikipedia.org/wiki/Perfect_number) is a
 * **positive integer** that is equal to the sum of its **positive divisors** ,
 * excluding the number itself. A **divisor** of an integer `x` is an integer
 * that
 * can divide `x` evenly.
 * 
 * Given an integer `n`, return `true` _if_`n` _is a perfect number, otherwise
 * return_`false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num = 28
 *     **Output:** true
 *     **Explanation:** 28 = 1 + 2 + 4 + 7 + 14
 *     1, 2, 4, 7, and 14 are all divisors of 28.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num = 7
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= num <= 108`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/perfect-number/
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
    bool checkPerfectNumber(int num) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 28
    // Test case 2: 7

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
