// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 220: Contains Duplicate III
 * Difficulty: Hard
 * Tags: Array, Bucket Sort, Ordered Set, Sorting, Sliding Window
 *
 * Problem Description:
 * You are given an integer array `nums` and two integers `indexDiff` and
 * `valueDiff`.
 * 
 * Find a pair of indices `(i, j)` such that:
 * 
 *   * `i != j`,
 *   * `abs(i - j) <= indexDiff`.
 *   * `abs(nums[i] - nums[j]) <= valueDiff`, and
 * 
 * Return `true` _if such pair exists or_`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,1], indexDiff = 3, valueDiff = 0
 *     **Output:** true
 *     **Explanation:** We can choose (i, j) = (0, 3).
 *     We satisfy the three conditions:
 *     i != j --> 0 != 3
 *     abs(i - j) <= indexDiff --> abs(0 - 3) <= 3
 *     abs(nums[i] - nums[j]) <= valueDiff --> abs(1 - 1) <= 0
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,5,9,1,5,9], indexDiff = 2, valueDiff = 3
 *     **Output:** false
 * **Explanation:** After trying all the possible pairs (i, j), we cannot
 * satisfy the three conditions, so we return false.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 *   * `1 <= indexDiff <= nums.length`
 *   * `0 <= valueDiff <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/contains-duplicate-iii/
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
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,1]
    // Test case 2: 3
    // Test case 3: 0
    // Test case 4: [1,5,9,1,5,9]
    // Test case 5: 2
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
