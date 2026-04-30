// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 357: Count Numbers with Unique Digits
 * Difficulty: Medium
 * Tags: Math, Dynamic Programming, Backtracking
 *
 * Problem Description:
 * Given an integer `n`, return the count of all numbers with unique digits,
 * `x`,
 * where `0 <= x < 10n`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 2
 *     **Output:** 91
 * **Explanation:** The answer should be the total numbers in the range of 0 ≤
 * x < 100, excluding 11,22,33,44,55,66,77,88,99
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 0
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= n <= 8`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-numbers-with-unique-digits/
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
    int countNumbersWithUniqueDigits(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
