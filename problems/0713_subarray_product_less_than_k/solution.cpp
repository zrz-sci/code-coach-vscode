// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 713: Subarray Product Less Than K
 * Difficulty: Medium
 * Tags: Array, Binary Search, Prefix Sum, Sliding Window
 *
 * Problem Description:
 * Given an array of integers `nums` and an integer `k`, return _the number of
 * contiguous subarrays where the product of all the elements in the subarray
 * is
 * strictly less than_`k`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [10,5,2,6], k = 100
 *     **Output:** 8
 *     **Explanation:** The 8 subarrays that have product less than 100 are:
 *     [10], [5], [2], [6], [10, 5], [5, 2], [2, 6], [5, 2, 6]
 * Note that [10, 5, 2] is not included as the product of 100 is not strictly
 * less than k.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3], k = 0
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 3 * 104`
 *   * `1 <= nums[i] <= 1000`
 *   * `0 <= k <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subarray-product-less-than-k/
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
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,5,2,6]
    // Test case 2: 100
    // Test case 3: [1,2,3]
    // Test case 4: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
