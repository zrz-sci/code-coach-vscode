// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 674: Longest Continuous Increasing Subsequence
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * Given an unsorted array of integers `nums`, return _the length of the
 * longest**continuous increasing subsequence** (i.e. subarray)_. The
 * subsequence
 * must be **strictly** increasing.
 * 
 * A **continuous increasing subsequence** is defined by two indices `l` and
 * `r`
 * (`l < r`) such that it is `[nums[l], nums[l + 1], ..., nums[r - 1],
 * nums[r]]`
 * and for each `l <= i < r`, `nums[i] < nums[i + 1]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,5,4,7]
 *     **Output:** 3
 * **Explanation:** The longest continuous increasing subsequence is [1,3,5]
 * with length 3.
 * Even though [1,3,5,7] is an increasing subsequence, it is not continuous as
 * elements 5 and 7 are separated by element
 *     4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,2,2,2]
 *     **Output:** 1
 * **Explanation:** The longest continuous increasing subsequence is [2] with
 * length 1. Note that it must be strictly
 *     increasing.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-continuous-increasing-subsequence/
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
    int findLengthOfLCIS(vector<int>& nums) {
        
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
