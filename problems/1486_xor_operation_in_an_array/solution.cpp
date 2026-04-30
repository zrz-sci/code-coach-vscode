// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1486: XOR Operation in an Array
 * Difficulty: Easy
 * Tags: Bit Manipulation, Math
 *
 * Problem Description:
 * You are given an integer `n` and an integer `start`.
 * 
 * Define an array `nums` where `nums[i] = start + 2 * i` (**0-indexed**) and
 * `n ==
 * nums.length`.
 * 
 * Return _the bitwise XOR of all elements of_ `nums`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 5, start = 0
 *     **Output:** 8
 * **Explanation:** Array nums is equal to [0, 2, 4, 6, 8] where (0 ^ 2 ^ 4 ^
 * 6 ^ 8) = 8.
 *     Where "^" corresponds to bitwise XOR operator.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 4, start = 3
 *     **Output:** 8
 * **Explanation:** Array nums is equal to [3, 5, 7, 9] where (3 ^ 5 ^ 7 ^ 9)
 * = 8.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 1000`
 *   * `0 <= start <= 1000`
 *   * `n == nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/xor-operation-in-an-array/
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
    int xorOperation(int n, int start) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: 0
    // Test case 3: 4
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
