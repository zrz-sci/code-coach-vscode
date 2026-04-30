// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 910: Smallest Range II
 * Difficulty: Medium
 * Tags: Greedy, Array, Math, Sorting
 *
 * Problem Description:
 * You are given an integer array `nums` and an integer `k`.
 * 
 * For each index `i` where `0 <= i < nums.length`, change `nums[i]` to be
 * either
 * `nums[i] + k` or `nums[i] - k`.
 * 
 * The **score** of `nums` is the difference between the maximum and minimum
 * elements in `nums`.
 * 
 * Return _the minimum**score** of _`nums` _after changing the values at each
 * index_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1], k = 0
 *     **Output:** 0
 *     **Explanation:** The score is max(nums) - min(nums) = 1 - 1 = 0.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,10], k = 2
 *     **Output:** 6
 * **Explanation:** Change nums to be [2, 8]. The score is max(nums) -
 * min(nums) = 8 - 2 = 6.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,6], k = 3
 *     **Output:** 3
 * **Explanation:** Change nums to be [4, 6, 3]. The score is max(nums) -
 * min(nums) = 6 - 3 = 3.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `0 <= nums[i] <= 104`
 *   * `0 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/smallest-range-ii/
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
    int smallestRangeII(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1]
    // Test case 2: 0
    // Test case 3: [0,10]
    // Test case 4: 2
    // Test case 5: [1,3,6]
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
