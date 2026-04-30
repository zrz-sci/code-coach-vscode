// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 523: Continuous Subarray Sum
 * Difficulty: Medium
 * Tags: Array, Hash Table, Math, Prefix Sum
 *
 * Problem Description:
 * Given an integer array nums and an integer k, return `true` _if_`nums` _has
 * a**good subarray** or _`false` _otherwise_.
 * 
 * A **good subarray** is a subarray where:
 * 
 *   * its length is **at least two** , and
 *   * the sum of the elements of the subarray is a multiple of `k`.
 * 
 * **Note** that:
 * 
 *   * A **subarray** is a contiguous part of the array.
 * * An integer `x` is a multiple of `k` if there exists an integer `n` such
 * that `x = n * k`. `0` is **always** a multiple of `k`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [23,_2,4_ ,6,7], k = 6
 *     **Output:** true
 * **Explanation:** [2, 4] is a continuous subarray of size 2 whose elements
 * sum up to 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [_23,2,6,4,7_], k = 6
 *     **Output:** true
 * **Explanation:** [23, 2, 6, 4, 7] is an continuous subarray of size 5 whose
 * elements sum up to 42.
 *     42 is a multiple of 6 because 42 = 7 * 6 and 7 is an integer.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [23,2,6,4,7], k = 13
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 109`
 *   * `0 <= sum(nums[i]) <= 231 - 1`
 *   * `1 <= k <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/continuous-subarray-sum/
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
    bool checkSubarraySum(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [23,2,4,6,7]
    // Test case 2: 6
    // Test case 3: [23,2,6,4,7]
    // Test case 4: 6
    // Test case 5: [23,2,6,4,7]
    // Test case 6: 13

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
