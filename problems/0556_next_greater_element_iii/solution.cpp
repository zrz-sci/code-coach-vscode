// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 556: Next Greater Element III
 * Difficulty: Medium
 * Tags: Math, Two Pointers, String
 *
 * Problem Description:
 * Given a positive integer `n`, find _the smallest integer which has exactly
 * the
 * same digits existing in the integer_ `n` _and is greater in value than_
 * `n`. If
 * no such positive integer exists, return `-1`.
 * 
 * **Note** that the returned integer should fit in **32-bit integer** , if
 * there
 * is a valid answer but it does not fit in **32-bit integer** , return `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 12
 *     **Output:** 21
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 21
 *     **Output:** -1
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
 * Link: https://leetcode.com/problems/next-greater-element-iii/
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
    int nextGreaterElement(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 12
    // Test case 2: 21

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
