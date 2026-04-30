// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 326: Power of Three
 * Difficulty: Easy
 * Tags: Recursion, Math
 *
 * Problem Description:
 * Given an integer `n`, return _`true` if it is a power of three. Otherwise,
 * return `false`_.
 * 
 * An integer `n` is a power of three, if there exists an integer `x` such
 * that `n
 * == 3x`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 27
 *     **Output:** true
 *     **Explanation:** 27 = 33
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 0
 *     **Output:** false
 *     **Explanation:** There is no x where 3x = 0.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = -1
 *     **Output:** false
 *     **Explanation:** There is no x where 3x = (-1).
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
 * **Follow up:** Could you solve it without loops/recursion?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/power-of-three/
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
    bool isPowerOfThree(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 27
    // Test case 2: 0
    // Test case 3: -1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
