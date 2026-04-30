// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 915: Partition Array into Disjoint Intervals
 * Difficulty: Medium
 * Tags: Array
 *
 * Problem Description:
 * Given an integer array `nums`, partition it into two (contiguous) subarrays
 * `left` and `right` so that:
 * 
 * * Every element in `left` is less than or equal to every element in
 * `right`.
 *   * `left` and `right` are non-empty.
 *   * `left` has the smallest possible size.
 * 
 * Return _the length of_`left` _after such a partitioning_.
 * 
 * Test cases are generated such that partitioning exists.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [5,0,3,8,6]
 *     **Output:** 3
 *     **Explanation:** left = [5,0,3], right = [8,6]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1,0,6,12]
 *     **Output:** 4
 *     **Explanation:** left = [1,1,1,0], right = [6,12]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 106`
 *   * There is at least one valid answer for the given input.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/partition-array-into-disjoint-intervals/
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
    int partitionDisjoint(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,0,3,8,6]
    // Test case 2: [1,1,1,0,6,12]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
