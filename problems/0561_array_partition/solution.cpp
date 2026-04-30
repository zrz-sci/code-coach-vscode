// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 561: Array Partition
 * Difficulty: Easy
 * Tags: Greedy, Array, Counting Sort, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` of `2n` integers, group these integers into
 * `n`
 * pairs `(a1, b1), (a2, b2), ..., (an, bn)` such that the sum of `min(ai,
 * bi)` for
 * all `i` is **maximized**. Return _the maximized sum_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,4,3,2]
 *     **Output:** 4
 * **Explanation:** All possible pairings (ignoring the ordering of elements)
 * are:
 *     1. (1, 4), (2, 3) -> min(1, 4) + min(2, 3) = 1 + 2 = 3
 *     2. (1, 3), (2, 4) -> min(1, 3) + min(2, 4) = 1 + 2 = 3
 *     3. (1, 2), (3, 4) -> min(1, 2) + min(3, 4) = 1 + 3 = 4
 *     So the maximum possible sum is 4.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [6,2,6,5,1,2]
 *     **Output:** 9
 * **Explanation:** The optimal pairing is (2, 1), (2, 5), (6, 6). min(2, 1) +
 * min(2, 5) + min(6, 6) = 1 + 2 + 6 = 9.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 104`
 *   * `nums.length == 2 * n`
 *   * `-104 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/array-partition/
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
    int arrayPairSum(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,4,3,2]
    // Test case 2: [6,2,6,5,1,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
