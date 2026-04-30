// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 7: Reverse Integer
 * Difficulty: Medium
 * Tags: Math
 *
 * Problem Description:
 * Given a signed 32-bit integer `x`, return `x` _with its digits reversed_. If
 * reversing `x` causes the value to go outside the signed 32-bit integer range
 * `[-231, 231 - 1]`, then return `0`.
 * 
 * **Assume the environment does not allow you to store 64-bit integers
 * (signed or
 * unsigned).**
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** x = 123
 *     **Output:** 321
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** x = -123
 *     **Output:** -321
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** x = 120
 *     **Output:** 21
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-231 <= x <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-integer/
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
    int reverse(int x) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 123
    // Test case 2: -123
    // Test case 3: 120

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
