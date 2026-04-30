// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 878: Nth Magical Number
 * Difficulty: Hard
 * Tags: Math, Binary Search
 *
 * Problem Description:
 * A positive integer is _magical_ if it is divisible by either `a` or `b`.
 * 
 * Given the three integers `n`, `a`, and `b`, return the `nth` magical number.
 * Since the answer may be very large, **return it modulo**`109 + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 1, a = 2, b = 3
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 4, a = 2, b = 3
 *     **Output:** 6
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 109`
 *   * `2 <= a, b <= 4 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/nth-magical-number/
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
    int nthMagicalNumber(int n, int a, int b) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 2
    // Test case 3: 3
    // Test case 4: 4
    // Test case 5: 2
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
