// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 201: Bitwise AND of Numbers Range
 * Difficulty: Medium
 * Tags: Bit Manipulation
 *
 * Problem Description:
 * Given two integers `left` and `right` that represent the range `[left,
 * right]`,
 * return _the bitwise AND of all numbers in this range, inclusive_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** left = 5, right = 7
 *     **Output:** 4
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** left = 0, right = 0
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** left = 1, right = 2147483647
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= left <= right <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/bitwise-and-of-numbers-range/
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
    int rangeBitwiseAnd(int left, int right) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: 7
    // Test case 3: 0
    // Test case 4: 0
    // Test case 5: 1
    // Test case 6: 2147483647

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
