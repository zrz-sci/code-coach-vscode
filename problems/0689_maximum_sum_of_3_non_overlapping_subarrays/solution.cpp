// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 689: Maximum Sum of 3 Non-Overlapping Subarrays
 * Difficulty: Hard
 * Tags: Array, Dynamic Programming, Prefix Sum, Sliding Window
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, find three non-overlapping
 * subarrays of length `k` with maximum sum and return them.
 * 
 * Return the result as a list of indices representing the starting position of
 * each interval (**0-indexed**). If there are multiple answers, return the
 * lexicographically smallest one.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,2,6,7,5,1], k = 2
 *     **Output:** [0,3,5]
 * **Explanation:** Subarrays [1, 2], [2, 6], [7, 5] correspond to the
 * starting indices [0, 3, 5].
 * We could have also taken [2, 1], but an answer of [1, 3, 5] would be
 * lexicographically larger.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,2,1,2,1,2,1], k = 2
 *     **Output:** [0,2,4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `1 <= nums[i] < 216`
 *   * `1 <= k <= floor(nums.length / 3)`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-sum-of-3-non-overlapping-subarrays/
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
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,1,2,6,7,5,1]
    // Test case 2: 2
    // Test case 3: [1,2,1,2,1,2,1,2,1]
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
