// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 673: Number of Longest Increasing Subsequence
 * Difficulty: Medium
 * Tags: Binary Indexed Tree, Segment Tree, Array, Dynamic Programming
 *
 * Problem Description:
 * Given an integer array `nums`, return _the number of longest increasing
 * subsequences._
 * 
 * **Notice** that the sequence has to be **strictly** increasing.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,5,4,7]
 *     **Output:** 2
 * **Explanation:** The two longest increasing subsequences are [1, 3, 4, 7]
 * and [1, 3, 5, 7].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,2,2,2]
 *     **Output:** 5
 * **Explanation:** The length of the longest increasing subsequence is 1, and
 * there are 5 increasing subsequences of length 1, so output 5.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2000`
 *   * `-106 <= nums[i] <= 106`
 *   * The answer is guaranteed to fit inside a 32-bit integer.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-longest-increasing-subsequence/
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
    int findNumberOfLIS(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,5,4,7]
    // Test case 2: [2,2,2,2,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
