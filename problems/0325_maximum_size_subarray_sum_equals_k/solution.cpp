// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 325: Maximum Size Subarray Sum Equals k
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return _the maximum
 * length of
 * a_ _subarray_ _that sums to_ `k`. If there is not one, return `0` instead.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,-1,5,-2,3], k = 3
 *     **Output:** 4
 * **Explanation:** The subarray [1, -1, 5, -2] sums to 3 and is the longest.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-2,-1,2,1], k = 1
 *     **Output:** 2
 *     **Explanation:** The subarray [-1, 2] sums to 1 and is the longest.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 105`
 *   * `-104 <= nums[i] <= 104`
 *   * `-109 <= k <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-size-subarray-sum-equals-k/
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
    int maxSubArrayLen(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,-1,5,-2,3]
    // Test case 2: 3
    // Test case 3: [-2,-1,2,1]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
