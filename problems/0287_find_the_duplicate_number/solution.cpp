// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 287: Find the Duplicate Number
 * Difficulty: Medium
 * Tags: Bit Manipulation, Array, Two Pointers, Binary Search
 *
 * Problem Description:
 * Given an array of integers `nums` containing `n + 1` integers where each
 * integer
 * is in the range `[1, n]` inclusive.
 * 
 * There is only **one repeated number** in `nums`, return _this repeated
 * number_.
 * 
 * You must solve the problem **without** modifying the array `nums` and using
 * only
 * constant extra space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,4,2,2]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,1,3,4,2]
 *     **Output:** 3
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [3,3,3,3,3]
 *     **Output:** 3
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 105`
 *   * `nums.length == n + 1`
 *   * `1 <= nums[i] <= n`
 * * All the integers in `nums` appear only **once** except for **precisely
 * one integer** which appears **two or more** times.
 * 
 * 
 * 
 * **Follow up:**
 * 
 * * How can we prove that at least one duplicate number must exist in `nums`?
 *   * Can you solve the problem in linear runtime complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-duplicate-number/
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
    int findDuplicate(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,4,2,2]
    // Test case 2: [3,1,3,4,2]
    // Test case 3: [3,3,3,3,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
