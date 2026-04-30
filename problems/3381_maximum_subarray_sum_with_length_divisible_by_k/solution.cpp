// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 3381: Maximum Subarray Sum With Length Divisible by K
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum
 *
 * Problem Description:
 * You are given an array of integers `nums` and an integer `k`.
 * 
 * Return the **maximum** sum of a subarray of `nums`, such that the size of
 * the
 * subarray is **divisible** by `k`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** nums = [1,2], k = 1
 * 
 * **Output:** 3
 * 
 * **Explanation:**
 * 
 * The subarray `[1, 2]` with sum 3 has length equal to 2 which is divisible
 * by 1.
 * 
 * **Example 2:**
 * 
 * **Input:** nums = [-1,-2,-3,-4,-5], k = 4
 * 
 * **Output:** -10
 * 
 * **Explanation:**
 * 
 * The maximum sum subarray is `[-1, -2, -3, -4]` which has length equal to 4
 * which
 * is divisible by 4.
 * 
 * **Example 3:**
 * 
 * **Input:** nums = [-5,1,2,-3,4], k = 2
 * 
 * **Output:** 4
 * 
 * **Explanation:**
 * 
 * The maximum sum subarray is `[1, 2, -3, 4]` which has length equal to 4
 * which is
 * divisible by 2.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= nums.length <= 2 * 105`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-subarray-sum-with-length-divisible-by-k/
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
    long long maxSubarraySum(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2]
    // Test case 2: 1
    // Test case 3: [-1,-2,-3,-4,-5]
    // Test case 4: 4
    // Test case 5: [-5,1,2,-3,4]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
