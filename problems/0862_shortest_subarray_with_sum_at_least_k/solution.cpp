// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 862: Shortest Subarray with Sum at Least K
 * Difficulty: Hard
 * Tags: Queue, Array, Binary Search, Prefix Sum, Sliding Window, Monotonic Queue, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return _the length of the
 * shortest non-empty**subarray** of _`nums` _with a sum of at least_`k`. If
 * there
 * is no such **subarray** , return `-1`.
 * 
 * A **subarray** is a **contiguous** part of an array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1], k = 1
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2], k = 4
 *     **Output:** -1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [2,-1,2], k = 3
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-105 <= nums[i] <= 105`
 *   * `1 <= k <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/
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
    int shortestSubarray(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1]
    // Test case 2: 1
    // Test case 3: [1,2]
    // Test case 4: 4
    // Test case 5: [2,-1,2]
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
