// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 644: Maximum Average Subarray II
 * Difficulty: Hard
 * Tags: Array, Binary Search, Prefix Sum
 *
 * Problem Description:
 * You are given an integer array `nums` consisting of `n` elements, and an
 * integer
 * `k`.
 * 
 * Find a contiguous subarray whose **length is greater than or equal to** `k`
 * that
 * has the maximum average value and return _this value_. Any answer with a
 * calculation error less than `10-5` will be accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,12,-5,-6,50,3], k = 4
 *     **Output:** 12.75000
 * **Explanation:** - When the length is 4, averages are [0.5, 12.75, 10.5]
 * and the maximum average is 12.75
 * - When the length is 5, averages are [10.4, 10.8] and the maximum average
 * is 10.8
 * - When the length is 6, averages are [9.16667] and the maximum average is
 * 9.16667
 * The maximum average is when we choose a subarray of length 4 (i.e., the sub
 * array [12, -5, -6, 50]) which has the max average 12.75, so we return 12.75
 *     Note that we do not consider the subarrays of length < 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [5], k = 1
 *     **Output:** 5.00000
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= k <= n <= 104`
 *   * `-104 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-average-subarray-ii/
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
    double findMaxAverage(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,12,-5,-6,50,3]
    // Test case 2: 4
    // Test case 3: [5]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
