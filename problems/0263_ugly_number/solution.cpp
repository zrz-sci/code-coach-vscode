// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 263: Ugly Number
 * Difficulty: Easy
 * Tags: Math
 *
 * Problem Description:
 * An **ugly number** is a _positive_ integer which does not have a prime
 * factor
 * other than 2, 3, and 5.
 * 
 * Given an integer `n`, return `true` _if_ `n` _is an**ugly number**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 6
 *     **Output:** true
 *     **Explanation:** 6 = 2 × 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1
 *     **Output:** true
 *     **Explanation:** 1 has no prime factors.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 14
 *     **Output:** false
 *     **Explanation:** 14 is not ugly since it includes the prime factor 7.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-231 <= n <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/ugly-number/
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
    bool isUgly(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 6
    // Test case 2: 1
    // Test case 3: 14

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
