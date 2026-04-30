// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 172: Factorial Trailing Zeroes
 * Difficulty: Medium
 * Tags: Math
 *
 * Problem Description:
 * Given an integer `n`, return _the number of trailing zeroes in_`n!`.
 * 
 * Note that `n! = n * (n - 1) * (n - 2) * ... * 3 * 2 * 1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 0
 *     **Explanation:** 3! = 6, no trailing zero.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 5
 *     **Output:** 1
 *     **Explanation:** 5! = 120, one trailing zero.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 0
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= n <= 104`
 * 
 * 
 * 
 * **Follow up:** Could you write a solution that works in logarithmic time
 * complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/factorial-trailing-zeroes/
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
    int trailingZeroes(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 5
    // Test case 3: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
