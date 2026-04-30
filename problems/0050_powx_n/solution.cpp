// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 50: Pow(x, n)
 * Difficulty: Medium
 * Tags: Recursion, Math
 *
 * Problem Description:
 * Implement [pow(x, n)](http://www.cplusplus.com/reference/valarray/pow/),
 * which
 * calculates `x` raised to the power `n` (i.e., `xn`).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** x = 2.00000, n = 10
 *     **Output:** 1024.00000
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** x = 2.10000, n = 3
 *     **Output:** 9.26100
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** x = 2.00000, n = -2
 *     **Output:** 0.25000
 *     **Explanation:** 2-2 = 1/22 = 1/4 = 0.25
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-100.0 < x < 100.0`
 *   * `-231 <= n <= 231-1`
 *   * `n` is an integer.
 *   * Either `x` is not zero or `n > 0`.
 *   * `-104 <= xn <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/powx-n/
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
    double myPow(double x, int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2.00000
    // Test case 2: 10
    // Test case 3: 2.10000
    // Test case 4: 3
    // Test case 5: 2.00000
    // Test case 6: -2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
