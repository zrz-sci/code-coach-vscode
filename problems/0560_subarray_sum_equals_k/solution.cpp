// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 560: Subarray Sum Equals K
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum
 *
 * Problem Description:
 * Given an array of integers `nums` and an integer `k`, return _the total
 * number
 * of subarrays whose sum equals to_ `k`.
 * 
 * A subarray is a contiguous **non-empty** sequence of elements within an
 * array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1], k = 2
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3], k = 3
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `-1000 <= nums[i] <= 1000`
 *   * `-107 <= k <= 107`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subarray-sum-equals-k/
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
    int subarraySum(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1,1]
    // Test case 2: 2
    // Test case 3: [1,2,3]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
