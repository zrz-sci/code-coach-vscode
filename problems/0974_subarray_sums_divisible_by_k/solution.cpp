// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 974: Subarray Sums Divisible by K
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return _the number of non-
 * empty**subarrays** that have a sum divisible by _`k`.
 * 
 * A **subarray** is a **contiguous** part of an array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,5,0,-2,-3,1], k = 5
 *     **Output:** 7
 *     **Explanation:** There are 7 subarrays with a sum divisible by k = 5:
 * [4, 5, 0, -2, -3, 1], [5], [5, 0], [5, 0, -2, -3], [0], [0, -2, -3], [-2,
 * -3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [5], k = 9
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 3 * 104`
 *   * `-104 <= nums[i] <= 104`
 *   * `2 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subarray-sums-divisible-by-k/
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
    int subarraysDivByK(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,5,0,-2,-3,1]
    // Test case 2: 5
    // Test case 3: [5]
    // Test case 4: 9

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
