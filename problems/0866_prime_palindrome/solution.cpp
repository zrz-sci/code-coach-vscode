// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 866: Prime Palindrome
 * Difficulty: Medium
 * Tags: Math, Number Theory
 *
 * Problem Description:
 * Given an integer n, return _the smallest**prime palindrome** greater than or
 * equal to _`n`.
 * 
 * An integer is **prime** if it has exactly two divisors: `1` and itself. Note
 * that `1` is not a prime number.
 * 
 *   * For example, `2`, `3`, `5`, `7`, `11`, and `13` are all primes.
 * 
 * An integer is a **palindrome** if it reads the same from left to right as it
 * does from right to left.
 * 
 *   * For example, `101` and `12321` are palindromes.
 * 
 * The test cases are generated so that the answer always exists and is in the
 * range `[2, 2 * 108]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 6
 *     **Output:** 7
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 8
 *     **Output:** 11
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 13
 *     **Output:** 101
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 108`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/prime-palindrome/
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
    int primePalindrome(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 6
    // Test case 2: 8
    // Test case 3: 13

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
