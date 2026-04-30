// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 415: Add Strings
 * Difficulty: Easy
 * Tags: Math, String, Simulation
 *
 * Problem Description:
 * Given two non-negative integers, `num1` and `num2` represented as string,
 * return
 * _the sum of_ `num1` _and_ `num2` _as a string_.
 * 
 * You must solve the problem without using any built-in library for handling
 * large
 * integers (such as `BigInteger`). You must also not convert the inputs to
 * integers directly.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num1 = "11", num2 = "123"
 *     **Output:** "134"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num1 = "456", num2 = "77"
 *     **Output:** "533"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** num1 = "0", num2 = "0"
 *     **Output:** "0"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= num1.length, num2.length <= 104`
 *   * `num1` and `num2` consist of only digits.
 * * `num1` and `num2` don't have any leading zeros except for the zero
 * itself.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-strings/
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
    string addStrings(string num1, string num2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "11"
    // Test case 2: "123"
    // Test case 3: "456"
    // Test case 4: "77"
    // Test case 5: "0"
    // Test case 6: "0"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
