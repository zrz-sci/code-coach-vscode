// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 813: Largest Sum of Averages
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Prefix Sum
 *
 * Problem Description:
 * You are given an integer array `nums` and an integer `k`. You can partition
 * the
 * array into **at most** `k` non-empty adjacent subarrays. The **score** of a
 * partition is the sum of the averages of each subarray.
 * 
 * Note that the partition must use every integer in `nums`, and that the
 * score is
 * not necessarily an integer.
 * 
 * Return _the maximum**score** you can achieve of all the possible
 * partitions_.
 * Answers within `10-6` of the actual answer will be accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [9,1,2,3,9], k = 3
 *     **Output:** 20.00000
 *     **Explanation:** 
 * The best choice is to partition nums into [9], [1, 2, 3], [9]. The answer
 * is 9 + (1 + 2 + 3) / 3 + 9 = 20.
 * We could have also partitioned nums into [9, 1], [2], [3, 9], for example.
 *     That partition would lead to a score of 5 + 2 + 6 = 13, which is worse.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,5,6,7], k = 4
 *     **Output:** 20.50000
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 100`
 *   * `1 <= nums[i] <= 104`
 *   * `1 <= k <= nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-sum-of-averages/
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
    double largestSumOfAverages(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [9,1,2,3,9]
    // Test case 2: 3
    // Test case 3: [1,2,3,4,5,6,7]
    // Test case 4: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
