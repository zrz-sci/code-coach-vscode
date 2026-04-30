// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 405: Convert a Number to Hexadecimal
 * Difficulty: Easy
 * Tags: Bit Manipulation, Math, String
 *
 * Problem Description:
 * Given a 32-bit integer `num`, return _a string representing its hexadecimal
 * representation_. For negative integers, [two's
 * complement](https://en.wikipedia.org/wiki/Two%27s_complement) method is
 * used.
 * 
 * All the letters in the answer string should be lowercase characters, and
 * there
 * should not be any leading zeros in the answer except for the zero itself.
 * 
 * **Note: **You are not allowed to use any built-in library method to
 * directly
 * solve this problem.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num = 26
 *     **Output:** "1a"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num = -1
 *     **Output:** "ffffffff"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-231 <= num <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/convert-a-number-to-hexadecimal/
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
    string toHex(int num) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 26
    // Test case 2: -1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
