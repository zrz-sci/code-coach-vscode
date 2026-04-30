// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 747: Largest Number At Least Twice of Others
 * Difficulty: Easy
 * Tags: Array, Sorting
 *
 * Problem Description:
 * You are given an integer array `nums` where the largest integer is
 * **unique**.
 * 
 * Determine whether the largest element in the array is **at least twice** as
 * much
 * as every other number in the array. If it is, return _the**index** of the
 * largest element, or return _`-1` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,6,1,0]
 *     **Output:** 1
 *     **Explanation:** 6 is the largest integer.
 *     For every other number in the array x, 6 is at least twice as big as x.
 *     The index of value 6 is 1, so we return 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4]
 *     **Output:** -1
 *     **Explanation:** 4 is less than twice the value of 3, so we return -1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 50`
 *   * `0 <= nums[i] <= 100`
 *   * The largest element in `nums` is unique.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-number-at-least-twice-of-others/
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
    int dominantIndex(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,6,1,0]
    // Test case 2: [1,2,3,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
