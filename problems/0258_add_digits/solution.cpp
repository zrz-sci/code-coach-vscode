// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 258: Add Digits
 * Difficulty: Easy
 * Tags: Math, Number Theory, Simulation
 *
 * Problem Description:
 * Given an integer `num`, repeatedly add all its digits until the result has
 * only
 * one digit, and return it.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num = 38
 *     **Output:** 2
 *     **Explanation:** The process is
 *     38 --> 3 + 8 --> 11
 *     11 --> 1 + 1 --> 2 
 *     Since 2 has only one digit, return it.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num = 0
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= num <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:** Could you do it without any loop/recursion in `O(1)` runtime?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-digits/
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
    int addDigits(int num) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 38
    // Test case 2: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
