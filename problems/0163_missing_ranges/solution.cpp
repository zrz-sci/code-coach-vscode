// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 163: Missing Ranges
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * You are given an inclusive range `[lower, upper]` and a **sorted unique**
 * integer array `nums`, where all elements are within the inclusive range.
 * 
 * A number `x` is considered **missing** if `x` is in the range `[lower,
 * upper]`
 * and `x` is not in `nums`.
 * 
 * Return _the**shortest sorted** list of ranges that **exactly covers all the
 * missing numbers**_. That is, no element of `nums` is included in any of the
 * ranges, and each missing number is covered by one of the ranges.
 * 
 * 
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,3,50,75], lower = 0, upper = 99
 *     **Output:** [[2,2],[4,49],[51,74],[76,99]]
 *     **Explanation:** The ranges are:
 *     [2,2]
 *     [4,49]
 *     [51,74]
 *     [76,99]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-1], lower = -1, upper = -1
 *     **Output:** []
 * **Explanation:** There are no missing ranges since there are no missing
 * numbers.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-109 <= lower <= upper <= 109`
 *   * `0 <= nums.length <= 100`
 *   * `lower <= nums[i] <= upper`
 *   * All the values of `nums` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/missing-ranges/
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
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,3,50,75]
    // Test case 2: 0
    // Test case 3: 99
    // Test case 4: [-1]
    // Test case 5: -1
    // Test case 6: -1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
