// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 400: Nth Digit
 * Difficulty: Medium
 * Tags: Math, Binary Search
 *
 * Problem Description:
 * Given an integer `n`, return the `nth` digit of the infinite integer
 * sequence
 * `[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ...]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 11
 *     **Output:** 0
 * **Explanation:** The 11th digit of the sequence 1, 2, 3, 4, 5, 6, 7, 8, 9,
 * 10, 11, ... is a 0, which is part of the number 10.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/nth-digit/
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
    int findNthDigit(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 11

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
