// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 795: Number of Subarrays with Bounded Maximum
 * Difficulty: Medium
 * Tags: Array, Two Pointers
 *
 * Problem Description:
 * Given an integer array `nums` and two integers `left` and `right`, return
 * _the
 * number of contiguous non-empty**subarrays** such that the value of the
 * maximum
 * array element in that subarray is in the range _`[left, right]`.
 * 
 * The test cases are generated so that the answer will fit in a **32-bit**
 * integer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,1,4,3], left = 2, right = 3
 *     **Output:** 3
 * **Explanation:** There are three subarrays that meet the requirements: [2],
 * [2, 1], [3].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,9,2,5,6], left = 2, right = 8
 *     **Output:** 7
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 109`
 *   * `0 <= left <= right <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-subarrays-with-bounded-maximum/
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
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1,4,3]
    // Test case 2: 2
    // Test case 3: 3
    // Test case 4: [2,9,2,5,6]
    // Test case 5: 2
    // Test case 6: 8

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
